#pragma once

#include "Game/Components/entity.h"
#include "Game/Components/TransformComponent.h"
#include "SDL2/SDL.h"

class TileComponent : public Component
{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;

    SDL_Rect tileRect;
    int tileID;
    char* path;

    TileComponent() = default;

    TileComponent(int x, int y, int w, int h, int id)
    {
        tileRect.x = x;
        tileRect.y = y;
        tileRect.w = w;
        tileRect.h = h;
        tileID = id;

        switch (tileID)
        {
        case 2:
            path = "Assets/bg.png";
            break;
        case 3:
            path = "Assets/bg.png";
            break;
        case 1:
            path = "Assets/wall2.png";
            break;
        default:
            break;
        }
    }

    void init() override
    {
        entity->addComponent<TransformComponent>((float) tileRect.x, (float) tileRect.y, tileRect.w, tileRect.h, 1);
        transform = &entity->getComponent<TransformComponent>();

        entity->addComponent<SpriteComponent>(path);
        sprite = &entity->getComponent<SpriteComponent>();
    }
};