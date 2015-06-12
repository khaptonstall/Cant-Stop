#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "board_view.h"

using namespace std;

int main(int, char**){
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
	if (win == nullptr){
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		SDL_DestroyWindow(win);
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	vector<int> blue_state = vector<int>(11, 0);
	blue_state[0] = 3;
	blue_state[1] = 5;
	blue_state[2] = 7;
	blue_state[3] = 9;
	blue_state[4] = 11;
	blue_state[5] = 13;
	blue_state[6] = 11;
	blue_state[7] = 9;
	blue_state[8] = 7;
	blue_state[9] = 5;
	blue_state[10] = 3;

	vector<int> red_state = vector<int>(11, 0);
	red_state[0] = 2;
	red_state[1] = 4;
	red_state[2] = 6;
	red_state[3] = 8;
	red_state[4] = 10;
	red_state[5] = 12;
	red_state[6] = 10;
	red_state[7] = 8;
	red_state[8] = 6;
	red_state[9] = 4;
	red_state[10] = 2;

	board_view b;

	vector<int> empty_vector = vector<int>();

	bool quit = false;
	SDL_Event e;

	while (!quit) {
		// Event handler
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}

		// Logic


		// Render
		SDL_RenderClear(ren);

		SDL_Surface* board_surface = b.get_surface(blue_state, red_state, empty_vector, empty_vector, empty_vector);
		SDL_Texture* board_texture;
		board_texture = SDL_CreateTextureFromSurface(ren, board_surface);

		SDL_Rect dst;
		dst.x = 0;
		dst.y = 0;
		SDL_QueryTexture(board_texture, NULL, NULL, &dst.w, &dst.h);
		dst.w /= 5;
		dst.h /= 5;
		SDL_RenderCopy(ren, board_texture, NULL, &dst);
		SDL_RenderPresent(ren);

		SDL_FreeSurface(board_surface);
		SDL_DestroyTexture(board_texture);

	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}

