#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "GameState.h"

#include "views/board_view.h"
#include "views/dice_view.h"
#include "controllers/dice_controller.h"
#include "views/stop_view.h"
#include "controllers/stop_controller.h"
#include "views/options_view.h"
#include "controllers/options_controller.h"
#include "views/log_view.h"

using namespace std;

log_view lv;
const bool TESTING_MODE = false;

int main(int, char**){
	//Seed random number
	srand(time(NULL));
	system("clear");
	SDL_Window* win = nullptr;
	SDL_Renderer* ren = nullptr;

	if (!TESTING_MODE) {
		if (SDL_Init(SDL_INIT_VIDEO) != 0){
			cout << "SDL_Init Error: " << SDL_GetError() << endl;
			return 1;
		}

		if (TTF_Init() != 0) {
			cout << "TTF_Init: " << TTF_GetError() << endl;
			return 1;
		}
	}

	// Define display things
	// Create views
	board_view bv;
	dice_view dv(TESTING_MODE);
	stop_view sv;
	options_view ov;
	lv.init(19, TESTING_MODE);

	int window_scale = 1;
	int board_width = bv.get_width();
	int dice_width = dv.get_width();
	int dice_height = dv.get_height();
	int board_height = bv.get_height();

	if (!TESTING_MODE) {

		win = SDL_CreateWindow("Can't Stop", 100, 100, (board_width + dice_width) / window_scale, board_height / window_scale, SDL_WINDOW_SHOWN);
		if (win == nullptr){
			cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
			SDL_Quit();
			return 1;
		}

		ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
		if (ren == nullptr){
			SDL_DestroyWindow(win);
			cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
			SDL_Quit();
			return 1;
		}

	}

	// Create controllers
	dice_controller dc(dv, board_width, 0, window_scale);
	stop_controller sc(sv, board_width, 0, window_scale);
	options_controller oc(ov, board_width, dice_height, window_scale);

	SDL_Event e;
	SDL_Point mouse_pos;
	vector<int> empty_vector = vector<int>();

	/////////////////////////////////////////////////////////////////////////////////////
	GameState cantStop;
	Player* player = cantStop.player1;

	bool quit = false;
	bool dice_active = false;
	bool stop_active = false;
	int dice_pair = 0;
	int stop_continue = 0;
	vector<int> dice_options;
	int game_over = false;

	// Statistics tracker
	const int MAX_GAMES = 2000;
	int current_game = 0;
	int player1_wins = 0;
	int player2_wins = 0;
	bool testing = TESTING_MODE;

	// Stuff so that the game doesn't render every frame
	vector<int> prevPlayer1State(11, 0);
	vector<int> prevPlayer2State(11, 0);
	vector<int> prevTempState(11, 0);
	bool renderDice = false;

	while (!quit) {
		bool check_dice_input = false;
		int option_selected = 0;

		////////////////////////////////////////////////

		if (!TESTING_MODE) {

			// Event handler
			SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
					if (!stop_active) {
						check_dice_input = true;
					}
					else {
						stop_continue = sc.input(mouse_pos.x, mouse_pos.y, cantStop.canStop());
					}
					option_selected = oc.input(mouse_pos.x, mouse_pos.y);
				}
				else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
					stop_continue = 0;
					dice_pair = 0;
				}
				else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p) {
					game_over = !game_over;
				}
			}

			// Start a new game
			if (option_selected == 1) {
				cantStop.startOver();
				dice_active = false;
				stop_active = false;
				player = cantStop.player1;
				game_over = false;
			}

		}
		// Logic
		// If new dice need to be rolled
		if (!dice_active) {
			dice_active = true;
			renderDice = true;
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
				if (player->name == "Player 2") {
					if (result.second > 0) {
						lv.println("Computer move:" + to_string(result.first) + " & " + to_string(result.second));
						// cout << "Computer picked: " << result.first << " + " << result.second << '\n';
					}else{
						lv.println("Computer move:" + to_string(result.first));
						// cout << "Computer picked: " << result.first << '\n';
					}
				}
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
					lv.println(player->name + " wins!");
					if (testing) {
						if ((current_game + 1) % 500 == 0)
							std::cout << "Game finished: #" << current_game + 1 << std::endl;
						++current_game;
						if (player == cantStop.player1) {
							player1_wins++;
						}
						else {
							player2_wins++;
						}

						if (current_game < MAX_GAMES) {
							cantStop.startOver();
							dice_active = false;
							stop_active = false;
							player = cantStop.player1;
							game_over = false;
						}
						else if (game_over == false) {
							cantStop.startOver();
							dice_active = false;
							stop_active = false;
							player = cantStop.player1;
							game_over = true;
							cout << "Player 1 won " << player1_wins << " / " << MAX_GAMES << endl;
							quit = !quit;
						}
					}
					else { game_over = true; }
				}
				player->currentCols.clear();
				if (player == cantStop.player1){ player = cantStop.player2;
					if (player->claimedCols.size() < 3 and !game_over){
						lv.clear();
					}
				}else if (player == cantStop.player2) player = cantStop.player1;
			}
		}
		// No dice choice is valid, revert
		else if (!game_over) {
			if (player == cantStop.player2) {
				lv.println("No possible moves!");
			}
			dice_active = false;
			player->revert();
			// if (player == cantStop.player1) cout << "No valid dice rolls" << endl << "============" << endl;
			if (player == cantStop.player1){
				player = cantStop.player2;
				lv.clear();
				// system("clear");
			}else if (player == cantStop.player2) player = cantStop.player1;
		}

		// Determine temporary tokens to display
		vector<int> temp_tokens = vector<int>(11, 0);
		for (vector<int>::iterator it = player->currentCols.begin(); it != player->currentCols.end(); ++it) {
			int temp = *it;
			temp -= 2;
			temp_tokens[temp] = player->stateReference[temp];
		}

		bool renderScreen = true;
		// for (int i = 0; i < 11; i++) {
		// 	if (cantStop.player1->state[i] != prevPlayer1State[i]) {
		// 		renderScreen = true;
		// 		break;
		// 	}
		// 	else if (cantStop.player2->state[i] != prevPlayer2State[i]) {
		// 		renderScreen = true;
		// 		break;
		// 	}
		// 	else if (temp_tokens[i] != prevTempState[i]) {
		// 		renderScreen = true;
		// 		break;
		// 	}
		// }

		// if (!renderScreen and renderDice) {
		// 	renderScreen = true;
		// 	renderDice = false;
		// }

		// prevPlayer1State = cantStop.player1->state;
		// prevPlayer2State = cantStop.player2->state;
		// prevTempState = temp_tokens;

		// if (!renderScreen) {
		// 	SDL_Delay(1.0 / 60.0);
		// }

		if (!TESTING_MODE and renderScreen) {
			// Render
			if (!testing) {
				SDL_RenderClear(ren);

				SDL_Surface* board_surface = bv.get_surface(cantStop.player1->state, cantStop.player2->state, empty_vector, empty_vector, temp_tokens);
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

				// SDL_Surface* stop_surface = sv.get_surface(mouse_pos.x, mouse_pos.y, cantStop.canStop());
				SDL_Surface* stop_surface = sv.get_surface(mouse_pos.x, mouse_pos.y, true);
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

				SDL_Surface* log_surface = lv.get_surface();
				SDL_Texture* log_texture = SDL_CreateTextureFromSurface(ren, log_surface);
				SDL_Rect log_destination;
				log_destination.x = board_width / window_scale + 15;
				log_destination.y = dice_height / window_scale + 90;
				SDL_QueryTexture(log_texture, NULL, NULL, &log_destination.w, &log_destination.h);
				log_destination.w /= window_scale;
				log_destination.h /= window_scale;

				SDL_RenderCopy(ren, board_texture, NULL, &board_destination);
				if (!stop_active) SDL_RenderCopy(ren, dice_texture, NULL, &dice_destination);
				else if (stop_active) SDL_RenderCopy(ren, stop_texture, NULL, &stop_destination);
				SDL_RenderCopy(ren, options_texture, NULL, &options_destination);
				SDL_RenderCopy(ren, log_texture, NULL, &log_destination);

				SDL_RenderPresent(ren);

				SDL_FreeSurface(board_surface);
				SDL_FreeSurface(dice_surface);
				SDL_FreeSurface(stop_surface);
				SDL_FreeSurface(options_surface);
				SDL_FreeSurface(log_surface);

				SDL_DestroyTexture(board_texture);
				SDL_DestroyTexture(dice_texture);
				SDL_DestroyTexture(stop_texture);
				SDL_DestroyTexture(options_texture);
				SDL_DestroyTexture(log_texture);
			}

		}
	}

	if (!TESTING_MODE) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		SDL_Quit();
	}
	return 0;
}
