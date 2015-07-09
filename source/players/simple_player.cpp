#include "players/simple_player.h"

#include <SDL2/SDL_timer.h>

#include "GameState.h"

simple_player::simple_player(string log_path, uint16_t delay) {
	timer = 0;
	last_ticks = 0;
}

simple_player::~simple_player() {

}

pair<int, int> simple_player::select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice) {
	if (last_ticks == 0) {
		last_ticks = SDL_GetTicks();
		return make_pair(-1, -1);
	}
	else if (timer < selection_delay) {
		timer += SDL_GetTicks() - last_ticks;
		return make_pair(-1, -1);
	}
	else {
		timer = 0;
		last_ticks = 0;
	}

	for (pair<int, int> rp : rolled_pairs) {
		if (game_state->validatePair(rp.first, rp.second, p)) {
			timer = last_ticks = 0;
			return rp;
		}
	}

	for (pair<int, int> rp : rolled_pairs) {
		if (game_state->validatePair(rp.first, p)) {
			timer = last_ticks = 0;
			return make_pair(rp.first, -1);
		}
	}

	return make_pair(-1, -1);
}

int simple_player::select_decision(GameState* game_state, int selected_decision) {
	return 2;
}