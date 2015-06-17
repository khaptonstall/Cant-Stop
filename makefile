all:
	g++ -std=c++11 -o cantstop main.cpp Player.cpp GameState.cpp Human_Player.cpp AI_Player.cpp -I/usr/local/include -L/usr/local/lib -lSDL2 -lSDL2_image 
