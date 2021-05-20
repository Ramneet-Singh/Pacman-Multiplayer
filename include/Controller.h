#pragma once

#include "game.h"
#include "entity.h"
#include "Components.h"

class Controller : public Component
{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;
    Game *game;

    Controller(Game *gamePtr)
    {
        game = gamePtr;
    }

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override
    {
        if (!Game::dead)
        {
            if (Game::event.type == SDL_KEYDOWN && Game::playerID == 1)
            {
                switch (Game::event.key.keysym.sym)
                {
                case SDLK_w:
                    {sprite->Play("moveU");
                    transform->velocity.y = -1;
                    transform->velocity.x = 0;
                    Game::userInput = UserInput::W;

                    message<GameMsg> msg;
                    msg.header.id = GameMsg::Game_UserInput;
                    msg << Game::userInput;
                    game->Send(msg);
                    std::cout << "[Player 1]: Sent user input: " << int(Game::userInput) << "\n";
                    break;}
                case SDLK_a:
                    {sprite->Play("moveL");
                    transform->velocity.x = -1;
                    transform->velocity.y = 0;
                    Game::userInput = UserInput::A;

                    message<GameMsg> msg;
                    msg.header.id = GameMsg::Game_UserInput;
                    msg << Game::userInput;
                    game->Send(msg);
                    std::cout << "[Player 1]: Sent user input: " << int(Game::userInput) << "\n";
                    break;}
                case SDLK_d:
                    {sprite->Play("moveR");
                    transform->velocity.x = 1;
                    transform->velocity.y = 0;
                    Game::userInput = UserInput::D;

                    message<GameMsg> msg;
                    msg.header.id = GameMsg::Game_UserInput;
                    msg << Game::userInput;
                    game->Send(msg);
                    std::cout << "[Player 1]: Sent user input: " << int(Game::userInput) << "\n";
                    break;}
                case SDLK_s:
                    {sprite->Play("moveD");
                    transform->velocity.y = 1;
                    transform->velocity.x = 0;
                    Game::userInput = UserInput::S;

                    message<GameMsg> msg;
                    msg.header.id = GameMsg::Game_UserInput;
                    msg << Game::userInput;
                    game->Send(msg);
                    std::cout << "[Player 1]: Sent user input: " << int(Game::userInput) << "\n";
                    break;}
                default:{
                    break;}
                }
            }

            else if (Game::event.type == SDL_KEYUP && Game::playerID == 1)
            {
                switch (Game::event.key.keysym.sym)
                {
                case SDLK_w:
                    {transform->velocity.y = 0;
                    sprite->Play("idleU");
                    Game::userInput = UserInput::W_UP;
                    message<GameMsg> msg;
                    msg.header.id = GameMsg::Game_UserInput;
                    msg << Game::userInput;
                    game->Send(msg);
                    std::cout << "[Player 1]: Sent user input: " << int(Game::userInput) << "\n";
                    break;}
                case SDLK_a:{
                    transform->velocity.x = 0;
                    sprite->Play("idleL");
                    Game::userInput = UserInput::A_UP;
                    message<GameMsg> msg;
                    msg.header.id = GameMsg::Game_UserInput;
                    msg << Game::userInput;
                    game->Send(msg);
                    std::cout << "[Player 1]: Sent user input: " << int(Game::userInput) << "\n";
                    break;}
                case SDLK_d:{
                    transform->velocity.x = 0;
                    sprite->Play("idleR");
                    Game::userInput = UserInput::D_UP;
                    message<GameMsg> msg;
                    msg.header.id = GameMsg::Game_UserInput;
                    msg << Game::userInput;
                    game->Send(msg);
                    std::cout << "[Player 1]: Sent user input: " << int(Game::userInput) << "\n";
                    break;}
                case SDLK_s:{
                    transform->velocity.y = 0;
                    sprite->Play("idleD");
                    Game::userInput = UserInput::S_UP;
                    message<GameMsg> msg;
                    msg.header.id = GameMsg::Game_UserInput;
                    msg << Game::userInput;
                    game->Send(msg);
                    std::cout << "[Player 1]: Sent user input: " << int(Game::userInput) << "\n";
                    break;}
                default:{
                    break;}
                }
            }

            else if (Game::playerID == 1)
            {
                Game::userInput = UserInput::DUMMY;
            }

            else
            {
                UserInput key = Game::otherUserInput;
                if (Game::playerID == 2)
                {
                    if (key == UserInput::W)
                    {
                        sprite->Play("moveU");
                        transform->velocity.y = -1;
                        transform->velocity.x = 0;
                    }
                    else if (key == UserInput::W_UP)
                    {
                        transform->velocity.y = 0;
                        sprite->Play("idleU");
                    }
                    else if (key == UserInput::A)
                    {
                        sprite->Play("moveL");
                        transform->velocity.x = -1;
                        transform->velocity.y = 0;
                    }
                    else if (key == UserInput::A_UP)
                    {
                        transform->velocity.x = 0;
                        sprite->Play("idleL");
                    }
                    else if (key == UserInput::S)
                    {
                        sprite->Play("moveD");
                        transform->velocity.y = 1;
                        transform->velocity.x = 0;
                    }
                    else if (key == UserInput::S_UP)
                    {
                        transform->velocity.y = 0;
                        sprite->Play("idleD");
                    }
                    else if (key == UserInput::D)
                    {
                        sprite->Play("moveR");
                        transform->velocity.x = 1;
                        transform->velocity.y = 0;
                    }
                    else if (key == UserInput::D_UP)
                    {
                        transform->velocity.x = 0;
                        sprite->Play("idleR");
                    }

                    Game::otherUserInput = UserInput::DUMMY;
                }
            }
        }
    }
};