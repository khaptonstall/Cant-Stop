#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <SDL2/SDL_timer.h>

#include "players/monte_carlo_player.h"

#include "GameState.h"
#include "dice_probability.h"

using namespace std;
typedef pair<pair<int,int>,int> Node;
int const monte_carlo_player::SELECT_DELAY = 0;

monte_carlo_player::monte_carlo_player() {
	timer = 0;
	last_ticks = 0;
}


// Function: select_dice
// Input: GamesState*, vector<pair<int,int>>, Player*, int
// Output: pair<int,int>
// Description: Returns the best result from the MCTS
pair<int, int> monte_carlo_player::select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int select_dice) {
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


	return MCTS(game_state, rolled_pairs, p).first;

}


// Function: MCTS
// Input: GameState*, vector<pair<int,int>, Player*
// Output: Node
// Description: Performs a Monte Carlo Tree Search to determine that best move to make this turn
Node monte_carlo_player::MCTS(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p){
	Node best;
	Node root;
	vector<pair<int, int> > validPairs;

	GameState* monteCarloState = new GameState();
	*monteCarloState->player1 = *game_state->player1;
	*monteCarloState->player2 = *p;
	monteCarloState->deadCols = game_state->deadCols;

	// Seperate valid and invalid pairs
	for (int i = 0; i < rolled_pairs.size(); i++) {
		if (game_state->validatePair(rolled_pairs[i].first,rolled_pairs[i].second,p)) {
			if(find(validPairs.begin(), validPairs.end(), pair<int,int>(rolled_pairs[i].second, rolled_pairs[i].first)) == validPairs.end() &&
			   find(validPairs.begin(), validPairs.end(), rolled_pairs[i]) == validPairs.end()){
				validPairs.push_back(rolled_pairs[i]);
			}
		}else if(game_state->validatePair(rolled_pairs[i].first,p)){
			if(find(validPairs.begin(), validPairs.end(), pair<int,int>(rolled_pairs[i].second, rolled_pairs[i].first)) == validPairs.end() &&
				 find(validPairs.begin(), validPairs.end(), rolled_pairs[i]) == validPairs.end()	){
			validPairs.push_back(pair<int,int>(rolled_pairs[i].first, -1));
		}
		}else if (game_state->validatePair(rolled_pairs[i].second,p)) {
			validPairs.push_back(pair<int,int>(rolled_pairs[i].second, -1));
		}
	}
	if (validPairs.size() == 0 || (validPairs.size() == 1 && validPairs[0].first == -1 && validPairs[0].second == -1 )) {
		best = Node(pair<int,int>(-1,-1), -50);
	}else{
		best = Node(validPairs[0], 0);
	}

	vector<Node> visited;

	for (int i = 0;  i < validPairs.size(); i++) {

		GameState* monteCarloState = new GameState();
		*monteCarloState->player1 = *game_state->player1;
		*monteCarloState->player2 = *p;
		monteCarloState->deadCols = game_state->deadCols;

		root = Node(validPairs[i], findValue(monteCarloState, monteCarloState->player2, validPairs[i]));
		visited.push_back(root);
		if (root.second > 9) {
			return root;
		}else if (root.first.first == -1 && root.first.second == -1) {
			int value = 0;
			for (int j = 0; j < monteCarloState->player2->stateReference.size(); j++) {
				value -= monteCarloState->player2->stateReference[j] - monteCarloState->player2->state[j];
			}
			return Node(root.first, root.second - value);
		}
		monteCarloState->player2->chooseDice(validPairs[i]);



		pair<int,int> rootPair = visited[0].first;

		//Add new child to the tree
		vector<pair<int,int> > nextRoll = rollDice(monteCarloState, monteCarloState->player2);
		visited.push_back(MCTS(monteCarloState, nextRoll, monteCarloState->player2));

	for (Node n : visited){
		visited[0].second += n.second;
	}
	visited[0].second -= visited[0].second; //Remove what was added twice in the loop

	if (visited[0].second > best.second) {
		if (find(monteCarloState->deadCols.begin(),monteCarloState->deadCols.end(), best.first.first) == monteCarloState->deadCols.end() &&
				find(monteCarloState->deadCols.begin(),monteCarloState->deadCols.end(), best.first.second) == monteCarloState->deadCols.end() &&
				find(validPairs.begin(),validPairs.end(), visited[0].first) != validPairs.end()) {
					if (visited[0].first.first != -1 && visited[0].first.second != -1) {
							best = visited[0];
					}
		}
	}
}
	return best;
}

int monte_carlo_player::findValue(GameState* game_state, Player* p, pair<int,int> roll){
	GameState* monteCarloState = new GameState();
	*monteCarloState->player1 = *game_state->player1;
	*monteCarloState->player2 = *p;
	monteCarloState->deadCols = game_state->deadCols;
	int value = 0;

	if (monteCarloState->validatePair(roll.first, roll.second, monteCarloState->player2)) {
		monteCarloState->player2->chooseDice(roll);
		monteCarloState->player2->checkForWin();
		monteCarloState->checkForDeadCols();

		// Attain value of current progress
		for (int i = 0; i < monteCarloState->player2->stateReference.size(); i++) {
			value += monteCarloState->player2->stateReference[i] - monteCarloState->player2->state[i];
		}

		if (monteCarloState->player2->stateReference[roll.first - 2] == monteCarloState->filledCols[roll.first - 2] ||
			monteCarloState->player2->stateReference[roll.second - 2] == monteCarloState->filledCols[roll.second - 2]){
				value += 10;
			}else{
				value += 2;
			}
			return value;
		}else if (monteCarloState->validatePair(roll.first, monteCarloState->player2)) {
			// Simulate making the move
			monteCarloState->player2->chooseDice(roll);
			monteCarloState->player2->checkForWin();
			monteCarloState->checkForDeadCols();

			for (int i = 0; i < monteCarloState->player2->stateReference.size(); i++) {
				value += monteCarloState->player2->stateReference[i] - monteCarloState->player2->state[i];
			}

			if (monteCarloState->player2->stateReference[roll.first - 2] == monteCarloState->filledCols[roll.first - 2]){
				value += 10;
			}else{
				value += 1;
			}
			return value;
		} else{ //No valid pair
				for (int i = 0; i < monteCarloState->player2->stateReference.size(); i++) {
					value -= monteCarloState->player2->stateReference[i] - monteCarloState->player2->state[i];
				}
				return value;
		}
}


// Function: rollDice
// Input: GameState*, Player*
// Output: vector<pair<int,int>>
// Description: Used by MCTS to simulate a new dice roll and filters out all
// invalid pairs
vector<pair<int,int> > monte_carlo_player::rollDice(GameState* g, Player* p){
	GameState* monteCarloState = new GameState();
	*monteCarloState->player1 = *g->player1;
	*monteCarloState->player2 = *p;
	monteCarloState->deadCols = g->deadCols;

	vector<int> rolled_dice;
	rolled_dice = monteCarloState->rollDice(true);

	vector<pair<int, int> > rolled_pairs(6);
	rolled_pairs[0] = pair<int, int>(rolled_dice[0] + rolled_dice[1], rolled_dice[2] + rolled_dice[3]);
	rolled_pairs[1] = pair<int, int>(rolled_dice[2] + rolled_dice[3], rolled_dice[0] + rolled_dice[1]);
	rolled_pairs[2] = pair<int, int>(rolled_dice[0] + rolled_dice[2], rolled_dice[1] + rolled_dice[3]);
	rolled_pairs[3] = pair<int, int>(rolled_dice[1] + rolled_dice[3], rolled_dice[0] + rolled_dice[2]);
	rolled_pairs[4] = pair<int, int>(rolled_dice[0] + rolled_dice[3], rolled_dice[2] + rolled_dice[1]);
	rolled_pairs[5] = pair<int, int>(rolled_dice[2] + rolled_dice[1], rolled_dice[0] + rolled_dice[3]);


	vector<pair<int,int> > diceOptions;
	for (int i = 0; i < rolled_pairs.size(); i++) {
		if (monteCarloState->validatePair(rolled_pairs[i].first, rolled_pairs[i].second, monteCarloState->player2)) {
			diceOptions.push_back(rolled_pairs[i]);
		}else if (monteCarloState->validatePair(rolled_pairs[i].first, monteCarloState->player2)) {
			diceOptions.push_back(rolled_pairs[i]);
		}
	}

	if (diceOptions.size() == 0) {
		diceOptions.push_back(pair<int,int>(-1,-1));
	}

	return diceOptions;
}
// Function: select_decision
// Input: GameState*, int
// Output: int
// Desciption:
int monte_carlo_player::select_decision(GameState* game_state, int selected_decision ) {
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
			probability -= (stateReference[index] - state[index]); // * 2;
			if(game_state->filledCols[index] - stateReference[index] == 1){
				probability += 10;
			}else if(game_state->filledCols[index] - stateReference[index] == 1){
				probability += 5;
			}
		}
	}
	//std::cout << "Computer probability of rolling another valid pair: " << probability  << std::endl;
	// cout << "probability after:  " << (probability) << '\n';
	std::cout << "Probability" << probability << std::endl;
	if( (probability) >= 73){
		return 1;
	}else{
		return 2;
	}
/*
	vector<int> tokens;
	for (int j = 0; j < 11; j++) {
		if (state[j] != stateReference[j])
			tokens.push_back(j + 2);
	}

	if (game_state->canStop() == false){
		return 1;
	}


		// Stop if you just got to the top
	for (int i = 0; i < 11; i++) {
		if (stateReference[i] == game_state->filledCols[i] && find(currentCols.begin(), currentCols.end(), i+2) != currentCols.end() &&
			tokens.size() == 3){
			return 2;
		}
	}
	// Less than three tokens, continue
	if (tokens.size() < 3){
		return 1;
}
		if (roll) {
			return 1;
		}
		else{
			return 2;
		}
*/
}
