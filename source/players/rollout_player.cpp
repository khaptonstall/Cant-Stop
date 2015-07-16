#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <SDL2/SDL_timer.h>

#include "players/rollout_player.h"

#include "GameState.h"
#include "dice_probability.h"

#include "views/log_view.h"
extern log_view lv;

using namespace std;

int const rollout_player::SELECT_DELAY = 0;

rollout_player::rollout_player() {
	timer = 0;
	last_ticks = 0;
}

// Function: select_dice
// Input: GameState*, vector<pair<int,int> >, Player*, int
// Output: pair<int,int>
// Desciption: Pick a pair of dice with the highest prob of being rolled again
pair<int, int> rollout_player::select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int select_dice) {
	// Decision delay
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
					highestProb = dice_p.get_probability(rp.first, 0, 0 + columnValue);
					highestPair = make_pair(rp.first, -1);
				}
			}
		}
	}

	if(highestPair.first != 0){
		return highestPair;
	}else{ //There was no good pair
		// cout << "Computer had no possible move." << '\n';
		return pair<int,int>(-1,-1);
	}
}



// Function: select_decision
// Input: GameState*, int
// Output: int
// Desciption returning 1 = continue, returning 2 = stop
int rollout_player::select_decision(GameState* game_state, int selected_decision ) {
	// Decision delay
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
		if (state[i] != stateReference[i])
			tokens.push_back(i + 2);
	}
	if (game_state->canStop() == false){
		return 1;
	}
		// Stop if you just got to the top
	for (int i = 0; i < 11; i++) {
		if (stateReference[i] == game_state->filledCols[i] && find(currentCols.begin(), currentCols.end(), i+2) != currentCols.end() &&
			tokens.size() == 3)
			return 2;
	}

	// Less than three tokens, continue
	if (tokens.size() < 3 && rollOut(game_state, this)){
		//std::cout << "Computer continued" << std::endl;
		return 1;
}
	if (rollOut(game_state, this)){
		//std::cout << "Computer continued" << std::endl;
		return 1;
	}else{
		lv.println("Computer stopped");
		// std::cout << "Computer stopped" << std::endl;
		return 2;
	}

}

// Function: rollOut
// Input: GameState*, Player*
// Output: bool
// Desciption: Creates a pobability based on 100 dice rolls. Subtracts current progress.
//	Adds to the probability if close to claiming a column. Returns a bool deciding to roll again or not
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
		for (int i = 0; i < 6; i++) {
			if (game_state->validatePair(rolled_pairs[i].first, this)) {
				valid_pairs = true;
			}
		}
		if(valid_pairs){
			probability++;
		}
	}
	// cout << "probability before: " << probability << '\n';
	for (int i = 0; i < currentCols.size(); i++){
		int index = currentCols[i] - 2;
		if(stateReference[index] > state[index]){
			probability -= (stateReference[index] - state[index]) * 2;
			if(game_state->filledCols[index] - stateReference[index] == 1){
				probability += 10;
			}else if(game_state->filledCols[index] - stateReference[index] == 1){
				probability += 5;
			}
		}
	}
	// std::cout << "Computer probability of rolling another valid pair: " << probability  << std::endl;
	// cout << "probability after:  " << (probability) << '\n';
	if( (probability) >= 75){
		return true;
	}else{
		return false;
	}
}
