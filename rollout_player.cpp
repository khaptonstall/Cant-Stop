#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

#include "GameState.h"
#include "rollout_player.h"

#include "dice_probability.h"

using namespace std;

// Function: select_dice
// Input: GameState*, vector<pair<int,int> >, Player*, int
// Output: pair<int,int>
// Desciption: Currently picks the first valid pair of dice
pair<int, int> rollout_player::select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice) {



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
					highestProb = dice_p.get_probability(rp.first, 0, 0 + columnValue);
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
int rollout_player::select_decision(GameState* game_state, int selected_decision ) {
	// Stop if you just got to the top
	for (int i = 0; i < 11; i++) {
		if (stateReference[i] == game_state->filledCols[i] && find(currentCols.begin(), currentCols.end(), i+2) != currentCols.end())
			return 2;
	}

	vector<int> tokens;
	for (int i = 0; i < 11; i++) {
		if (state[i] != stateReference[i])
			tokens.push_back(i + 2);
	}

	// Less than three tokens, continue
	if (tokens.size() < 3)
		return 1;

	if (rollOut(game_state, this)){
		return 1;
	}else{
		return 2;
	}

}

bool rollout_player::rollOut(GameState* game_state, Player* p){
	int probability = 0;
	for(int i = 0; i < 100; i++){
		vector<int> dice_options;
		dice_options = game_state->rollDice(true);
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
			if (game_state->validatePair(rolled_pairs[i].first, this)) {
				valid_pairs = true;
			}
			else {
				bad_dice[i] = true;
			}
		}
		if(valid_pairs){
			probability++;
		}
	}
	cout << "probability before: " << probability << '\n';
	for (int i = 0; i < currentCols.size(); i++){
		int index = currentCols[i] - 2;
		if(stateReference[index] > state[index]){
			probability -= stateReference[index] - state[index];
			probability -= stateReference[index] - state[index];

		}
	}

	cout << "probability after:  " << (probability) << '\n';
	if( (probability) > 70){
		return true;
	}else{
		return false;
	}
}
