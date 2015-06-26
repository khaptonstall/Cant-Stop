#include <iostream>
#include <vector>
#include <SDL2/SDL_timer.h>

#include "dumb_player.h"

#include "GameState.h"
#include "dice_probability.h"

using namespace std;

int const dumb_player::SELECT_DELAY = 0;

dumb_player::dumb_player() {
	timer = 0;
	last_ticks = 0;
}

pair<int, int> dumb_player::select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice) {
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

	if(highestPair.first != 0) {
		return highestPair;
	}
	else {
		return pair<int,int>(-1,-1);
	}
}

int dumb_player::select_decision(GameState* game_state, int selected_decision) {
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

	double progress = 0;
	int counter = 0;
	for (int i = 0; i < 11; i++) {
		if (state[i] != stateReference[i]) {
			counter++;
			// cout << "difference: " << stateReference[i] - state[i] << endl;
			// cout << "len: " << game_state->filledCols[i] << endl;
			if (game_state->filledCols[i] != state[i])
				progress += (double)(stateReference[i] - state[i]) / game_state->filledCols[i];
		}
	}
	progress /= counter;

	double probability = dice_p.get_probability(tokens[0], tokens[1], tokens[2]);

	// cout << "Probability: " << probability << endl;
	// cout << "Progress: " << progress << endl;
	// cout << "Result: " << probability - progress << endl;

	if (probability - progress > 0.31) {
		return 1;
	}
	else {
		// cout << "Stopped" << endl;
		return 2;
	}

}

