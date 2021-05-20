#pragma once
#include "Components.h"
#include "Vector2D.h"
#include "game.h"
#include "Collision.h"
#include <string>

class TransformComponent : public Component
{

public:
    Vector2D position;
    Vector2D velocity;
    int height = 40;
    int width = 40;
    int scale = 1;
    int speed = 2;
    std::string tagged = "";

    TransformComponent()
    {
        position.Zero();
    }

    TransformComponent(float x, float y)
    {
        position.x = x;
        position.y = y;
    }

    TransformComponent(float x, float y, int spd)
    {
        position.x = x;
        position.y = y;
        speed = spd;
    }

    TransformComponent(int sc)
    {
        position.Zero();
        scale = sc;
    }

    TransformComponent(float x, float y, int h, int w, int sc)
    {
        position.x = x;
        position.y = y;
        height = h;
        width = w;
        scale = sc;
    }

    TransformComponent(float x, float y, std::string tag1)
    {
        position.x = x;
        position.y = y;
        tagged = tag1;
    }

    void init() override
    {
        velocity.Zero();
    }

    void update() override
    {
        if (Game::toUpdate)
        {
            Vector2D pos = position;
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
            if (tagged != "" and Game::checkColl(tagged)){
                cout << "collision\n";
                if (velocity.x == 1) {position.x = pos.x-9; velocity.x = 0;}
                if (velocity.x == -1) {position.x = pos.x+9; velocity.x = 0;}
                if (velocity.y == 1) {position.y = pos.y-9; velocity.y = 0;}
                if (velocity.y == -1) {position.y = pos.y+9; velocity.y = 0;}
            }
        }
    }
};