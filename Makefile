all:
	g++ --std=c++17 Map.cpp Vector2D.cpp maze.cpp game.cpp main.cpp Collision.cpp TextureManager.cpp entity.cpp -I . -I ~/Downloads/asio-1.18.1/include -pthread -w -g -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o TestClient
	g++ --std=c++17 server.cpp -I . -I /home/rakshita/Downloads/asio-1.18.1/include -pthread -w -g -o TestServer
clean:
	rm TestClient TestServer
