#include "Game/Utils/Map.h"
#include "Game/game.h"
#include "Game/Utils/maze.h"
#include <fstream>

Map::Map()
{
}

Map::~Map()
{
}

void Map::LoadMap(int sizeX, int sizeY)
{
    int tile;
    for (int y = 0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            tile = maze::mazeM[y][x];
            Game::AddTile(tile, x * 40, y * 40);
            if (tile == 2 or tile == 3)
            {
                Game::AddCollectible(tile, x * 40, y * 40);
            }
        }
    }
}
