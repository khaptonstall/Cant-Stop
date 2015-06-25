#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <SDL2/SDL_timer.h>

#include "GameState.h"
#include "probability_player.h"

#include "dice_probability.h"

using namespace std;

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


/*
	for (pair<int, int> rp : rolled_pairs) {
		// cout << "AI Evaluating: " << rp.first << ", " << rp.second << endl;
		if (game_state->validatePair(rp.first, rp.second, p)) {
			cout << "AI Pair picked: " << rp.first << ", " << rp.second << endl;
			return rp;
		}
	}

	for (pair<int, int> rp : rolled_pairs) {
		// cout << "AI Evaluation: " << rp.first << endl;
		if (game_state->validatePair(rp.first, p)) {
			cout << "Ai Pair picked: " << rp.first << endl;
			return pair<int, int>(rp.first, -1);
		}
	}
	return pair<int,int>(-1,-1); */
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

	vector<int> tokens;
	for (int i = 0; i < 11; i++) {
		if (state[i] != stateReference[i] && state[i] != filledCols[i])
			tokens.push_back(i + 2);
	}
	for (int i = tokens.size(); i < 3; i++) {
		tokens.push_back(0);
	}

	// Less than three tokens, continue
	if (tokens.size() < 3 && game_state->deadCols.size() == 0)
		return 1;

	// Stop iif you're at the top of a column and used all 3 tokens 
	for (int i = 0; i < 11; i++) {
		if (stateReference[i] == game_state->filledCols[i] && find(currentCols.begin(), currentCols.end(), i+2) != currentCols.end() &&
			tokens.size() == 3)
			return 2;
	}

	// If outlook is positive, continue
	int progress = 0;
	for (int i = 0; i < 11; i++) {
		//progress += state[i];
		progress += stateReference[i] - state[i];
	}

	double expected_progress = dice_p.get_expected_progress(tokens[0], tokens[1], tokens[2]);
	//cout << "g = " << expected_progress << endl;
	double successful_probability = dice_p.get_probability(tokens[0], tokens[1], tokens[2]);
	//cout << "p = " << successful_probability << endl;
	double lhs = successful_probability * (progress + expected_progress);
	double rhs = progress;

	//cout << "p(h+g) = " << lhs << " and h = " << rhs << endl;
	//cout << "p(h+g) = " << lhs << " and h = " << rhs << endl;

	if (lhs >= rhs)
		return 1;

	// Otherwise, stop!
	return 2;
}