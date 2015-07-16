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

//	rolled_pairs = p->filterDice(rolled_pairs);
/*	std::cout << "Rolled pairs size: " << rolled_pairs.size() << std::endl;
	if (rolled_pairs.size() == 2) {
		return rolled_pairs[0];
	}else if (rolled_pairs.size() == 0) {
		return pair<int,int>(-1,-1);
	} */

	for (int i = 0; i < rolled_pairs.size(); i++) {
		//std::cout << rolled_pairs[i].first << " " << rolled_pairs[i].second << std::endl;
	}
	pair<int,int> chosenPair = MCTS(game_state, rolled_pairs, p).first;
//	std::cout << "Chosen: " << chosenPair.first << " " << chosenPair.second << std::endl;
	return chosenPair;
}

Node monte_carlo_player::MCTS(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p){
	Node best = Node(pair<int,int>(-1,-1), 0);
	vector<pair<int, int> > validPairs;

	for (int i = 0; i < rolled_pairs.size(); i++) {
		if (game_state->validatePair(rolled_pairs[i].first,rolled_pairs[i].second,p)) {
			validPairs.push_back(rolled_pairs[i]);
			best = Node(rolled_pairs[i],0);
		}else if(game_state->validatePair(rolled_pairs[i].first,p)){
			validPairs.push_back(pair<int,int>(rolled_pairs[i].first, -1));
			best = Node(pair<int,int>(rolled_pairs[i].first, -1),0);
		}else if (game_state->validatePair(rolled_pairs[i].second,p)) {
			validPairs.push_back(pair<int,int>(rolled_pairs[i].second, -1));
			best = Node(pair<int,int>(rolled_pairs[i].second, -1),0);
		}
	}



//	Node best = Node(pair<int,int>(rolled_pairs[0].first,rolled_pairs[0].second),0);

	for (int i = 0;  i < validPairs.size(); i++) {
		GameState* monteCarloState = new GameState();
		*monteCarloState->player1 = *game_state->player1;
		*monteCarloState->player2 = *p;
		monteCarloState->deadCols = game_state->deadCols;

		vector<Node> visited;
		Node node = Node(pair<int,int>(validPairs[i].first,validPairs[i].second),0);
	//	Node node = Node(pair<int,int>(-1,-1), 0);


			if (monteCarloState->validatePair(validPairs[i].first, validPairs[i].second, monteCarloState->player2)) {
				node = Node(validPairs[i], 0);
				visited.push_back(node);
				monteCarloState->player2->chooseDice(validPairs[i]);

			}
			else if (monteCarloState->validatePair(validPairs[i].first, monteCarloState->player2)) {
				node = Node(validPairs[i], 0);
				visited.push_back(node);
				monteCarloState->player2->chooseDice(validPairs[i]);

			}else{
				visited.push_back(best);
			}

		pair<int,int> root = visited[0].first;

		//Add new child to the tree
		vector<pair<int,int> > nextRoll = rollDice(monteCarloState, monteCarloState->player2);
		Node newChild = Node(pair<int,int>(-1,-1),0);
		for (int i = 0; i < nextRoll.size(); i++) {
			if (monteCarloState->validatePair(nextRoll[i].first, nextRoll[i].second, monteCarloState->player2)) {
				monteCarloState->player2->chooseDice(nextRoll[i]);
				newChild = Node(nextRoll[i],0);
				break;

			}else if (monteCarloState->validatePair(nextRoll[i].first, monteCarloState->player2)) {
				monteCarloState->player2->chooseDice(pair<int,int>(nextRoll[i].first, -1));
				newChild = Node(pair<int,int>(nextRoll[i].first,-1),0);

				break;
			}else if (monteCarloState->validatePair(nextRoll[i].second, monteCarloState->player2)) {
				monteCarloState->player2->chooseDice(pair<int,int>(nextRoll[i].second, -1));
				newChild = Node(pair<int,int>(nextRoll[i].second, -1),0);
				break;
			}
		}

		int value =  rollOut(monteCarloState, monteCarloState->player2, newChild).second;
		newChild.second = value;
		visited.push_back(newChild);
		if (value > 0) {
			rollAgain += 1;
		}else{
			rollAgain -= 1;
		}

	for (Node n : visited){
		visited[0].second += n.second;
	}
	if (visited[0].second > best.second) {
		if (find(monteCarloState->deadCols.begin(),monteCarloState->deadCols.end(), best.first.first) == monteCarloState->deadCols.end() &&
				find(monteCarloState->deadCols.begin(),monteCarloState->deadCols.end(), best.first.second) == monteCarloState->deadCols.end() &&
				find(validPairs.begin(),validPairs.end(), best.first) != validPairs.end()) {

					if (best.first.first != -1 && best.first.second != -1) {
						if (find(validPairs.begin(),validPairs.end(), best.first) != validPairs.end()) {
						//	std::cout << "Change best to: " << visited[0].first.first << " " << visited[0].first.second << std::endl;
							best = visited[0];
						}
					}else{
						//best = visited[0];

					}
		}
	//	best = visited[0];
	}
}

	return best;
}


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
	for (int i = 0; i < monteCarloState->deadCols.size(); i++) {
	//	std::cout << "Dead dice: " << monteCarloState->deadCols[i] << std::endl;

	}

	vector<pair<int,int> > diceOptions;
	for (int i = 0; i < rolled_pairs.size(); i++) {
		if (monteCarloState->validatePair(rolled_pairs[i].first, rolled_pairs[i].second, monteCarloState->player2)) {
			diceOptions.push_back(rolled_pairs[i]);
		}else if (monteCarloState->validatePair(rolled_pairs[i].first, monteCarloState->player2)) {
			diceOptions.push_back(rolled_pairs[i]);
		}
	}

	//diceOptions = p->filterDice(rolled_pairs);

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

	vector<int> tokens;
	for (int j = 0; j < 11; j++) {
		if (state[j] != stateReference[j])
			tokens.push_back(j + 2);
	}

	if (game_state->canStop() == false){
		rollAgain = 0;
		return 1;
	}


		// Stop if you just got to the top
	for (int i = 0; i < 11; i++) {
		if (stateReference[i] == game_state->filledCols[i] && find(currentCols.begin(), currentCols.end(), i+2) != currentCols.end() &&
			tokens.size() == 3){
			rollAgain = 0;
			return 2;
		}
	}
	// Less than three tokens, continue
	if (tokens.size() < 3)
		return 1;

		if (rollAgain > 0) {
			rollAgain = 0;
			return 1;
		}
		else{
			rollAgain = 0;
			return 2;
		}

}

// Function: rollOut
// Input: GameState*, Player*, pair<int,int>
// Output: int
// Desciption:
Node monte_carlo_player::rollOut(GameState* game_state, Player* p, Node childNode){
	GameState* monteCarloState = new GameState();
	*monteCarloState->player1 = *game_state->player1;
	*monteCarloState->player2 = *p;
	monteCarloState->deadCols = game_state->deadCols;

	int value = 0;
	pair<int,int> roll = childNode.first;


	if (monteCarloState->validatePair(roll.first, roll.second, monteCarloState->player2)) {
		monteCarloState->player2->chooseDice(roll);
		monteCarloState->player2->checkForWin();
		monteCarloState->checkForDeadCols();

		if (monteCarloState->player2->stateReference[roll.first - 2] == monteCarloState->filledCols[roll.first - 2] ||
			monteCarloState->player2->stateReference[roll.second - 2] == monteCarloState->filledCols[roll.second - 2]){
				childNode.second += 100;
				return childNode;
			}else{
				childNode.second += 2;
			}
			//monteCarloState->player2->checkForWin();
			//monteCarloState->checkForDeadCols();
			vector<pair<int,int> > nextRoll = rollDice(monteCarloState, monteCarloState->player2);
			if (nextRoll[0].first == -1 && nextRoll[0].second == -1) {
				return Node(roll, childNode.second - 100);
			}else{
				rollOut(monteCarloState, monteCarloState->player2, Node(nextRoll[0],0));
			}
		}else if (monteCarloState->validatePair(roll.first, monteCarloState->player2)) {

			monteCarloState->player2->chooseDice(roll);
			monteCarloState->player2->checkForWin();
			monteCarloState->checkForDeadCols();

			if (monteCarloState->player2->stateReference[roll.first - 2] == monteCarloState->filledCols[roll.first - 2]){
				childNode.second += 100;
				return childNode;
			}else{
				childNode.second += 1;
				//monteCarloState->player2->checkForWin();
				//monteCarloState->checkForDeadCols();
				vector<pair<int,int> > nextRoll = rollDice(monteCarloState, monteCarloState->player2);

				if (nextRoll[0].first == -1 && nextRoll[0].second == -1) {
					return Node(roll, childNode.second - 100);
				}else{
					rollOut(monteCarloState, monteCarloState->player2, Node(nextRoll[0],0));
				}

			}
		} else{
			return Node(roll, childNode.second - 100);
		}
		return childNode;
}
