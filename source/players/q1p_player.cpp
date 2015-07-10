#include "players/q1p_player.h"

#include <SDL2/SDL_timer.h>

#include "GameState.h"

q1p_player::q1p_player(string log_path, uint16_t delay)
	: cpu_player(log_path) {
	selection_delay = delay;
	timer = 0;
	last_ticks = 0;
}

q1p_player::~q1p_player() {

}

pair<int, int> q1p_player::select_dice_impl(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice) {
	if (last_ticks == 0) {
		last_ticks = SDL_GetTicks();
		return make_pair(-1, -1);
	}
	else if (timer < selection_delay) {
		timer += SDL_GetTicks() - last_ticks;
		last_ticks = SDL_GetTicks();
		return make_pair(-1, -1);
	}
	else {
		timer = 0;
		last_ticks = 0;
	}

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

int q1p_player::select_decision_impl(GameState* game_state, int selected_decision) {
	if (last_ticks == 0) {
		last_ticks = SDL_GetTicks();
		return 0;
	}
	else if (timer < selection_delay) {
		timer += SDL_GetTicks() - last_ticks;
		last_ticks = SDL_GetTicks();
		return 0;
	}
	else {
		timer = 0;
		last_ticks = 0;
	}

	if (game_state->canStop())
		return 2;
	else
		return 1;

}