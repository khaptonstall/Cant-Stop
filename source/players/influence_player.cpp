#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <algorithm>
#include <SDL2/SDL_timer.h>

#include "GameState.h"
#include "players/influence_player.h"

#include "dice_probability.h"

using namespace std;

influence_player::influence_player(string log_path, int delay)
	: cpu_player(log_path, delay) {
}

influence_player::~influence_player() {

}

// Function: select_dice
// Input: GameState*, vector<pair<int,int> >, Player*, int
// Output: pair<int,int>
// Desciption: Currently picks the first valid pair of dice
pair<int, int> influence_player::select_dice_impl(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice) {
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



// Function: select_decision
// Input: GameState*, int
// Output: int
// Desciption returning 1 = continue, returning 2 = stop
int influence_player::select_decision_impl(GameState* game_state, int selected_decision) {
	// if (game_state->canStop() == false){
	// 	return 1;
	// }

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

	// If outlook is positive, continue
	double progress = 0;
	for (int i = 0; i < 11; i++) {
		//progress += state[i];
		progress += (stateReference[i] - state[i]) / ((double)filledCols[i]);
	}

	double expected_progress = dice_p.get_expected_progress(tokens[0], tokens[1], tokens[2]);
	double successful_probability = dice_p.get_probability(tokens[0], tokens[1], tokens[2]);

	// cout << "DEBUG: " << tokens.size() << ", " << game_state->deadCols.size() << endl;
	// If deadCols > 0 and tokens < 3, successful probability is chance of getting a non-dead column
	if (find(tokens.begin(), tokens.end(), 0) != tokens.end() and game_state->deadCols.size() != 0) {
		vector<int> liveCols;
		for (int i = 2; i <= 12; i++) {
			if (find(game_state->deadCols.begin(), game_state->deadCols.end(), i) == game_state->deadCols.end()) {
				// cout << "Pushing on: " << i << endl;
				liveCols.push_back(i);
			}
		}
		successful_probability = dice_p.get_probability(liveCols);
		// cout << "There are dead columns and the new probability is: " << successful_probability << endl;
	}

	vector<double> relative_eprogresses;
	for (int t : tokens) {
		if (t != 0)
			relative_eprogresses.push_back(expected_progress / filledCols[t - 2]);
	}

	expected_progress = 0;
	for (double rp : relative_eprogresses) {
		expected_progress += rp;
	}

	expected_progress /= relative_eprogresses.size();

	// Influence stuff
	vector<double> influences;
	for (int i = 0; i < 11; i++) {
		if (state[i] != stateReference[i]) {
			double col_influence;
			// cout << "Token diff: " << game_state->tokenDistance(i, this) << endl;
			// cout << "Column len: " << filledCols[i] << endl;
			col_influence = abs(1.0 - (((double)game_state->tokenDistance(i, this)) / (filledCols[i])));
			// cout << "Influence on column " << i + 2 << ": " << col_influence << endl;
			// col_influence *= dice_p.get_probability(i + 2, 0, 0);
			if (col_influence > .70)
				influences.push_back(col_influence);
			else
				influences.push_back(0);
		}
	}

	double avg_influence = 0;
	for (double i : influences) {
		avg_influence += i;
		// cout << "Influence " << i << endl;
	}
	avg_influence /= influences.size();
	// cout << "Avg influence: " << avg_influence << endl;

	if (influences.size() < 2)
		avg_influence = 0;

	// double scaled_influence = avg_influence * successful_probability;
	double scaled_influence = avg_influence * (successful_probability - (successful_probability * successful_probability));
	scaled_influence += successful_probability * successful_probability;

	// cout << "g = " << expected_progress << endl;
	// cout << "p = " << successful_probability << endl;
	// cout << "h = " << progress << endl;
	// cout << "i = " << scaled_influence << endl;
	double lhs = (scaled_influence / successful_probability) * (progress + expected_progress);
	double rhs = progress;

	// cout << "(i/p)*(h+g) = " << lhs << " and h = " << rhs << endl;
	// cout << "p(h+g) = " << lhs << " and h = " << rhs << endl;

	if (lhs >= rhs)
		return 1;

	// Otherwise, stop!
	return 2;
}

void influence_player::start_over_impl() {

}

void influence_player::revert_impl() {

}