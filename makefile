all:
	g++ -std=c++11 -o cantstop main.cpp -I/usr/local/include -L/usr/local/lib -lSDL2 -lSDL2_image 
