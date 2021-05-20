#pragma once

#include "game.h"
#include "entity.h"
#include "Components.h"

class Controller : public Component
{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override
    {
        if (!Game::dead){
            if (Game::event.type == SDL_KEYDOWN)
            {
                switch (Game::event.key.keysym.sym)
                {
                case SDLK_w:
                    sprite->Play("moveU");
                    transform->velocity.y = -1;
                    transform->velocity.x = 0;
                    break;
                case SDLK_a:
                    sprite->Play("moveL");
                    transform->velocity.x = -1;
                    transform->velocity.y = 0;
                    break;
                case SDLK_d:
                    sprite->Play("moveR");
                    transform->velocity.x = 1;
                    transform->velocity.y = 0;
                    break;
                case SDLK_s:
                    sprite->Play("moveD");
                    transform->velocity.y = 1;
                    transform->velocity.x = 0;
                    break;
                default:
                    break;
                }
            }

            else if (Game::event.type == SDL_KEYUP)
            {
                switch (Game::event.key.keysym.sym)
                {
                case SDLK_w:
                    transform->velocity.y = 0;
                    sprite->Play("idleU");
                    break;
                case SDLK_a:
                    transform->velocity.x = 0;
                    sprite->Play("idleL");
                    break;
                case SDLK_d:
                    transform->velocity.x = 0;
                    sprite->Play("idleR");
                    break;
                case SDLK_s:
                    transform->velocity.y = 0;
                    sprite->Play("idleD");
                    break;
                default:
                    break;
                }
            }
        }
    }
};