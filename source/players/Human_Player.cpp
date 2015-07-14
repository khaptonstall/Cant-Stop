#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

#include "GameState.h"
#include "players/Human_Player.h"

using namespace std;

pair<int, int> Human_Player::select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int clicked_pair) {
	pair<int, int> selected_dice;
	// Idenitify what pair was clicked
	if (clicked_pair == 0) return pair<int, int>(-1, -1);
	else if (clicked_pair == 1) selected_dice = rolled_pairs[0];
	else if (clicked_pair == 2) selected_dice = rolled_pairs[1];
	else if (clicked_pair == 3) selected_dice = rolled_pairs[2];
	else if (clicked_pair == 4) selected_dice = rolled_pairs[3];
	else if (clicked_pair == 5) selected_dice = rolled_pairs[4];
	else if (clicked_pair == 6) selected_dice = rolled_pairs[5];

	if (game_state->validatePair(selected_dice.first, selected_dice.second, p)) {
		return selected_dice;
	}
	else if (game_state->validatePair(selected_dice.first, p)) {
		return pair<int, int>(selected_dice.first, -1);
	}
	else return pair<int, int>(-1, -1);
}

int Human_Player::select_decision(GameState* game_state, int selected_decision) {
	return selected_decision;
}
