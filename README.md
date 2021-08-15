# Pacman-Multiplayer
A multiplayer version of the classic game Pacman, built using SDL2 for graphics and asyncio for networking. Please see our slides [here](https://docs.google.com/presentation/d/1NEsNa0QuZiD1fUpPnKywLjQBMWJC9w_j7ztHdNSI88s/edit?usp=sharing) for a description of how the game works.

#### Instructions to run
1. Type `make` in the terminal to compile the files and generate two executables namely `game` and `server`.
2. Run the server executable using command `./server` in the terminal.
3. Open a new terminal and run the game executable using `./game`, this is the terminal for player 1.
4. Repeat step 3 to get a player 2 terminal.
5. Once game is over, make sure you close the server and restart it (using `./server`) before running your game again.

#### Instructions to install asio
1. Download asio from the link https://think-async.com/Asio/ based on your OS. Extract it in a folder of your choice.
2. Change the path `~/software/asio/asio-1.18.1/include` in makefile according to your asio location. Make sure you enter the path correctly.

#### Instructions to install SDL 2 and other libraries
1. Use the command `sudo apt-get install libsdl2-dev` to install SDL 2.
2. In order to run the game successfully, the following extra libraries are needed: SDL_image, SDL_ttf and SDL_mixer.
3. To install SDL_image, use `sudo apt-get install libsdl2-image-dev`.
4. Install SDL_ttf using `sudo apt-get install libsdl2-ttf-dev`.
5. Use `sudo apt-get install libsdl2-mixer-dev` to install SDL_mixer.

*The instructions mentioned above for various installations were used on Ubuntu 20.04 and are meant only for such an OS.*

##### References
1. https://lazyfoo.net/tutorials/SDL/index.php
2. https://en.wikipedia.org/wiki/User:Dllu/Maze
3. https://github.com/DarkMuffinJoe/Pacman
4. https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T
