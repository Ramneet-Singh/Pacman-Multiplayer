#pragma once

#include "game.h"
#include "entity.h"
#include "Components.h"
#include <string>

class enemyController : public Component
{
private:
public:
    TransformComponent *transform;
    SpriteComponent *sprite;
    Game *game;
    int monst = 1;
    int num;

    enemyController(int number, Game *gamePtr)
    {
        num = number;
        game = gamePtr;
    }

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override
    {
        if (Game::trigger[num - 1])
        {
            if (Game::lastSeconds[num - 1])
            {
                sprite->Play("monL");
            }
            else
            {
                sprite->Play("monT");
            }
        }
        else if (Game::triggerChange[num - 1])
        {
            if (num == 1)
            {
                sprite->Play("monU1");
            }
            else if (num == 2)
            {
                sprite->Play("monU2");
            }
            else if (num == 3)
            {
                sprite->Play("monU3");
            }
            else if (num == 4)
            {
                sprite->Play("monU4");
            }
            Game::triggerChange[num - 1] = false;
        }
        if (Game::event.type == SDL_KEYDOWN && Game::playerID == 2)
        {
            if (Game::event.key.keysym.sym == SDLK_1)
            {
                monst = 1;
                Game::userInput = UserInput::ONE;

                message<GameMsg> msg;
                msg.header.id = GameMsg::Game_UserInput;
                msg << Game::userInput;
                game->Send(msg);
                std::cout << "[Player 2]: Sent user input: " << int(Game::userInput) << "\n";
            }
            else if (Game::event.key.keysym.sym == SDLK_2)
            {
                monst = 2;
                Game::userInput = UserInput::TWO;

                message<GameMsg> msg;
                msg.header.id = GameMsg::Game_UserInput;
                msg << Game::userInput;
                game->Send(msg);
                std::cout << "[Player 2]: Sent user input: " << int(Game::userInput) << "\n";
            }
            else if (Game::event.key.keysym.sym == SDLK_3)
            {
                monst = 3;
                Game::userInput = UserInput::THREE;

                message<GameMsg> msg;
                msg.header.id = GameMsg::Game_UserInput;
                msg << Game::userInput;
                game->Send(msg);
                std::cout << "[Player 2]: Sent user input: " << int(Game::userInput) << "\n";
            }
            else if (Game::event.key.keysym.sym == SDLK_4)
            {
                monst = 4;
                Game::userInput = UserInput::FOUR;

                message<GameMsg> msg;
                msg.header.id = GameMsg::Game_UserInput;
                msg << Game::userInput;
                game->Send(msg);
                std::cout << "[Player 2]: Sent user input: " << int(Game::userInput) << "\n";
            }
            else if (monst == num)
            {
                switch (Game::event.key.keysym.sym)
                {
                case SDLK_UP:
                {
                    if (!Game::trigger[num - 1])
                    {
                        if (monst == 1)
                        {
                            sprite->Play("monU1");
                        }
                        else if (monst == 2)
                        {
                            sprite->Play("monU2");
                        }
                        else if (monst == 3)
                        {
                            sprite->Play("monU3");
                        }
                        else if (monst == 4)
                        {
                            sprite->Play("monU4");
                        }
                    }
                    transform->velocity.y = -1;
                    Game::userInput = UserInput::UP;
                    Game::toUpdate = true;
                    transform->update();

                    message<GameMsg> msg;
                    msg.header.id = GameMsg::Game_UserInput;
                    msg << Game::userInput;
                    game->Send(msg);
                    std::cout << "[Player 2]: Sent user input: " << int(Game::userInput) << "\n";
                    break;
                }
                case SDLK_LEFT:
                {
                    if (!Game::trigger[num - 1])
                    {
                        if (monst == 1)
                        {
                            sprite->Play("monL1");
                        }
                        else if (monst == 2)
                        {
                            sprite->Play("monL2");
                        }
                        else if (monst == 3)
                        {
                            sprite->Play("monL3");
                        }
                        else if (monst == 4)
                        {
                            sprite->Play("monL4");
                        }
                    }
                    transform->velocity.x = -1;
                    Game::userInput = UserInput::LEFT;
                    Game::toUpdate = true;
                    transform->update();

                    message<GameMsg> msg;
                    msg.header.id = GameMsg::Game_UserInput;
                    msg << Game::userInput;
                    game->Send(msg);
                    std::cout << "[Player 2]: Sent user input: " << int(Game::userInput) << "\n";
                    break;
                }
                case SDLK_RIGHT:
                {
                    if (!Game::trigger[num - 1])
                    {
                        if (monst == 1)
                        {
                            sprite->Play("monR1");
                        }
                        else if (monst == 2)
                        {
                            sprite->Play("monR2");
                        }
                        else if (monst == 3)
                        {
                            sprite->Play("monR3");
                        }
                        else if (monst == 4)
                        {
                            sprite->Play("monR4");
                        }
                    }
                    transform->velocity.x = 1;
                    Game::userInput = UserInput::RIGHT;
                    Game::toUpdate = true;
                    transform->update();

                    message<GameMsg> msg;
                    msg.header.id = GameMsg::Game_UserInput;
                    msg << Game::userInput;
                    game->Send(msg);
                    std::cout << "[Player 2]: Sent user input: " << int(Game::userInput) << "\n";
                    break;
                }
                case SDLK_DOWN:
                {
                    if (!Game::trigger[num - 1])
                    {
                        if (monst == 1)
                        {
                            sprite->Play("monD1");
                        }
                        else if (monst == 2)
                        {
                            sprite->Play("monD2");
                        }
                        else if (monst == 3)
                        {
                            sprite->Play("monD3");
                        }
                        else if (monst == 4)
                        {
                            sprite->Play("monD4");
                        }
                    }
                    transform->velocity.y = 1;
                    Game::userInput = UserInput::DOWN;
                    Game::toUpdate = true;
                    transform->update();

                    message<GameMsg> msg;
                    msg.header.id = GameMsg::Game_UserInput;
                    msg << Game::userInput;
                    game->Send(msg);
                    std::cout << "[Player 2]: Sent user input: " << int(Game::userInput) << "\n";
                    break;
                }
                default:
                {
                    break;
                }
                }
            }
        }

        else if (Game::event.type == SDL_KEYUP && Game::playerID == 2)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_UP:
            {
                transform->velocity.y = 0;
                Game::userInput = UserInput::UP_UP;
                Game::toUpdate = true;
                transform->update();

                message<GameMsg> msg;
                msg.header.id = GameMsg::Game_UserInput;
                msg << Game::userInput;
                game->Send(msg);
                std::cout << "[Player 2]: Sent user input: " << int(Game::userInput) << "\n";
                break;
            }
            case SDLK_LEFT:
            {
                transform->velocity.x = 0;
                Game::userInput = UserInput::LEFT_UP;
                Game::toUpdate = true;
                transform->update();

                message<GameMsg> msg;
                msg.header.id = GameMsg::Game_UserInput;
                msg << Game::userInput;
                game->Send(msg);
                std::cout << "[Player 2]: Sent user input: " << int(Game::userInput) << "\n";
                break;
            }
            case SDLK_RIGHT:
            {
                transform->velocity.x = 0;
                Game::userInput = UserInput::RIGHT_UP;
                Game::toUpdate = true;
                transform->update();

                message<GameMsg> msg;
                msg.header.id = GameMsg::Game_UserInput;
                msg << Game::userInput;
                game->Send(msg);
                std::cout << "[Player 2]: Sent user input: " << int(Game::userInput) << "\n";
                break;
            }
            case SDLK_DOWN:
            {
                transform->velocity.y = 0;
                Game::userInput = UserInput::DOWN_UP;
                Game::toUpdate = true;
                transform->update();

                message<GameMsg> msg;
                msg.header.id = GameMsg::Game_UserInput;
                msg << Game::userInput;
                game->Send(msg);
                std::cout << "[Player 2]: Sent user input: " << int(Game::userInput) << "\n";
                break;
            }
            default:
            {
                break;
            }
            }
        }

        else if (Game::playerID == 2)
        {
            Game::userInput = UserInput::DUMMY;
        }
        else if (Game::playerID == 1)
        {
            UserInput key = Game::otherUserInput;

            if (key == UserInput::ONE)
            {
                monst = 1;
            }
            else if (key == UserInput::TWO)
            {
                std::cout << "Monst changed to 2\n";
                monst = 2;
            }
            else if (key == UserInput::THREE)
            {
                monst = 3;
            }
            else if (key == UserInput::FOUR)
            {
                monst = 4;
            }
            else if (key == UserInput::UP)
            {
                if (monst == num)
                {
                    if (!Game::trigger[num - 1])
                    {

                        if (monst == 1)
                        {
                            sprite->Play("monU1");
                        }
                        else if (monst == 2)
                        {
                            sprite->Play("monU2");
                        }
                        else if (monst == 3)
                        {
                            sprite->Play("monU3");
                        }
                        else if (monst == 4)
                        {
                            sprite->Play("monU4");
                        }
                    }

                    transform->velocity.y = -1;
                    Game::toUpdate = true;
                    transform->update();
                }
            }
            else if (key == UserInput::LEFT)
            {
                if (monst == num)
                {
                    if (!Game::trigger[num - 1])
                    {

                        if (monst == 1)
                        {
                            sprite->Play("monL1");
                        }
                        else if (monst == 2)
                        {
                            sprite->Play("monL2");
                        }
                        else if (monst == 3)
                        {
                            sprite->Play("monL3");
                        }
                        else if (monst == 4)
                        {
                            sprite->Play("monL4");
                        }
                    }

                    transform->velocity.x = -1;
                    Game::toUpdate = true;
                    transform->update();
                }
            }
            else if (key == UserInput::DOWN)
            {
                if (monst == num)
                {
                    if (!Game::trigger[num - 1])
                    {

                        if (monst == 1)
                        {
                            sprite->Play("monD1");
                        }
                        else if (monst == 2)
                        {
                            sprite->Play("monD2");
                        }
                        else if (monst == 3)
                        {
                            sprite->Play("monD3");
                        }
                        else if (monst == 4)
                        {
                            sprite->Play("monD4");
                        }
                    }
                    transform->velocity.y = 1;
                    Game::toUpdate = true;
                    transform->update();
                }
            }
            else if (key == UserInput::RIGHT)
            {
                if (monst == num)
                {
                    if (!Game::trigger[num - 1])
                    {

                        if (monst == 1)
                        {
                            sprite->Play("monR1");
                        }
                        else if (monst == 2)
                        {
                            sprite->Play("monR2");
                        }
                        else if (monst == 3)
                        {
                            sprite->Play("monR3");
                        }
                        else if (monst == 4)
                        {
                            sprite->Play("monR4");
                        }
                    }

                    transform->velocity.x = 1;
                    Game::toUpdate = true;
                    transform->update();
                }
            }
            else if (key == UserInput::UP_UP)
            {
                transform->velocity.y = 0;
                Game::toUpdate = true;
                transform->update();
            }
            else if (key == UserInput::LEFT_UP)
            {
                transform->velocity.x = 0;
                Game::toUpdate = true;
                transform->update();
            }
            else if (key == UserInput::DOWN_UP)
            {
                transform->velocity.y = 0;
                Game::toUpdate = true;
                transform->update();
            }
            else if (key == UserInput::RIGHT_UP)
            {
                transform->velocity.x = 0;
                Game::toUpdate = true;
                transform->update();
            }
            if (num == 4)
            {

                Game::otherUserInput = UserInput::DUMMY;
            }
        }
    }
};