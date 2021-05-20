#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "Components.h"

class ColliderComponent : public Component
{
public:
    SDL_Rect collider;
    std::string tag;

    TransformComponent* transform;

    ColliderComponent(std::string t)
    {
        tag = t;
    }

    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();

        Game::colliders.push_back(this);
    }

    void update() override
    {
        if (tag == "collectible"){
            collider.x = static_cast<int> (transform->position.x) +10;
            collider.y = static_cast<int> (transform->position.y) +10;
            collider.w = 20;
            collider.h = 20;
        }
        else if (tag == "obstacle"){
            collider.x = static_cast<int> (transform->position.x) +5;
            collider.y = static_cast<int> (transform->position.y) +5;
            collider.w = 30;
            collider.h = 30;
        }
        else {
            collider.x = static_cast<int> (transform->position.x) +4;
            collider.y = static_cast<int> (transform->position.y) +4;
            collider.w = 32;
            collider.h = 32;
        }
    }

    void Destroy()
    {
        entity->destroy();
    }


};