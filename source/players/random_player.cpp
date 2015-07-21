#include "players/random_player.h"

#include <SDL2/SDL_timer.h>

#include "GameState.h"

random_player::random_player(string log_path, int delay)
	: cpu_player(log_path, delay) {
}

random_player::~random_player() {

}

pair<int, int> random_player::select_dice_impl(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice) {
	int num_pairs = 0;
	for (pair<int, int> rp : rolled_pairs) {
		if (game_state->validatePair(rp.first, rp.second, p)) {
			num_pairs++;
		}
	}

	int random_option;
	if (num_pairs != 0) {
		random_option = rand() % num_pairs;
	}
	else random_option = 0;
	for (pair<int, int> rp : rolled_pairs) {
		if (game_state->validatePair(rp.first, rp.second, p) and random_option == 0) {
			return rp;
		}
		else if (game_state->validatePair(rp.first, rp.second, p)) random_option--;
	}

	/////////

	num_pairs = 0;
	for (pair<int, int> rp : rolled_pairs) {
		if (game_state->validatePair(rp.first, p)) {
			num_pairs++;
		}
	}

	if (num_pairs != 0) {
		random_option = rand() % num_pairs;
	}
	else random_option = 0;
	for (pair<int, int> rp : rolled_pairs) {
		if (game_state->validatePair(rp.first, p) and random_option == 0) {
			return make_pair(rp.first, -1);
		}
		else if (game_state->validatePair(rp.first, p)) random_option--;
	}

	return make_pair(-1, -1);
}

int random_player::select_decision_impl(GameState* game_state, int selected_decision) {
	if (!game_state->canStop())
		return 1;
	else if (rand() % 2)
		return 1;
	else
		return 2;

}