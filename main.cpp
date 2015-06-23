#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "board_view.h"
#include "dice_view.h"
#include "dice_controller.h"
#include "stop_view.h"
#include "stop_controller.h"
#include "options_view.h"

#include "GameState.h"

using namespace std;

int main(int, char**){
	//Seed random number 
	srand(time(NULL));

	dice_probability test;
	cout << test.get_probability(0,0,7) << endl;

	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		return 1;
	}

	if (TTF_Init() != 0) {
		cout << "TTF_Init: " << TTF_GetError() << endl;
    	return 1;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

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

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (ren == nullptr){
		SDL_DestroyWindow(win);
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	vector<int> empty_vector = vector<int>();

	GameState cantStop;
	//Player player1, player2;
	cantStop.player1.turn = true;
	Player* player = &cantStop.player1;

	bool quit = false;
	bool dice_active = false;
	bool stop_active = false;
	int dice_pair = 0;
	int stop_continue = 0;
	SDL_Event e;
	vector<int> dice_options;
	//vector<int> dice_options = player->rollDice(true);


	while (!quit) {
		// Event handler
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
				if (!stop_active) {
					dice_pair = dc.input(e.motion.x, e.motion.y);
				}
				else {
					stop_continue = sc.input(e.motion.x, e.motion.y);
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
				stop_continue = 0;
				dice_pair = 0;
			}
		}

		// Logic
		// If new dice need to be rolled
		if (!dice_active) {
			dice_active = true;
			dice_options = cantStop.rollDice(true);
		}

		// Check that there is a valid move
		vector<pair<int, int> > rolled_pairs(6);
		rolled_pairs[0] = pair<int, int>(dice_options[0] + dice_options[1], dice_options[2] + dice_options[3]);
		rolled_pairs[1] = pair<int, int>(dice_options[2] + dice_options[3], dice_options[0] + dice_options[1]);
		rolled_pairs[2] = pair<int, int>(dice_options[0] + dice_options[2], dice_options[1] + dice_options[3]);
		rolled_pairs[3] = pair<int, int>(dice_options[1] + dice_options[3], dice_options[0] + dice_options[2]);
		rolled_pairs[4] = pair<int, int>(dice_options[0] + dice_options[3], dice_options[2] + dice_options[1]);
		rolled_pairs[5] = pair<int, int>(dice_options[2] + dice_options[1], dice_options[0] + dice_options[3]);
		bool valid_pairs = false;
		vector<bool> bad_dice = vector<bool>(6, false);
		for (int i = 0; i < 6; i++) {
			if (cantStop.validatePair(rolled_pairs[i].first, player)) {
				valid_pairs = true;
			}
			else {
				bad_dice[i] = true;
			}
		}

		// for (pair<int, int> p : rolled_pairs) {
		// 	if (cantStop.validatePair(p.first, player)) {
		// 		valid_pairs = true;
		// 		break;
		// 	}
		// 	else {

		// 	}
		// }

		// Wait for player to choose a dice pair
		if (!stop_active && valid_pairs) {
			pair<int, int> result = player->select_dice(&cantStop, rolled_pairs, player, dice_pair);
			if (result != pair<int, int>(-1, -1)) {
				player->chooseDice(result);
				stop_active = true;
			}
		}
		// Dice has been selected, now choose stop / continue
		else if (stop_active) {
			// // continue
			// if (stop_continue == 1) {
			// 	dice_active = false;
			// 	stop_active = false;
			// }
			// // stop
			// else if (stop_continue == 2) {
			// 	dice_active = false;
			// 	stop_active = false;
			// 	player->state = player->stateReference;

			// 	player->checkForWin();
			// 	if (player->claimedCols.size() == 3) { cout << "Win!" << endl; break; }
			// 	player->currentCols.clear();
			// 	if (player == &cantStop.player1) player = &cantStop.player2;
			// 	else if (player == &cantStop.player2) player = &cantStop.player1;
			// }
			int result = player->select_decision(&cantStop, stop_continue);
			if (result == 1) {
				dice_active = false;
				stop_active = false;
			}
			else if (result == 2) {
				dice_active = false;
				stop_active = false;
				player->state = player->stateReference;
				player->checkForWin();
				cantStop.checkForDeadCols();
				if (player->claimedCols.size() == 3) { cout << player->name << " wins!" << endl; break; }
				player->currentCols.clear();
				if (player == &cantStop.player1) player = &cantStop.player2;
				else if (player == &cantStop.player2) player = &cantStop.player1;
			}
		}
		// No dice choice is valid, revert
		else {
			dice_active = false;
			player->stateReference = player->state;
			player->currentCols.clear();
			if (player == &cantStop.player1) player = &cantStop.player2;
			else if (player == &cantStop.player2) player = &cantStop.player1;
		}

		// // Logic
		// if (cantStop.player1.turn){
		// 	bool goOn = true;
		// 	int choice;
		// 	cout << "Its player1 turn" << '\n';
		// 	vector< pair<int,int> > options = cantStop.player1.filterDice(cantStop.rollDice());
		// 	if(options.size() > 0){
		// 		cantStop.player1.displayCombinations( options);
		// 		cout << "Choose a pair : " << '\n';
		// 		cin >> choice;
		// 		cantStop.player1.chooseDice(options[choice - 1]); //Choose a pair
		// 	}else{
		// 		goOn = false;
		// 		cantStop.player1.stateReference = cantStop.player1.state;
		// 	}
			
		// 	//Update board with player choice
		// 	// Render 
		// 	SDL_RenderClear(ren);

		// 	SDL_Surface* board_surface = bv.get_surface(cantStop.player1.stateReference, cantStop.player2.stateReference, empty_vector, empty_vector, empty_vector);
		// 	SDL_Texture* board_texture;
		// 	board_texture = SDL_CreateTextureFromSurface(ren, board_surface);

		// 	SDL_Rect dst;
		// 	dst.x = 0;
		// 	dst.y = 0;
		// 	SDL_QueryTexture(board_texture, NULL, NULL, &dst.w, &dst.h);
		// 	dst.w /= 5;
		// 	dst.h /= 5;
		// 	SDL_RenderCopy(ren, board_texture, NULL, &dst);
		// 	SDL_RenderPresent(ren);

	// 	// Logic
	// 	if (player1.turn){
	// 		bool goOn = true;
	// 		int choice;
	// 		cout << "Its player1 turn" << '\n';
	// 		vector< pair<int,int> > options = player1.rollDice(); //Roll the dice
	// 		if(options.size() > 0){
	// 			player1.displayCombinations( options);
	// 			cout << "Choose a pair : " << '\n';
	// 			cin >> choice;
	// 			player1.chooseDice(options[choice - 1]); //Choose a pair
	// 		}else{
	// 			goOn = false;
	// 			player1.stateReference = player1.state;
	// 		}
			
	// 		//Update board with player choice
	// 		// Render 
	// 		SDL_RenderClear(ren);

	// 		SDL_Surface* board_surface = bv.get_surface(player1.stateReference, player2.stateReference, empty_vector, empty_vector, empty_vector);
	// 		SDL_Texture* board_texture;
	// 		board_texture = SDL_CreateTextureFromSurface(ren, board_surface);

	// 		SDL_Rect dst;
	// 		dst.x = 0;
	// 		dst.y = 0;
	// 		SDL_QueryTexture(board_texture, NULL, NULL, &dst.w, &dst.h);
	// 		dst.w /= 5;
	// 		dst.h /= 5;
	// 		SDL_RenderCopy(ren, board_texture, NULL, &dst);
	// 		SDL_RenderPresent(ren);

	// 		SDL_FreeSurface(board_surface);
	// 		SDL_DestroyTexture(board_texture);

			
	// 		//Ask player to stop or continue
	// 		if(goOn){
	// 			int stopOrGo;
	// 			cout << "Stop (0) or continue (1)? " << '\n';
	// 			cin >> stopOrGo;
	// 			if (stopOrGo == 0){
	// 				//Switch turns
	// 				player1.state = player1.stateReference;
	// 				player1.checkForWin();
	// 				if(player1.claimedCols.size() == 3){
	// 					cout << "Player 1 wins!";
	// 					break;
	// 				}
	// 				player1.currentCols.clear();
	// 				player2.changeTurns();
	// 				player1.changeTurns();
	// 			}
	// 		}else{
	// 			player1.currentCols.clear();
	// 			player2.changeTurns();
	// 			player1.changeTurns();
	// 		}

	// 	}else if(player2.turn){
	// 		bool goOn = true;
	// 		int choice;
	// 		cout << "Its player2 turn" << '\n';
	// 		vector< pair<int,int> > options = player2.rollDice(); //Roll the dice

	// 		if(options.size() > 0){
	// 			player2.displayCombinations( options);
	// 			cout << "Choose a pair : " << '\n';
	// 			cin >> choice;
	// 			player2.chooseDice(options[choice - 1]); //Choose a pair
	// 		}else{
	// 			goOn = false;
	// 			player2.stateReference = player2.state;
	// 		}
			

	// 		//Update board with player choice
	// 		// Render 
	// 		SDL_RenderClear(ren);

	// 		SDL_Surface* board_surface = bv.get_surface(player1.stateReference, player2.stateReference, empty_vector, empty_vector, empty_vector);
	// 		SDL_Texture* board_texture;
	// 		board_texture = SDL_CreateTextureFromSurface(ren, board_surface);

	// 		SDL_Rect dst;
	// 		dst.x = 0;
	// 		dst.y = 0;
	// 		SDL_QueryTexture(board_texture, NULL, NULL, &dst.w, &dst.h);
	// 		dst.w /= 5;
	// 		dst.h /= 5;
	// 		SDL_RenderCopy(ren, board_texture, NULL, &dst);
	// 		SDL_RenderPresent(ren);

	// 		SDL_FreeSurface(board_surface);
	// 		SDL_DestroyTexture(board_texture);

	// 		//Ask player to stop or continue
	// 		if(goOn){
	// 			int stopOrGo;
	// 			cout << "Stop (0) or continue (1)? " << '\n';
	// 			cin >> stopOrGo;
	// 			if (stopOrGo == 0){
	// 				//Switch turns
	// 				player2.state = player2.stateReference;
	// 				player2.checkForWin();
	// 				if(player2.claimedCols.size() == 3){
	// 					cout << "Player 2 wins!";
	// 					break;
	// 				}
	// 				player2.currentCols.clear();
	// 				player2.changeTurns();
	// 				player1.changeTurns();
	// 			}
	// 		}else{
	// 			player2.currentCols.clear();
	// 			player2.changeTurns();
	// 			player1.changeTurns();
	// 		}
		// 	//Ask player to stop or continue
		// 	if(goOn){
		// 		int stopOrGo;
		// 		cout << "Stop (0) or continue (1)? " << '\n';
		// 		cin >> stopOrGo;
		// 		if (stopOrGo == 0){
		// 			//Switch turns
		// 			cantStop.player1.state = cantStop.player1.stateReference;
		// 			cantStop.player1.checkForWin();
		// 			cantStop.checkForDeadCols();
		// 			if(cantStop.player1.claimedCols.size() == 3){
		// 				cout << "Player 1 wins!";
		// 				break;
		// 			}
		// 			cantStop.player1.currentCols.clear();
		// 			cantStop.player1.turn = false;
		// 			cantStop.player2.turn = true;
		// 		}
		// 	}else{
		// 		cantStop.player1.currentCols.clear();
		// 		cantStop.player1.turn = false;
		// 		cantStop.player2.turn = true;
		// 	}

		// }else if(cantStop.player2.turn){
		// 	bool goOn = true;
		// 	int choice;
		// 	cout << "Its player2 turn" << '\n';
		// 	vector< pair<int,int> > options = cantStop.player2.filterDice(cantStop.rollDice());

		// 	if(options.size() > 0){
		// 		cantStop.player2.displayCombinations( options);
		// 		cout << "Choose a pair : " << '\n';
		// 		cin >> choice;
		// 		cantStop.player2.chooseDice(options[choice - 1]); //Choose a pair
		// 	}else{
		// 		goOn = false;
		// 		cantStop.player2.stateReference = cantStop.player2.state;
		// 	}
			
	// 	}
	// }

		vector<int> temp_tokens = vector<int>(11, 0);
		for (vector<int>::iterator it = player->currentCols.begin(); it != player->currentCols.end(); ++it) {
			int temp = *it;
			temp -= 2;
			temp_tokens[temp] = player->stateReference[temp];
		}

		{
			// Initial render (kinda a test)
			SDL_RenderClear(ren);

			SDL_Surface* board_surface = bv.get_surface(cantStop.player1.state, cantStop.player2.state, empty_vector, empty_vector, temp_tokens);
			SDL_Texture* board_texture = SDL_CreateTextureFromSurface(ren, board_surface);
			SDL_Rect board_destination;
			board_destination.x = 0;
			board_destination.y = 0;
			SDL_QueryTexture(board_texture, NULL, NULL, &board_destination.w, &board_destination.h);
			board_destination.w /= window_scale;
			board_destination.h /= window_scale;

			SDL_Surface* dice_surface = dv.get_surface(dice_options, &bad_dice);
			SDL_Texture* dice_texture = SDL_CreateTextureFromSurface(ren, dice_surface);
			SDL_Rect dice_destination;
			dice_destination.x = board_width / window_scale;
			dice_destination.y = 0;
			SDL_QueryTexture(dice_texture, NULL, NULL, &dice_destination.w, &dice_destination.h);
			dice_destination.w /= window_scale;
			dice_destination.h /= window_scale;

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
			if (!stop_active) SDL_RenderCopy(ren, dice_texture, NULL, &dice_destination);
			else if (stop_active) SDL_RenderCopy(ren, stop_texture, NULL, &stop_destination);
			SDL_RenderCopy(ren, options_texture, NULL, &options_destination);

			SDL_RenderPresent(ren);

			SDL_FreeSurface(board_surface);
			SDL_FreeSurface(dice_surface);
			SDL_FreeSurface(stop_surface);
			SDL_FreeSurface(options_surface);

			SDL_DestroyTexture(board_texture);
			SDL_DestroyTexture(dice_texture);
			SDL_DestroyTexture(stop_texture);
			SDL_DestroyTexture(options_texture);
		}

	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}



