#pragma once
#include <string>

#include "TextureManager.h"


class Map{
public:
    Map();
    ~Map();
    static void LoadMap(std::string path, int sizeX, int sizeY);
private:
};