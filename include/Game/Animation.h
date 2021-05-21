#pragma once

struct Animation
{
    int indexY;
    int indexX;
    int frames;
    int speed;

    Animation() {}
    Animation(int y, int x, int f, int s)
    {
        indexY = y;
        indexX = x;
        frames = f;
        speed = s;
    }
};
