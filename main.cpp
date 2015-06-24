#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "GameState.h"

#include "board_view.h"
#include "dice_view.h"
#include "dice_controller.h"
#include "stop_view.h"
#include "stop_controller.h"
#include "options_view.h"
#include "options_controller.h"

using namespace std;

int main(int, char**){
	//Seed random number 
	srand(time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		return 1;
	}

	if (TTF_Init() != 0) {
		cout << "TTF_Init: " << TTF_GetError() << endl;
    	return 1;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	// Create views
	board_view bv;
	dice_view dv;
	stop_view sv;
	options_view ov;

	int window_scale = 1;
	int board_width = bv.get_width();
	int dice_width = dv.get_width();
	int dice_height = dv.get_height();
	int board_height = bv.get_height();

	// Create controllers
	dice_controller dc(dv, board_width, 0, window_scale);
	stop_controller sc(sv, board_width, 0, window_scale);
	options_controller oc(ov, board_width, dice_height, window_scale);

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

	GameState cantStop;
	cantStop.player1.turn = true;
	Player* player = &cantStop.player1;

	bool quit = false;
	bool dice_active = false;
	bool stop_active = false;
	int dice_pair = 0;
	int stop_continue = 0;
	SDL_Event e;
	vector<int> dice_options;
	int game_over = false;


	while (!quit) {
		SDL_Point mouse_pos;
		SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
		bool check_dice_input = false;
		int option_selected = 0;
		// Event handler
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
				if (!stop_active) {
					check_dice_input = true;
				}
				else {
					stop_continue = sc.input(mouse_pos.x, mouse_pos.y);
				}
				option_selected = oc.input(mouse_pos.x, mouse_pos.y);
			}
			else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
				stop_continue = 0;
				dice_pair = 0;
			}
		}

		// Start a new game
		if (option_selected == 1) {
			cantStop.startOver();
			dice_active = false;
			stop_active = false;
			player = &cantStop.player1;
			game_over = false;
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

		if (check_dice_input)
			dice_pair = dc.input(mouse_pos.x, mouse_pos.y, bad_dice, dice_options, &cantStop, player);

		// Wait for player to choose a dice pair
		if (!stop_active && valid_pairs && !game_over) {
			pair<int, int> result = player->select_dice(&cantStop, rolled_pairs, player, dice_pair);
			if (result != pair<int, int>(-1, -1)) {
				player->chooseDice(result);
				stop_active = true;
			}
		}
		// Dice has been selected, now choose stop / continue
		else if (stop_active && !game_over) {
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
				if (player->claimedCols.size() >= 3) {
					cout << player->name << " wins!" << endl;
					game_over = true;
				}
				player->currentCols.clear();
				if (player == &cantStop.player1) player = &cantStop.player2;
				else if (player == &cantStop.player2) player = &cantStop.player1;
			}
		}
		// No dice choice is valid, revert
		else if (!game_over) {
			dice_active = false;
			player->stateReference = player->state;
			player->currentCols.clear();
			if (player == &cantStop.player1) player = &cantStop.player2;
			else if (player == &cantStop.player2) player = &cantStop.player1;
		}

		// Determine temporary tokens to display
		vector<int> temp_tokens = vector<int>(11, 0);
		for (vector<int>::iterator it = player->currentCols.begin(); it != player->currentCols.end(); ++it) {
			int temp = *it;
			temp -= 2;
			temp_tokens[temp] = player->stateReference[temp];
		}

		// Render
		{
			SDL_RenderClear(ren);

			SDL_Surface* board_surface = bv.get_surface(cantStop.player1.state, cantStop.player2.state, empty_vector, empty_vector, temp_tokens);
			SDL_Texture* board_texture = SDL_CreateTextureFromSurface(ren, board_surface);
			SDL_Rect board_destination;
			board_destination.x = 0;
			board_destination.y = 0;
			SDL_QueryTexture(board_texture, NULL, NULL, &board_destination.w, &board_destination.h);
			board_destination.w /= window_scale;
			board_destination.h /= window_scale;

			SDL_Surface* dice_surface = dv.get_surface(dice_options, mouse_pos.x, mouse_pos.y, &bad_dice, &cantStop, player);
			SDL_Texture* dice_texture = SDL_CreateTextureFromSurface(ren, dice_surface);
			SDL_Rect dice_destination;
			dice_destination.x = board_width / window_scale;
			dice_destination.y = 0;
			SDL_QueryTexture(dice_texture, NULL, NULL, &dice_destination.w, &dice_destination.h);
			dice_destination.w /= window_scale;
			dice_destination.h /= window_scale;

			SDL_Surface* stop_surface = sv.get_surface(mouse_pos.x, mouse_pos.y);
			SDL_Texture* stop_texture = SDL_CreateTextureFromSurface(ren, stop_surface);
			SDL_Rect stop_destination;
			stop_destination.x = board_width / window_scale;
			stop_destination.y = 0;
			SDL_QueryTexture(stop_texture, NULL, NULL, &stop_destination.w, &stop_destination.h);
			stop_destination.w /= window_scale;
			stop_destination.h /= window_scale;

			SDL_Surface* options_surface = ov.get_surface(mouse_pos.x, mouse_pos.y);
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



