#pragma once
#include "include/game.h"
#include "include/entity.h"
#include "include/Components.h"
#include "include/Map.h"
#include "include/Vector2D.h"
#include "include/Collision.h"
#include "SDL2/SDL_mixer.h"
#include "include/maze.h"
#include <sstream>


Map* MAP;
Manager manager;

SDL_Renderer* Game::ren = nullptr;
SDL_Event Game::event;

std::vector<ColliderComponent*> Game::colliders;
std::map<std::string, TTF_Font*> Game::fonts;
std::vector<std::pair<std::pair<int, int>, bool>> Game::collectibleStatus;
bool Game::trigger[4] = {false, false, false, false};
bool Game::triggerChange[4] = {false, false, false, false};
bool Game::lastSeconds[4] = {false, false, false, false};
Vector2D Game::initialPos;
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

auto& player(manager.addEntity());
auto& enemy1(manager.addEntity());
auto& enemy2(manager.addEntity());
auto& enemy3(manager.addEntity());
auto& enemy4(manager.addEntity());
auto& label(manager.addEntity());
auto& label2(manager.addEntity());
auto& gameOverLabel(manager.addEntity());
auto& P1wonLabel(manager.addEntity());
auto& P2wonLabel(manager.addEntity());

enum groupLabels : std::size_t
{
    groupMap,
    groupEnemies,
    groupColliders,
    groupCollectibles
};

Game::Game()
{}

Game::~Game()
{}

void Game::init(int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen){
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
    }
    win = SDL_CreateWindow("game", xpos, ypos, width, height, flags);
    if (win == NULL){
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
    }
    ren = SDL_CreateRenderer(win, -1, 0);
    if (ren == NULL){
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << "\n";
    }
    SDL_SetRenderDrawColor(ren, 255,255,255,255);
    isRunning = true;
    MAP = new Map();
    Map::LoadMap("maze.txt", 31, 23);

    if (TTF_Init() == -1)
    {
        std::cout << "SDL_ttf could not initialize! SDL_Error: " << SDL_GetError() << "\n";
    }

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    music = Mix_LoadMUS("Assets/pacman_beginning.wav");
    Mix_PlayMusic( music, 1);

    chomp = Mix_LoadWAV("Assets/pacman_chomp2.wav");
    death = Mix_LoadWAV("Assets/pacman_death.wav");
    eatGhost = Mix_LoadWAV("Assets/pacman_eatghost.wav");
    intermission = Mix_LoadWAV("Assets/pacman_intermission.wav");

    pair<int, int> p = getCoordinates();
    player.addComponent<TransformComponent>(p.first,p.second);
    player.addComponent<SpriteComponent>("Assets/xx.png", true, 0);
    player.addComponent<Controller>();
    player.addComponent<ColliderComponent>("player");
    initialPos = player.getComponent<TransformComponent>().position;

    pair<int, int> e = getCoordinates();
    while (p == e) {e = getCoordinates();}
    enemy1.addComponent<TransformComponent>(e.first, e.second);
    enemy1.addComponent<SpriteComponent>("Assets/monsters1.png", true, 1);
    enemy1.addComponent<ColliderComponent>("enemy");
    enemy1.addComponent<enemyController>(1);
    enemy1.addGroup(groupEnemies);

    e = getCoordinates();
    while (p == e) {e = getCoordinates();}
    enemy2.addComponent<TransformComponent>(e.first, e.second);
    enemy2.addComponent<SpriteComponent>("Assets/monsters1.png", true, 2);
    enemy2.addComponent<ColliderComponent>("enemy");
    enemy2.addComponent<enemyController>(2);
    enemy2.addGroup(groupEnemies);

    e = getCoordinates();
    while (p == e) {e = getCoordinates();}
    enemy3.addComponent<TransformComponent>(e.first, e.second);
    enemy3.addComponent<SpriteComponent>("Assets/monsters1.png", true, 3);
    enemy3.addComponent<ColliderComponent>("enemy");
    enemy3.addComponent<enemyController>(3);
    enemy3.addGroup(groupEnemies);
    
    e = getCoordinates();
    while (p == e) {e = getCoordinates();}
    enemy4.addComponent<TransformComponent>(e.first, e.second);
    enemy4.addComponent<SpriteComponent>("Assets/monsters1.png", true, 4);
    enemy4.addComponent<ColliderComponent>("enemy");
    enemy4.addComponent<enemyController>(4);
    enemy4.addGroup(groupEnemies);

    SDL_Color white = {255, 255, 255, 255};
    // AddFont("Itim", "Assets/Itim.ttf", 20);
    // AddFont("Itim2", "Assets/Itim.ttf", 60);
    AddFont("Lobster", "Assets/Lobster.ttf", 22);
    AddFont("Lobster2", "Assets/Lobster.ttf", 30);
    AddFont("Girassol", "Assets/Girassol.ttf", 70);
    label.addComponent<UI>(10, 5, "Lives left: 3", "Lobster", white);
    label2.addComponent<UI>(150, 5, "Score: 0", "Lobster", white);
    gameOverLabel.addComponent<UI>(480, 360, "GAME OVER", "Girassol", white);
    P1wonLabel.addComponent<UI>(550, 450, "player 1 won!", "Lobster2", white);
    P2wonLabel.addComponent<UI>(550, 450, "player 2 won!", "Lobster2", white);
}

auto& tiles(manager.getGroup(groupMap));
auto& enemies(manager.getGroup(groupEnemies));
auto& collectibles(manager.getGroup(groupCollectibles));

void Game :: handleEvents()
{
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
}

void Game:: update()
{
    if (!GameOver){
        Vector2D playerpos = player.getComponent<TransformComponent>().position;
        Vector2D enemypos1 = enemy1.getComponent<TransformComponent>().position;
        Vector2D enemypos2 = enemy2.getComponent<TransformComponent>().position;
        Vector2D enemypos3 = enemy3.getComponent<TransformComponent>().position;
        Vector2D enemypos4 = enemy4.getComponent<TransformComponent>().position;
        std::stringstream ss;
        ss << "Lives left: " << livesleft;
        label.getComponent<UI>().SetText(ss.str(), "Lobster");
        manager.refresh();
        manager.update();

        for (int i=0; i<colliders.size(); i++){
            auto& c = colliders[i];
            if (Collision::coll(*c, player.getComponent<ColliderComponent>()))
            {
                if (c->tag == "obstacle"){
                    player.getComponent<TransformComponent>().position = playerpos;
                }
                else if (c->tag == "collectible")
                {
                    Mix_PlayChannel(-1, chomp, 0);
                    count++;
                    std::stringstream ss1;
                    ss1 << "Score: " << count;
                    label2.getComponent<UI>().SetText(ss1.str(), "Lobster");
                    int xp = c->transform->position.x;
                    int yp = c->transform->position.y;
                    for (int j=0; j<collectibleStatus.size(); j++){
                        if (collectibleStatus[j].first == make_pair(xp, yp)){
                            collectibleStatus[j].second = false;
                        }
                    }
                    colliders.erase(colliders.begin() + i);
                    render();
                    break;
                }
                else if (c->tag == "trigger"){
                    timeTicks = SDL_GetTicks();
                    Mix_PlayChannel(-1, intermission, 0);
                    for (int k =0; k<4; k++){
                        trigger[k] = true;
                        lastSeconds[k] = false;
                        triggerChange[k] = false;
                    }
                    int xp = c->transform->position.x;
                    int yp = c->transform->position.y;
                    for (int j=0; j<collectibleStatus.size(); j++){
                        if (collectibleStatus[j].first == make_pair(xp, yp)){
                            collectibleStatus[j].second = false;
                        }
                    }
                    colliders.erase(colliders.begin() + i);
                    count += 2;
                    std::stringstream ss1;
                    ss1 << "Score: " << count;
                    label2.getComponent<UI>().SetText(ss1.str(), "Lobster");
                    render();
                    break;
                }
                else if (c->tag == "enemy"){
                    int mon = c->entity->getComponent<enemyController>().num;
                    if (trigger[mon - 1]){
                        Mix_PlayChannel(-1, eatGhost, 0);
                        trigger[mon -1] = false;
                        lastSeconds[mon-1] = false;
                        triggerChange[mon - 1] = true;
                        pair<int, int> e = getCoordinates();
                        while (e.first == playerpos.x and e.second == playerpos.y) {e = getCoordinates();}
                        c->entity->getComponent<TransformComponent>().position = Vector2D(e.first, e.second);
                        count += 5;
                        std::stringstream ss1;
                        ss1 << "Score: " << count;
                        label2.getComponent<UI>().SetText(ss1.str(), "Lobster");
                        render();
                        break;
                    }
                    else {
                        dead = true;
                        pair<int, int> e = getCoordinates();
                        while (e.first == playerpos.x and e.second == playerpos.y) {e = getCoordinates();}
                        c->entity->getComponent<TransformComponent>().position = Vector2D(e.first, e.second);
                        player.getComponent<TransformComponent>().velocity.Zero();
                        Mix_PlayChannel(-1, death, 0);
                        player.getComponent<SpriteComponent>().Play("die");
                        if (livesleft == 0){
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


        for (int k=0; k<4; k++){
            if (trigger[k]){
                int timeTicks2 = SDL_GetTicks();
                if (timeTicks2 - timeTicks > 15000){
                    trigger[k] = false;
                    lastSeconds[k] = false;
                    triggerChange[k] = true;
                }
                else if (timeTicks2 - timeTicks > 11000){
                    lastSeconds[k] = true;
                }
            }
        }

        for (int i=0; i<colliders.size(); i++){
            auto& c = colliders[i];
            if (c->tag != "player" and c->tag != "collectible" and c->tag != "trigger" and Collision::coll(*c, enemy1.getComponent<ColliderComponent>()))
            {
                if (c->tag == "obstacle"){
                    enemy1.getComponent<TransformComponent>().position = enemypos1;
                }
            }
            if (c->tag != "player" and c->tag != "collectible" and c->tag != "trigger" and Collision::coll(*c, enemy2.getComponent<ColliderComponent>()))
            {
                if (c->tag == "obstacle"){
                    enemy2.getComponent<TransformComponent>().position = enemypos2;
                }
            }
            if (c->tag != "player" and c->tag != "collectible" and c->tag != "trigger" and Collision::coll(*c, enemy3.getComponent<ColliderComponent>()))
            {
                if (c->tag == "obstacle"){
                    enemy3.getComponent<TransformComponent>().position = enemypos3;
                }
            }
            if (c->tag != "player" and c->tag != "collectible" and c->tag != "trigger" and Collision::coll(*c, enemy4.getComponent<ColliderComponent>()))
            {
                if (c->tag == "obstacle"){
                    enemy4.getComponent<TransformComponent>().position = enemypos4;
                }
            }
        }

        if (count >= CollectibleCount){
            GameOver = true;
            render();
        }
    }
}

void Game::render()
{
    if (!GameOver){
        SDL_RenderClear(ren);
        for (auto& t : tiles)
        {
            t->draw();
        }
        for (auto& c : collectibles)
        {
            bool drawable = true;
            for (int j=0; j<collectibleStatus.size(); j++){
                if (c->getComponent<TransformComponent>().position.x == collectibleStatus[j].first.first and c->getComponent<TransformComponent>().position.y == collectibleStatus[j].first.second){
                    if (!collectibleStatus[j].second){
                        drawable = false;
                    }
                }
            }
            if (drawable){
                c->draw();
            }
        }
        player.draw();
        for (auto& e : enemies)
        {
            e->draw();
        }
        label.draw();
        label2.draw();
        SDL_RenderPresent(ren);
    }
    else {
        SDL_RenderClear(ren);
        for (auto& t : tiles)
        {
            t->draw();
        }
        gameOverLabel.draw();
        if (count >= CollectibleCount){
            P1wonLabel.draw();
        }
        else {
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
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(x, y, 40, 40, id);
    if (id == 1) {
        tile.addComponent<ColliderComponent>("obstacle");
    }
    tile.addGroup(groupMap);
}

void Game::AddCollectible(int id, int x, int y)
{
    CollectibleCount++;
    auto& collectibles(manager.addEntity());
    collectibles.addComponent<CollectibleComponent>(x, y, 40, 40, id);
    if (id == 2){
        collectibles.addComponent<ColliderComponent>("collectible");
    }
    else {
        collectibles.addComponent<ColliderComponent>("trigger");
    }
    collectibles.addGroup(groupCollectibles);
    collectibleStatus.push_back(std::make_pair(std::make_pair(x, y), true));
}

void Game::AddFont(std::string id, std::string path, int fontSize)
{
    fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* Game::GetFont(std::string id)
{
    return fonts[id];
}

pair<int, int> Game::getCoordinates(){
    int x = rand() % 30 + 1;
    int y = rand() % 22 + 1;
    while (maze::mazeM[y][x] != 2){
        x = rand() % 30 + 1;
        y = rand() % 22 + 1;
    }
    return make_pair(x*40, y*40);
}