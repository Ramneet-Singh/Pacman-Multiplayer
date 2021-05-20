all:
	g++ --std=c++17 Map.cpp Vector2D.cpp game.cpp maicpp Collision.cpp TextureManager.cpp entity.cpp -I . -I ~/software/asio/asio-1.18.1/include -pthread -w -g -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o TestClient
	g++ --std=c++17 server.cpp -I . -I /home/ramneet/software/asio/asio-1.18.1/include -pthread -w -g -o TestServer
clean:
	rm TestClient TestServer