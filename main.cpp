#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "board_view.h"
#include "Player.cpp"
using namespace std;

int main(int, char**){
	//Seed random number 
	srand(time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Can't Stop", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
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

	Player player1, player2;
	player1.changeTurns();

	

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


		// Render 
		/*
		SDL_RenderClear(ren);

		SDL_Surface* board_surface = b.get_surface(player1.stateReference, player2.stateReference, empty_vector, empty_vector, empty_vector);
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
*/

		// Logic
		if (player1.turn){
			bool goOn = true;
			int choice;
			cout << "Its player1 turn" << '\n';
			vector< pair<int,int> > options = player1.rollDice(); //Roll the dice
			if(options.size() > 0){
				player1.displayCombinations( options);
				cout << "Choose a pair : " << '\n';
				cin >> choice;
				player1.chooseDice(options[choice - 1]); //Choose a pair
			}else{
				goOn = false;
				player1.stateReference = player1.state;
			}
			
			//Update board with player choice
			// Render 
			SDL_RenderClear(ren);

			SDL_Surface* board_surface = b.get_surface(player1.stateReference, player2.stateReference, empty_vector, empty_vector, empty_vector);
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
					player1.state = player1.stateReference;
					player1.checkForWin();
					if(player1.claimedCols.size() == 3){
						cout << "Player 1 wins!";
						break;
					}
					player1.currentCols.clear();
					player2.changeTurns();
					player1.changeTurns();
				}
			}else{
				player1.currentCols.clear();
				player2.changeTurns();
				player1.changeTurns();
			}

		}else if(player2.turn){
			bool goOn = true;
			int choice;
			cout << "Its player2 turn" << '\n';
			vector< pair<int,int> > options = player2.rollDice(); //Roll the dice

			if(options.size() > 0){
				player2.displayCombinations( options);
				cout << "Choose a pair : " << '\n';
				cin >> choice;
				player2.chooseDice(options[choice - 1]); //Choose a pair
			}else{
				goOn = false;
				player2.stateReference = player2.state;
			}
			

			//Update board with player choice
			// Render 
			SDL_RenderClear(ren);

			SDL_Surface* board_surface = b.get_surface(player1.stateReference, player2.stateReference, empty_vector, empty_vector, empty_vector);
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
					player2.state = player2.stateReference;
					player2.checkForWin();
					if(player2.claimedCols.size() == 3){
						cout << "Player 2 wins!";
						break;
					}
					player2.currentCols.clear();
					player2.changeTurns();
					player1.changeTurns();
				}
			}else{
				player2.currentCols.clear();
				player2.changeTurns();
				player1.changeTurns();
			}
			
		}
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}



