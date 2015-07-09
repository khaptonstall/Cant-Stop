#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

#include "GameState.h"
#include "players/AI_Player.h"

using namespace std;

// Function: select_dice
// Input: GameState*, vector<pair<int,int> >, Player*, int
// Output: pair<int,int>
// Desciption: Currently picks the first valid pair of dice
pair<int, int> AI_Player::select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice) {
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
	return pair<int,int>(-1,-1); 
}

// Function: select_decision
// Input: GameState*, int
// Output: int
// Desciption returning 1 = continue, returning 2 = stop
int AI_Player::select_decision(GameState* game_state, int selected_decision) {
	int counter = 0;
	for(int i = 0; i < stateReference.size(); i++){
		if(stateReference[i] == game_state->filledCols[i]){
			counter++;
		}
	}

	if(counter > claimedCols.size() ) {
		return 2;
	}

	return 1;

	
}