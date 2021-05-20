#pragma once
#ifndef GAME_H
#define GAME_H

#include <iostream>
using namespace std;
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "SDL2/SDL_ttf.h"
#include "Vector2D.h"
#include <map>

class ColliderComponent;

class Game
{
public:
    Game();
    ~Game();

    void init(int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() {return isRunning;};

    static void AddTile(int id, int x, int y);
    static void AddFont(std::string id, std::string path, int fontSize);
    static TTF_Font* GetFont(std::string id);
    static void AddCollectible(int id, int x, int y);
    static SDL_Renderer *ren;
    static SDL_Event event;
    static std:: vector<ColliderComponent*> colliders;
    static std::map<std::string, TTF_Font*> fonts;
    static std:: vector<std::pair<std::pair<int, int>, bool>> collectibleStatus;
    static Vector2D initialPos;
    static pair<int, int> getCoordinates();
    static bool trigger[4];
    static bool triggerChange[4];
    static bool lastSeconds[4];
    static bool dead;
    static bool GameOver;
    static bool restart;

private:
    bool isRunning;
    int count = 0;
    int timeTicks = 0;
    SDL_Window *win;
    
};

#endif //GAME_H
