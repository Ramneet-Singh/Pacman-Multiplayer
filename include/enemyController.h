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
    int monst = 1;
    int num;

    enemyController(int number)
    {
        num = number;
    }

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override
    {
        if (Game::trigger[num-1]){
            if (Game::lastSeconds[num-1]){
                sprite->Play("monL");
            }
            else {
                sprite->Play("monT");
            }
        }
        else if (Game::triggerChange[num-1]){
            if (num == 1){
                sprite->Play("monU1");
            }
            else if (num == 2){
                sprite->Play("monU2");
            }
            else if (num == 3){
                sprite->Play("monU3");
            }
            else if (num == 4){
                sprite->Play("monU4");
            }
            Game::triggerChange[num-1] = false;
        }
        if (Game::event.type == SDL_KEYDOWN)
        {
            if (Game::event.key.keysym.sym == SDLK_1){
                monst = 1;
            }
            else if (Game::event.key.keysym.sym == SDLK_2){
                monst = 2;
            }
            else if (Game::event.key.keysym.sym == SDLK_3){
                monst = 3;
            }
            else if (Game::event.key.keysym.sym == SDLK_4){
                monst = 4;
            }
            if (monst == num)
            {
                switch (Game::event.key.keysym.sym)
                {
                case SDLK_UP:
                    if (!Game::trigger[num-1]){
                        if (monst == 1){
                            sprite->Play("monU1");
                        }
                        else if (monst == 2){
                            sprite->Play("monU2");
                        }
                        else if (monst == 3){
                            sprite->Play("monU3");
                        }
                        else if (monst == 4){
                            sprite->Play("monU4");
                        }
                    }
                    transform->velocity.y = -1;
                    break;
                case SDLK_LEFT:
                    if (!Game::trigger[num-1]){
                        if (monst == 1){
                            sprite->Play("monL1");
                        }
                        else if (monst == 2){
                            sprite->Play("monL2");
                        }
                        else if (monst == 3){
                            sprite->Play("monL3");
                        }
                        else if (monst == 4){
                            sprite->Play("monL4");
                        }
                    }
                    transform->velocity.x = -1;
                    break;
                case SDLK_RIGHT:
                    if (!Game::trigger[num-1]){
                        if (monst == 1){
                            sprite->Play("monR1");
                        }
                        else if (monst == 2){
                            sprite->Play("monR2");
                        }
                        else if (monst == 3){
                            sprite->Play("monR3");
                        }
                        else if (monst == 4){
                            sprite->Play("monR4");
                        }
                    }
                    transform->velocity.x = 1;
                    break;
                case SDLK_DOWN:
                    if (!Game::trigger[num-1]){
                        if (monst == 1){
                            sprite->Play("monD1");
                        }
                        else if (monst == 2){
                            sprite->Play("monD2");
                        }
                        else if (monst == 3){
                            sprite->Play("monD3");
                        }
                        else if (monst == 4){
                            sprite->Play("monD4");
                        }
                    }
                    transform->velocity.y = 1;
                    break;
                default:
                    break;
                }
            }
        }

        if (Game::event.type == SDL_KEYUP)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_UP:
                transform->velocity.y = 0;
                break;
            case SDLK_LEFT:
                transform->velocity.x = 0;
                break;
            case SDLK_RIGHT:
                transform->velocity.x = 0;
                break;
            case SDLK_DOWN:
                transform->velocity.y = 0;
                break;
            default:
                break;
            }
        }
    }
};