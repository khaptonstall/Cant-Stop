#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

#include "GameState.h"

using namespace std;

// Function: rollDice
// Input: bool
// Output: vector<int>
// Description: Return a vector with 4 random dice 
vector<int> GameState::rollDice(bool b) {
	vector<int> output;
	for (int i = 0; i < 4; i++) {
		output.push_back(rand() % 6 + 1);
	}
	return output;
}


// Function: validatePair
// Input: int, int, Player*
// Output: bool
// Desciption: Compare each pair of dice to make sure the pair to make sure the player is not currently at the top of one of 
//	those columns, and that the pair does not contain a dead column
bool GameState::validatePair(int a, int b, Player* p) {
	if (p->currentCols.size() < 2 && (p->stateReference[a - 2] != filledCols[a - 2] && p->stateReference[b - 2] != filledCols[b - 2]) &&
		(find(deadCols.begin(), deadCols.end(), a) == deadCols.end()  && find(deadCols.begin(), deadCols.end(), b) == deadCols.end())) {
		return true;
	}
	else if (p->currentCols.size() == 2 && (find(p->currentCols.begin(), p->currentCols.end(), a) != p->currentCols.end() ||
				find(p->currentCols.begin(), p->currentCols.end(), b) != p->currentCols.end()) &&
					(p->stateReference[a - 2] != filledCols[a - 2] && p->stateReference[b - 2] != filledCols[b - 2]) &&
					( find(deadCols.begin(), deadCols.end(), a) == deadCols.end()  && find(deadCols.begin(), deadCols.end(), b) == deadCols.end() )) {
		return true;
	}
	else if (p->currentCols.size() == 3 && (find(p->currentCols.begin(), p->currentCols.end(), a) != p->currentCols.end() &&
				find(p->currentCols.begin(), p->currentCols.end(), b) != p->currentCols.end()) &&
					(p->stateReference[a - 2] != filledCols[a - 2] && p->stateReference[b - 2] != filledCols[b - 2]) &&
					(find(deadCols.begin(), deadCols.end(), a) == deadCols.end()  && find(deadCols.begin(), deadCols.end(), b) == deadCols.end())) {
		return true;
	}	

	else return false;
}


// Function: validatePair
// Input: int, Player*
// Output: bool
// Desciption: If the pair wasn't valid, call this function. Compare each die to make sure the player is not currently at the top of that
//	column, and that the die is not a dead column
bool GameState::validatePair(int a, Player* p) {
	if (p->currentCols.size() < 3 && (p->stateReference[a - 2] != filledCols[a - 2]) && find(deadCols.begin(), deadCols.end(), a) == deadCols.end()) {
		return true;
	}
	else if (find(p->currentCols.begin(), p->currentCols.end(), a) != p->currentCols.end() && (p->stateReference[a - 2] != filledCols[a - 2]) &&
			find(deadCols.begin(), deadCols.end(), a) == deadCols.end()) {
		return true;
	}
	else return false;
}


// Function: checkForDeadCols
// Input:
// Output: void
// Desciption: Once a player has reached the top of a column and stops, add that column to the vector of dead columns.
//	Also clears a column once one player has claimed it.
void GameState::checkForDeadCols(){
	for(int i = 0; i < player1.claimedCols.size(); i++){
		if(find(deadCols.begin(), deadCols.end(), player1.claimedCols[i]) == deadCols.end()){
			deadCols.push_back(player1.claimedCols[i] + 2); // + 2 to adjust to game board (1-12)
			cout << "Dead column: " << i << '\n';
			player2.stateReference[ player1.claimedCols[i]] = 0;
			player2.state[player1.claimedCols[i]] = 0;
		}
	}

	for(int j = 0; j < player2.claimedCols.size(); j++){
		if(find(deadCols.begin(), deadCols.end(), player2.claimedCols[j]) == deadCols.end()){
			deadCols.push_back(player2.claimedCols[j] + 2); // + 2 to adjust to game board (1-12)
			cout << "Dead column: " << j << '\n';
			player1.stateReference[ player2.claimedCols[j]] = 0;
			player1.state[ player2.claimedCols[j]] = 0;
			}
		}

	}
