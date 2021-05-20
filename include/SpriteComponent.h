#pragma once
#include "Components.h"
#include "SDL2/SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>
#include "game.h"

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;

    bool animated = false;
    int frames = 0;
    int speed = 100;
    // bool pause = false;

public:
    int animIndex = 0;
    int offsetX = 0;

    std::map<const char*, Animation> animations;

    SpriteComponent() = default;

    SpriteComponent(const char* path)
    {
        setTex(path);
    }

    SpriteComponent(const char* path, bool isAnimated, int monstNum)
    {
        animated = isAnimated;

        if (path == "Assets/xx.png")
        {
            Animation idleR = Animation(0, 1, 1, 100);
            Animation moveR = Animation(0, 0, 3, 100);
            Animation idleL = Animation(1, 1, 1, 100);
            Animation moveL = Animation(1, 0, 3, 100);
            Animation idleU = Animation(2, 1, 1, 100);
            Animation moveU = Animation(2, 0, 3, 100);
            Animation idleD = Animation(3, 1, 1, 100);
            Animation moveD = Animation(3, 0, 3, 100);
            Animation die = Animation(4, 2, 12, 140);

            animations.emplace("idleR", idleR);
            animations.emplace("idleL", idleL);
            animations.emplace("idleU", idleU);
            animations.emplace("idleD", idleD);
            animations.emplace("moveR", moveR);
            animations.emplace("moveL", moveL);
            animations.emplace("moveU", moveU);
            animations.emplace("moveD", moveD);
            animations.emplace("die", die);

            Play("idleR");
            setTex(path);
        }
        else if (monstNum == 1)
        {
            Animation monU1 = Animation(0, 0, 2, 200);
            Animation monD1 = Animation(0, 2, 2, 200);
            Animation monL1 = Animation(0, 4, 2, 200);
            Animation monR1 = Animation(0, 6, 2, 200);
            Animation monT = Animation(4, 0, 2, 200);
            Animation monL = Animation(4, 1, 2, 200);
            animations.emplace("monU1", monU1);
            animations.emplace("monD1", monD1);
            animations.emplace("monL1", monL1);
            animations.emplace("monR1", monR1);
            animations.emplace("monT", monT);
            animations.emplace("monL", monL);
            Play("monD1");
            setTex(path);
        }
        else if (monstNum == 2)
        {
            Animation monU2 = Animation(1, 0, 2, 200);
            Animation monD2 = Animation(1, 2, 2, 200);
            Animation monL2 = Animation(1, 4, 2, 200);
            Animation monR2 = Animation(1, 6, 2, 200);
            Animation monT = Animation(4, 0, 2, 200);
            Animation monL = Animation(4, 1, 2, 200);
            animations.emplace("monU2", monU2);
            animations.emplace("monD2", monD2);
            animations.emplace("monL2", monL2);
            animations.emplace("monR2", monR2);
            animations.emplace("monT", monT);
            animations.emplace("monL", monL);
            Play("monD2");
            setTex(path);
        }
        else if (monstNum == 3)
        {
            Animation monU3 = Animation(2, 0, 2, 200);
            Animation monD3 = Animation(2, 2, 2, 200);
            Animation monL3 = Animation(2, 4, 2, 200);
            Animation monR3 = Animation(2, 6, 2, 200);
            Animation monT = Animation(4, 0, 2, 200);
            Animation monL = Animation(4, 1, 2, 200);
            animations.emplace("monU3", monU3);
            animations.emplace("monD3", monD3);
            animations.emplace("monL3", monL3);
            animations.emplace("monR3", monR3);
            animations.emplace("monT", monT);
            animations.emplace("monL", monL);
            Play("monD3");
            setTex(path);
        }
        else if (monstNum == 4)
        {
            Animation monU4 = Animation(3, 0, 2, 200);
            Animation monD4 = Animation(3, 2, 2, 200);
            Animation monL4 = Animation(3, 4, 2, 200);
            Animation monR4 = Animation(3, 6, 2, 200);
            Animation monT = Animation(4, 0, 2, 200);
            Animation monL = Animation(4, 1, 2, 200);
            animations.emplace("monU4", monU4);
            animations.emplace("monD4", monD4);
            animations.emplace("monL4", monL4);
            animations.emplace("monR4", monR4);
            animations.emplace("monT", monT);
            animations.emplace("monL", monL);
            Play("monD4");
            setTex(path);
        }
    }

    ~SpriteComponent()
    {
        SDL_DestroyTexture(texture);
    }

    void setTex (const char* path)
    {
        texture = TextureManager::LoadTexture(path);
    }

    void init() override
    {

        transform = &entity -> getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
    }

    void update() override
    {
        if (srcRect.x == 520 and srcRect.y == 160){
            
            Play("idleR");
            srcRect.x = srcRect.w * (offsetX + static_cast<int>((SDL_GetTicks() / speed) % frames));
            srcRect.y = animIndex * transform->height;
            entity->getComponent<TransformComponent>().position = Game::initialPos;
            Game::dead = false;
        }
        else {
            if (animated)
            {
                srcRect.x = srcRect.w * (offsetX + static_cast<int>((SDL_GetTicks() / speed) % frames));
            }

            srcRect.y = animIndex * transform->height;

            destRect.x = static_cast<int> (transform->position.x);
            destRect.y = static_cast<int> (transform->position.y);
            destRect.w = transform->width * transform->scale;
            destRect.h = transform->height * transform->scale;
        }
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect);
    }

    void Play(const char* name)
    {
        frames = animations[name].frames;
        animIndex = animations[name].indexY;
        speed = animations[name].speed;
        offsetX = animations[name].indexX;
    }
};