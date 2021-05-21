#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "Game/Components/Components.h"

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
        else {
            collider.x = static_cast<int> (transform->position.x) +3;
            collider.y = static_cast<int> (transform->position.y) +3;
            collider.w = 34;
            collider.h = 34;
        }
    }

    void Destroy()
    {
        entity->destroy();
    }


};