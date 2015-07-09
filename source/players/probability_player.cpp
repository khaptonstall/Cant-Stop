#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <SDL2/SDL_timer.h>

#include "GameState.h"
#include "players/probability_player.h"

#include "dice_probability.h"

using namespace std;

int const probability_player::SELECT_DELAY = 0;

probability_player::probability_player() {
	timer = 0;
	last_ticks = 0;
}

// Function: select_dice
// Input: GameState*, vector<pair<int,int> >, Player*, int
// Output: pair<int,int>
// Desciption: Currently picks the first valid pair of dice
pair<int, int> probability_player::select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice) {
	if (last_ticks == 0) {
		last_ticks = SDL_GetTicks();
		return pair<int, int>(-1, -1);
	}
	else if (timer < SELECT_DELAY) {
		timer += SDL_GetTicks() - last_ticks;
		return pair<int, int>(-1, -1);
	}
	else {
		timer = 0;
		last_ticks = 0;
	}

	int highestProb = 0;
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



// Function: select_decision
// Input: GameState*, int
// Output: int
// Desciption returning 1 = continue, returning 2 = stop
int probability_player::select_decision(GameState* game_state, int selected_decision) {
	if (last_ticks == 0) {
		last_ticks = SDL_GetTicks();
		return 0;
	}
	else if (timer < SELECT_DELAY) {
		timer += SDL_GetTicks() - last_ticks;
		return 0;
	}
	else {
		timer = 0;
		last_ticks = 0;
	}

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

	// If outlook is positive, continue
	double progress = 0;
	for (int i = 0; i < 11; i++) {
		//progress += state[i];
		progress += (stateReference[i] - state[i]) / ((double)filledCols[i]);
	}

	double expected_progress = dice_p.get_expected_progress(tokens[0], tokens[1], tokens[2]);
	double successful_probability = dice_p.get_probability(tokens[0], tokens[1], tokens[2]);

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

	// cout << "g = " << expected_progress << endl;

	// cout << "p = " << successful_probability << endl;
	// cout << "h = " << progress << endl;
	double lhs = successful_probability * (progress + expected_progress);
	double rhs = progress;

	// cout << "p(h+g) = " << lhs << " and h = " << rhs << endl;
	//cout << "p(h+g) = " << lhs << " and h = " << rhs << endl;

	if (lhs >= rhs)
		return 1;

	// Otherwise, stop!
	return 2;
}