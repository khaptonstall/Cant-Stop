#include "players/simple_player.h"

#include <SDL2/SDL_timer.h>

#include "GameState.h"

const int simple_player::FIXED_TURNS = 0;

simple_player::simple_player(string log_path, int delay)
	: cpu_player(log_path, delay) {

	current_turn = 0;
}

simple_player::~simple_player() {

}

pair<int, int> simple_player::select_dice_impl(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice) {
	for (pair<int, int> rp : rolled_pairs) {
		if (game_state->validatePair(rp.first, rp.second, p)) {
			return rp;
		}
	}

	for (pair<int, int> rp : rolled_pairs) {
		if (game_state->validatePair(rp.first, p)) {
			return make_pair(rp.first, -1);
		}
	}

	return make_pair(-1, -1);
}

int simple_player::select_decision_impl(GameState* game_state, int selected_decision) {

	return 2;

}

void simple_player::start_over_impl() {

}

void simple_player::revert_impl() {

}
