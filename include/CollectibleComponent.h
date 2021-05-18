#pragma once

#include "entity.h"
#include "TransformComponent.h"
#include "SDL2/SDL.h"

class CollectibleComponent : public Component
{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;

    SDL_Rect collectRect;
    int collectID;
    char* path;

    CollectibleComponent() = default;

    CollectibleComponent(int x, int y, int w, int h, int id)
    {
        collectRect.x = x;
        collectRect.y = y;
        collectRect.w = w;
        collectRect.h = h;
        collectID = id;

        switch (collectID)
        {
        case 2:
            path = "Assets/collect2.png";
            break;
        case 3:
            path = "Assets/collect1.png";
            break;
        default:
            break;
        }
    }

    void init() override
    {
        entity->addComponent<TransformComponent>((float) collectRect.x, (float) collectRect.y, collectRect.w, collectRect.h, 1);
        transform = &entity->getComponent<TransformComponent>();

        entity->addComponent<SpriteComponent>(path);
        sprite = &entity->getComponent<SpriteComponent>();
    }
};