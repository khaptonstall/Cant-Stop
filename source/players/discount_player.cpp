#include "players/discount_player.h"

#include <SDL2/SDL_timer.h>

#include "GameState.h"

discount_player::discount_player(string log_path, int delay)
	: cpu_player(log_path, delay) {
}

discount_player::~discount_player() {

}

pair<int, int> discount_player::select_dice_impl(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice) {
	// Check what columns player is on
	vector<int> tokens;
	for (int i = 0; i < 11; i++) {
		if (state[i] != stateReference[i] && state[i] != filledCols[i])
			tokens.push_back(i + 2);
	}

	if (tokens.size() < 3) {
		double highestProb = 0;
		pair<int,int> highestPair = make_pair(0,0);

		for(pair<int,int> rp : rolled_pairs){
			double columnValue = stateReference[rp.first - 2] / filledCols[rp.first - 2] + stateReference[rp.second - 2] / filledCols[rp.second - 2];
			if((columnValue + dice_p.get_probability(rp.first, rp.second, 0)) > highestProb) {
				if(game_state->validatePair(rp.first, rp.second, p)){
					highestProb = dice_p.get_probability(rp.first, rp.second, 0) + columnValue;
					highestPair = rp;
				}
			}
		}

		if(highestPair.first != 0){
			return highestPair;
		}else{
			for(pair<int,int> rp : rolled_pairs){
				double columnValue = stateReference[rp.first - 2] / filledCols[rp.first - 2];
				if((columnValue + dice_p.get_probability(rp.first, 0, 0)) > highestProb){
					if(game_state->validatePair(rp.first, p)){
						highestProb = dice_p.get_probability(rp.first, 0, 0) + columnValue;
						highestPair = make_pair(rp.first, -1);
					}
				}
			}
		}

		if(highestPair.first != 0){
			return highestPair;
		}else{
			return pair<int,int>(-1,-1);
		}
	}
	else {
		double highestTravel = 0;
		pair<int, int> highestPair = make_pair(0, 0);

		for (pair<int, int> rp : rolled_pairs) {
			if(game_state->validatePair(rp.first, rp.second, p)) {
				double travel = 1.0 / filledCols[rp.first - 2] + 1 / filledCols[rp.second - 2];
				if (travel > highestTravel) {
					highestTravel = travel;
					highestPair = rp;
				}
			}
		}

		if (highestPair.first != 0) {
			return highestPair;
		}

		for (pair<int, int> rp : rolled_pairs) {
			if (game_state->validatePair(rp.first, p)) {
				double travel = 1.0 / filledCols[rp.first - 2];
				if (travel > highestTravel) {
					highestTravel = travel;
					highestPair = make_pair(rp.first, -1);
				}
			}
		}
		if (highestPair.first != 0) {
			return highestPair;
		}
		else
			return make_pair(-1, -1);
	}
}

int discount_player::select_decision_impl(GameState* game_state, int selected_decision) {
	if (game_state->canStop() == false){
		return 1;
	}

	vector<int> tokens;
	for (int i = 0; i < 11; i++) {
		if (state[i] != stateReference[i] && state[i] != filledCols[i])
			tokens.push_back(i + 2);
	}
	// Less than three tokens, continue
	if (tokens.size() < 3 && game_state->deadCols.size() == 0)
		return 1;

	for (int i = tokens.size(); i < 3; i++) {
		tokens.push_back(0);
	}

	// Stop iif you're at the top of a column and used all 3 tokens 
	for (int i = 0; i < 11; i++) {
		if (stateReference[i] == game_state->filledCols[i] && find(currentCols.begin(), currentCols.end(), i+2) != currentCols.end() &&
			tokens.size() == 3)
			return 2;
	}

	double progress = 0;
	double col_length_sum = 0;
	int counter = 0;
	for (int i = 0; i < 11; i++) {
		if (state[i] != stateReference[i]) {
			counter++;
			// cout << "difference: " << stateReference[i] - state[i] << endl;
			// cout << "len: " << game_state->filledCols[i] << endl;
			if (game_state->filledCols[i] != state[i]) {
				int col_progress = stateReference[i] - state[i];
				if (game_state->checkSpace(i, col_progress + 1))
					col_progress++;
				progress += (double) col_progress / game_state->filledCols[i];
				col_length_sum += game_state->filledCols[i];
			}
		}
	}
	progress /= counter;

	double probability = dice_p.get_probability(tokens[0], tokens[1], tokens[2]);
	double expected_progress = dice_p.get_expected_progress(tokens[0], tokens[1], tokens[2]) / col_length_sum;

	// cout << "Probability: " << probability << endl;
	// cout << "Progress: " << progress << endl;
	// cout << "Result: " << probability - progress << endl;

	if (probability - progress > 0.33) {
		return 1;
	}
	else {
		// cout << "Stopped" << endl;
		return 2;
	}

	return 2;

}

void discount_player::start_over_impl() {

}

void discount_player::revert_impl() {
	
}