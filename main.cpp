#include "include/game.h"
#include "include/maze.h"
#include <fstream>

Game *game = nullptr;
int main()
{
    srand((unsigned int)time(NULL)); //seed random number generator with system time
	maze::generateMaze();        //generate the maze
    int countTriggers = 0;
    while (countTriggers < 8){
        int x = rand() % 30 + 1;
        int y = rand() % 22 + 1;
        if (maze::mazeM[y][x] != 1){
            maze::mazeM[y][x] = 3;
            countTriggers++;
        }
    }
    ofstream file;
    file.open("maze.txt");
    for (int y=0; y<23; y++){
        for (int x=0; x<31; x++){
            file << maze::mazeM[y][x];
            if (x != 30){
                file << ",";
            }
        }
        file << "\n";
    }
    file.close();
    const int FPS = 60;
    const int frameDelay = 1000/FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();
    game->init(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1240, 920, false);
    while (game->running())
    {
        frameStart = SDL_GetTicks();
        game->handleEvents();
        game->update();
        game->render();
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }
    }
    game->clean();
    return 0;
}