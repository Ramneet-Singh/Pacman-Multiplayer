#pragma once
#include "SDL2/SDL.h"

class ColliderComponent;

class Collision
{
public:
    static bool coll(const SDL_Rect& rec1, const SDL_Rect& rec2);
    static bool coll(const ColliderComponent& col1, const ColliderComponent& col2);
};