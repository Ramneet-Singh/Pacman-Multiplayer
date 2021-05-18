#include "include/Map.h"
#include "include/game.h"
#include <fstream>

Map::Map()
{

}

Map::~Map()
{
}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
    char tile;
    std::fstream mapFile;
    mapFile.open(path);
    for (int y = 0; y < sizeY; y++){
        for (int x=0; x < sizeX; x++){
            mapFile.get(tile);
            Game::AddTile(atoi(&tile), x*40, y*40);
            if (tile == '2' or tile == '3'){
                Game::AddCollectible(atoi(&tile), x*40, y*40);
            }
            mapFile.ignore();
        }
    }

    mapFile.close();
}
