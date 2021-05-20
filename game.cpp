#pragma once
#include "include/game.h"
#include "include/entity.h"
#include "include/Components.h"
#include "include/Map.h"
#include "include/Vector2D.h"
#include "include/Collision.h"
#include "SDL2/SDL_mixer.h"
#include <sstream>

Map *MAP;
Manager manager;

SDL_Renderer *Game::ren = nullptr;
SDL_Event Game::event;

std::vector<ColliderComponent *> Game::colliders;
std::map<std::string, TTF_Font *> Game::fonts;
std::vector<std::pair<std::pair<int, int>, bool>> Game::collectibleStatus;
bool Game::trigger[4] = {false, false, false, false};
bool Game::triggerChange[4] = {false, false, false, false};
bool Game::lastSeconds[4] = {false, false, false, false};
int Game::playerID = 0;
UserInput Game::userInput = UserInput::DUMMY;
UserInput Game::otherUserInput = UserInput::DUMMY;
bool Game::toUpdate = true;
std::vector<Vector2D> Game::initialPos;
int livesleft = 3;
bool Game::GameOver = false;
bool Game::dead = false;
bool Game::restart = false;
int CollectibleCount = 0;

Mix_Music *music = NULL;
Mix_Chunk *chomp = NULL;
Mix_Chunk *death = NULL;
Mix_Chunk *eatGhost = NULL;
Mix_Chunk *intermission = NULL;

auto &player(manager.addEntity());
auto &enemy1(manager.addEntity());
auto &enemy2(manager.addEntity());
auto &enemy3(manager.addEntity());
auto &enemy4(manager.addEntity());
auto &label(manager.addEntity());
auto &label2(manager.addEntity());
auto &gameOverLabel(manager.addEntity());
auto &P1wonLabel(manager.addEntity());
auto &P2wonLabel(manager.addEntity());

enum groupLabels : std::size_t
{
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders,
    groupCollectibles
};

Game::Game()
{
}

Game::~Game()
{
}

void Game::init(int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "SDL initialized\n";
        win = SDL_CreateWindow("game", xpos, ypos, width, height, flags);
        if (win)
        {
            std::cout << "window created\n";
        }
        ren = SDL_CreateRenderer(win, -1, 0);
        if (ren)
        {
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            std::cout << "Renderer created\n";
        }
        isRunning = true;
    }

    // [rmnt]: Connect to server
    if (!Connect("127.0.0.1", 60000))
    {
        std::cout << "Couldn't connect to server\n";
    }

    MAP = new Map();
    Map::LoadMap("map0.txt", 32, 24);

    if (TTF_Init() == -1)
    {
        std::cout << "Error with SDL_ttf\n";
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }

    music = Mix_LoadMUS("Assets/pacman_beginning.wav");
    Mix_PlayMusic(music, 1);

    chomp = Mix_LoadWAV("Assets/pacman_chomp.wav");
    death = Mix_LoadWAV("Assets/pacman_death.wav");
    eatGhost = Mix_LoadWAV("Assets/pacman_eatghost.wav");
    intermission = Mix_LoadWAV("Assets/pacman_intermission.wav");

    player.addComponent<TransformComponent>(40, 40);
    player.addComponent<SpriteComponent>("Assets/xx.png", true, 0);
    player.addComponent<Controller>(this);
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);

    enemy1.addComponent<TransformComponent>(440, 400);
    enemy1.addComponent<SpriteComponent>("Assets/monsters1.png", true, 1);
    enemy1.addComponent<ColliderComponent>("enemy");
    enemy1.addComponent<enemyController>(1, this);
    enemy1.addGroup(groupEnemies);
    initialPos.push_back(enemy1.getComponent<TransformComponent>().position);

    enemy2.addComponent<TransformComponent>(480, 400);
    enemy2.addComponent<SpriteComponent>("Assets/monsters1.png", true, 2);
    enemy2.addComponent<ColliderComponent>("enemy");
    enemy2.addComponent<enemyController>(2, this);
    enemy2.addGroup(groupEnemies);
    initialPos.push_back(enemy2.getComponent<TransformComponent>().position);

    enemy3.addComponent<TransformComponent>(520, 400);
    enemy3.addComponent<SpriteComponent>("Assets/monsters1.png", true, 3);
    enemy3.addComponent<ColliderComponent>("enemy");
    enemy3.addComponent<enemyController>(3, this);
    enemy3.addGroup(groupEnemies);
    initialPos.push_back(enemy3.getComponent<TransformComponent>().position);

    enemy4.addComponent<TransformComponent>(560, 400);
    enemy4.addComponent<SpriteComponent>("Assets/monsters1.png", true, 4);
    enemy4.addComponent<ColliderComponent>("enemy");
    enemy4.addComponent<enemyController>(4, this);
    enemy4.addGroup(groupEnemies);
    initialPos.push_back(enemy4.getComponent<TransformComponent>().position);
    initialPos.push_back(player.getComponent<TransformComponent>().position);

    SDL_Color white = {255, 255, 255, 255};
    AddFont("Itim", "Assets/Itim.ttf", 20);
    AddFont("Itim2", "Assets/Itim.ttf", 60);
    AddFont("Itim3", "Assets/Itim.ttf", 30);
    label.addComponent<UI>(10, 10, "Lives left: 3", "Itim", white);
    label2.addComponent<UI>(150, 10, "Score: 0", "Itim", white);
    gameOverLabel.addComponent<UI>(480, 360, "GAME OVER", "Itim2", white);
    P1wonLabel.addComponent<UI>(550, 430, "Player 1 won!", "Itim3", white);
    P2wonLabel.addComponent<UI>(550, 430, "Player 2 won!", "Itim3", white);
}

auto &tiles(manager.getGroup(groupMap));
auto &players(manager.getGroup(groupPlayers));
auto &enemies(manager.getGroup(groupEnemies));
auto &collectibles(manager.getGroup(groupCollectibles));

void Game ::handleEvents()
{

    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_r)
        {
            restart = true;
        }
        break;
    default:
        break;
    }
}

void Game::update()
{
    Vector2D playerpos = player.getComponent<TransformComponent>().position;
    Vector2D enemypos1 = enemy1.getComponent<TransformComponent>().position;
    Vector2D enemypos2 = enemy2.getComponent<TransformComponent>().position;
    Vector2D enemypos3 = enemy3.getComponent<TransformComponent>().position;
    Vector2D enemypos4 = enemy4.getComponent<TransformComponent>().position;
    if (IsConnected())
    {
        while (!Incoming().empty())
        {
            auto msg = Incoming().pop_front().msg;

            switch (msg.header.id)
            {
            case GameMsg::Client_AssignID:
            {
                msg >> Game::playerID;
                std::cout << "Player ID: " << Game::playerID << "\n";
                break;
            }

            case GameMsg::Game_UserInput:
            {
                msg >> Game::otherUserInput;
                if (Game::otherUserInput != UserInput::DUMMY)
                {
                    std::cout << "Received Other User Input: " << int(Game::otherUserInput) << "\n";

                    if (playerID == 1)
                    {
                        Game::toUpdate = true;
                        enemy1.getComponent<enemyController>().update();
                        enemy2.getComponent<enemyController>().update();
                        enemy3.getComponent<enemyController>().update();
                        enemy4.getComponent<enemyController>().update();
                    }
                    else
                    {
                        Game::toUpdate = true;
                        player.getComponent<Controller>().update();
                    }
                }
                break;
            }
            }
        }
    }
    if (!GameOver)
    {
        std::stringstream ss;
        ss << "Lives left: " << livesleft;
        label.getComponent<UI>().SetText(ss.str(), "Itim");
        manager.refresh();
        Game::toUpdate = false;
        manager.update();

        for (int i = 0; i < colliders.size(); i++)
        {
            auto &c = colliders[i];
            if (Collision::coll(*c, player.getComponent<ColliderComponent>()))
            {
                if (c->tag == "obstacle")
                {
                    player.getComponent<TransformComponent>().position = playerpos;
                }
                else if (c->tag == "collectible")
                {
                    Mix_PlayChannel(-1, chomp, 0);
                    count++;
                    std::stringstream ss1;
                    ss1 << "Score: " << count;
                    label2.getComponent<UI>().SetText(ss1.str(), "Itim");
                    int xp = c->transform->position.x;
                    int yp = c->transform->position.y;
                    for (int j = 0; j < collectibleStatus.size(); j++)
                    {
                        if (collectibleStatus[j].first == make_pair(xp, yp))
                        {
                            collectibleStatus[j].second = false;
                        }
                    }
                    colliders.erase(colliders.begin() + i);
                    render();
                    break;
                }
                else if (c->tag == "trigger")
                {
                    timeTicks = SDL_GetTicks();
                    Mix_PlayChannel(-1, intermission, 0);
                    for (int k = 0; k < 4; k++)
                    {
                        trigger[k] = true;
                        lastSeconds[k] = false;
                        triggerChange[k] = false;
                    }
                    int xp = c->transform->position.x;
                    int yp = c->transform->position.y;
                    for (int j = 0; j < collectibleStatus.size(); j++)
                    {
                        if (collectibleStatus[j].first == make_pair(xp, yp))
                        {
                            collectibleStatus[j].second = false;
                        }
                    }
                    colliders.erase(colliders.begin() + i);
                    count += 2;
                    std::stringstream ss1;
                    ss1 << "Score: " << count;
                    label2.getComponent<UI>().SetText(ss1.str(), "Itim");
                    render();
                    break;
                }
                else if (c->tag == "enemy")
                {
                    int mon = c->entity->getComponent<enemyController>().num;
                    if (trigger[mon - 1])
                    {
                        Mix_PlayChannel(-1, eatGhost, 0);
                        trigger[mon - 1] = false;
                        lastSeconds[mon - 1] = false;
                        triggerChange[mon - 1] = true;
                        c->entity->getComponent<TransformComponent>().position = initialPos[mon - 1];
                        if (mon == 1)
                        {
                            enemypos1 = initialPos[0];
                        }
                        else if (mon == 2)
                        {
                            enemypos2 = initialPos[1];
                        }
                        else if (mon == 3)
                        {
                            enemypos3 = initialPos[2];
                        }
                        else if (mon == 4)
                        {
                            enemypos4 = initialPos[3];
                        }
                        count += 10;
                        std::stringstream ss1;
                        ss1 << "Score: " << count;
                        label2.getComponent<UI>().SetText(ss1.str(), "Itim");
                        render();
                        break;
                    }
                    else
                    {
                        dead = true;
                        c->entity->getComponent<TransformComponent>().position = initialPos[mon - 1];
                        player.getComponent<TransformComponent>().velocity.Zero();
                        Mix_PlayChannel(-1, death, 0);
                        player.getComponent<SpriteComponent>().Play("die");
                        if (livesleft == 0)
                        {
                            // isRunning = false;
                            GameOver = true;
                            render();
                            break;
                        }
                        livesleft--;
                        break;
                    }
                }
            }
        }

        for (int k = 0; k < 4; k++)
        {
            if (trigger[k])
            {
                int timeTicks2 = SDL_GetTicks();
                if (timeTicks2 - timeTicks > 15000)
                {
                    trigger[k] = false;
                    lastSeconds[k] = false;
                    triggerChange[k] = true;
                }
                else if (timeTicks2 - timeTicks > 11000)
                {
                    lastSeconds[k] = true;
                }
            }
        }

        for (int i = 0; i < colliders.size(); i++)
        {
            auto &c = colliders[i];
            if (c->tag != "player" and c->tag != "collectible" and c->tag != "trigger" and Collision::coll(*c, enemy1.getComponent<ColliderComponent>()))
            {
                if (c->tag == "obstacle")
                {
                    enemy1.getComponent<TransformComponent>().position = enemypos1;
                }
            }
            if (c->tag != "player" and c->tag != "collectible" and c->tag != "trigger" and Collision::coll(*c, enemy2.getComponent<ColliderComponent>()))
            {
                if (c->tag == "obstacle")
                {
                    enemy2.getComponent<TransformComponent>().position = enemypos2;
                }
            }
            if (c->tag != "player" and c->tag != "collectible" and c->tag != "trigger" and Collision::coll(*c, enemy3.getComponent<ColliderComponent>()))
            {
                if (c->tag == "obstacle")
                {
                    enemy3.getComponent<TransformComponent>().position = enemypos3;
                }
            }
            if (c->tag != "player" and c->tag != "collectible" and c->tag != "trigger" and Collision::coll(*c, enemy4.getComponent<ColliderComponent>()))
            {
                if (c->tag == "obstacle")
                {
                    enemy4.getComponent<TransformComponent>().position = enemypos4;
                }
            }
        }

        if (count >= CollectibleCount)
        {
            GameOver = true;
            render();
        }

        // message<GameMsg> msg;
        // msg.header.id = GameMsg::Game_UserInput;
        // msg << Game::userInput;
        // Send(msg);
        // if (Game::userInput != UserInput::DUMMY)
        // {
        //     std::cout << "[Player " << Game::playerID << "]: Sent user input: " << int(Game::userInput) << "\n";
        // }
    }

    // for (auto& c : colliders)
    // {
    //     if (Collision::coll(*c, player.getComponent<ColliderComponent>()))
    //     {
    //         if (c->tag == "obstacle"){
    //             player.getComponent<TransformComponent>().position = playerpos;
    //         }
    //         else if (c->tag == "collectible")
    //         {
    //             c->Destroy();
    //             break;
    //         }
    //     }

    // }
}

void Game::render()
{
    if (!GameOver)
    {
        SDL_RenderClear(ren);
        for (auto &t : tiles)
        {
            t->draw();
        }
        for (auto &c : collectibles)
        {
            bool drawable = true;
            for (int j = 0; j < collectibleStatus.size(); j++)
            {
                if (c->getComponent<TransformComponent>().position.x == collectibleStatus[j].first.first and c->getComponent<TransformComponent>().position.y == collectibleStatus[j].first.second)
                {
                    if (!collectibleStatus[j].second)
                    {
                        drawable = false;
                    }
                }
            }
            if (drawable)
            {
                c->draw();
            }
        }
        for (auto &p : players)
        {
            p->draw();
        }
        for (auto &e : enemies)
        {
            e->draw();
        }
        label.draw();
        label2.draw();
        SDL_RenderPresent(ren);
    }
    else
    {
        SDL_RenderClear(ren);
        for (auto &t : tiles)
        {
            t->draw();
        }
        gameOverLabel.draw();
        if (count >= CollectibleCount)
        {
            P1wonLabel.draw();
        }
        else
        {
            P2wonLabel.draw();
        }
        SDL_RenderPresent(ren);
    }
}

void Game::clean()
{
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
    std::cout << "quit\n";
}

void Game::AddTile(int id, int x, int y)
{
    auto &tile(manager.addEntity());
    tile.addComponent<TileComponent>(x, y, 40, 40, id);
    if (id == 1)
    {
        tile.addComponent<ColliderComponent>("obstacle");
    }
    tile.addGroup(groupMap);
}

void Game::AddCollectible(int id, int x, int y)
{
    CollectibleCount++;
    auto &collectibles(manager.addEntity());
    collectibles.addComponent<CollectibleComponent>(x, y, 40, 40, id);
    if (id == 2)
    {
        collectibles.addComponent<ColliderComponent>("collectible");
    }
    else
    {
        collectibles.addComponent<ColliderComponent>("trigger");
    }
    collectibles.addGroup(groupCollectibles);
    collectibleStatus.push_back(std::make_pair(std::make_pair(x, y), true));
}

void Game::AddFont(std::string id, std::string path, int fontSize)
{
    fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font *Game::GetFont(std::string id)
{
    return fonts[id];
}