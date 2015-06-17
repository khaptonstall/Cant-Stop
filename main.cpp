#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "board_view.h"
#include "dice_view.h"
#include "dice_controller.h"
#include "stop_view.h"
#include "stop_controller.h"
#include "options_view.h"
//#include "Player.cpp"
#include "GameState.cpp"
using namespace std;

int main(int, char**){
	//Seed random number 
	srand(time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		return 1;
	}

	// Create views
	board_view bv;
	dice_view dv;
	stop_view sv;
	options_view ov;

	int window_scale = 5;
	int board_width = bv.get_width();
	int dice_width = dv.get_width();
	int dice_height = dv.get_height();
	int board_height = bv.get_height();

	// Create controllers
	dice_controller dc(dv, board_width, 0, window_scale);
	stop_controller sc(sv, board_width, 0, window_scale);

	SDL_Window *win = SDL_CreateWindow("Can't Stop", 100, 100, (board_width + dice_width) / window_scale, board_height / window_scale, SDL_WINDOW_SHOWN);
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

	vector<int> empty_vector = vector<int>();

	{
		// Initial render (kinda a test)
		SDL_RenderClear(ren);

		SDL_Surface* board_surface = bv.get_surface(empty_vector, empty_vector, empty_vector, empty_vector, empty_vector);
		SDL_Texture* board_texture = SDL_CreateTextureFromSurface(ren, board_surface);
		SDL_Rect board_destination;
		board_destination.x = 0;
		board_destination.y = 0;
		SDL_QueryTexture(board_texture, NULL, NULL, &board_destination.w, &board_destination.h);
		board_destination.w /= window_scale;
		board_destination.h /= window_scale;

		SDL_Surface* stop_surface = sv.get_surface();
		SDL_Texture* stop_texture = SDL_CreateTextureFromSurface(ren, stop_surface);
		SDL_Rect stop_destination;
		stop_destination.x = board_width / window_scale;
		stop_destination.y = 0;
		SDL_QueryTexture(stop_texture, NULL, NULL, &stop_destination.w, &stop_destination.h);
		stop_destination.w /= window_scale;
		stop_destination.h /= window_scale;

		SDL_Surface* options_surface = ov.get_surface();
		SDL_Texture* options_texture = SDL_CreateTextureFromSurface(ren, options_surface);
		SDL_Rect options_destination;
		options_destination.x = board_width / window_scale;
		options_destination.y = dice_height / window_scale;
		SDL_QueryTexture(options_texture, NULL, NULL, &options_destination.w, &options_destination.h);
		options_destination.w /= window_scale;
		options_destination.h /= window_scale;

		SDL_RenderCopy(ren, board_texture, NULL, &board_destination);
		SDL_RenderCopy(ren, stop_texture, NULL, &stop_destination);
		SDL_RenderCopy(ren, options_texture, NULL, &options_destination);
		SDL_RenderPresent(ren);

		SDL_FreeSurface(board_surface);
		SDL_FreeSurface(stop_surface);
		SDL_FreeSurface(options_surface);

		SDL_DestroyTexture(board_texture);
		SDL_DestroyTexture(stop_texture);
		SDL_DestroyTexture(options_texture);
	}

	GameState cantStop;
	//Player player1, player2;
	cantStop.player1.turn = true;

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
		if (cantStop.player1.turn){
			bool goOn = true;
			int choice;
			cout << "Its player1 turn" << '\n';
			vector< pair<int,int> > options = cantStop.player1.filterDice(cantStop.rollDice());
			if(options.size() > 0){
				cantStop.player1.displayCombinations( options);
				cout << "Choose a pair : " << '\n';
				cin >> choice;
				cantStop.player1.chooseDice(options[choice - 1]); //Choose a pair
			}else{
				goOn = false;
				cantStop.player1.stateReference = cantStop.player1.state;
			}
			
			//Update board with player choice
			// Render 
			SDL_RenderClear(ren);

			SDL_Surface* board_surface = bv.get_surface(cantStop.player1.stateReference, cantStop.player2.stateReference, empty_vector, empty_vector, empty_vector);
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

			
			//Ask player to stop or continue
			if(goOn){
				int stopOrGo;
				cout << "Stop (0) or continue (1)? " << '\n';
				cin >> stopOrGo;
				if (stopOrGo == 0){
					//Switch turns
					cantStop.player1.state = cantStop.player1.stateReference;
					cantStop.player1.checkForWin();
					cantStop.checkForDeadCols();
					if(cantStop.player1.claimedCols.size() == 3){
						cout << "Player 1 wins!";
						break;
					}
					cantStop.player1.currentCols.clear();
					cantStop.player1.turn = false;
					cantStop.player2.turn = true;
				}
			}else{
				cantStop.player1.currentCols.clear();
				cantStop.player1.turn = false;
				cantStop.player2.turn = true;
			}

		}else if(cantStop.player2.turn){
			bool goOn = true;
			int choice;
			cout << "Its player2 turn" << '\n';
			vector< pair<int,int> > options = cantStop.player2.filterDice(cantStop.rollDice());

			if(options.size() > 0){
				cantStop.player2.displayCombinations( options);
				cout << "Choose a pair : " << '\n';
				cin >> choice;
				cantStop.player2.chooseDice(options[choice - 1]); //Choose a pair
			}else{
				goOn = false;
				cantStop.player2.stateReference = cantStop.player2.state;
			}
			

			//Update board with player choice
			// Render 
			SDL_RenderClear(ren);

			SDL_Surface* board_surface = bv.get_surface(cantStop.player1.stateReference, cantStop.player2.stateReference, empty_vector, empty_vector, empty_vector);
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

			//Ask player to stop or continue
			if(goOn){
				int stopOrGo;
				cout << "Stop (0) or continue (1)? " << '\n';
				cin >> stopOrGo;
				if (stopOrGo == 0){
					//Switch turns
					cantStop.player2.state = cantStop.player2.stateReference;
					cantStop.player2.checkForWin();
					cantStop.checkForDeadCols();
					if(cantStop.player2.claimedCols.size() == 3){
						cout << "Player 2 wins!";
						break;
					}
					cantStop.player2.currentCols.clear();
					cantStop.player2.turn = false;
					cantStop.player1.turn = true;
				}
			}else{
				cantStop.player2.currentCols.clear();
				cantStop.player2.turn = false;
				cantStop.player1.turn = true;
			}
			
		}
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}



