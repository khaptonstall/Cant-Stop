#include <iostream>
#include <vector>
#include <algorithm>
#include <random>


#include "GameState.h"

using namespace std;

vector<int> const GameState::filledCols = {3,5,7,9,11,13,11,9,7,5,3};

GameState::GameState() {
	player1.name = "Player 1";
	player2.name = "Player 2";

}

// Function: rollDice
// Input: bool
// Output: vector<int>
// Description: Return a vector with 4 random dice 
vector<int> GameState::rollDice(bool b) {
	vector<int> output;
	std::random_device rd;
	std::uniform_int_distribution<int> distribution(1,6);
	for (int i = 0; i < 4; i++) {
		//output.push_back(rand() % 6 + 1);
		output.push_back(distribution(rd));
	}
	return output;
}




// Function: validatePair
// Input: int, int, Player*
// Output: bool
// Desciption: Compare each pair of dice to make sure the pair to make sure the player is not currently at the top of one of 
//	those columns, and that the pair does not contain a dead column
bool GameState::validatePair(int a, int b, Player* p) {

	bool identicalPairIsPlayable = true;

	if (find(deadCols.begin(), deadCols.end(), a) != deadCols.end() || find(deadCols.begin(), deadCols.end(), b) != deadCols.end()) {
		return false;
	}

	if(a == b){
		if((filledCols[a-2] - p->stateReference[a-2]) < 2){
			identicalPairIsPlayable = false;
		}
	}
	if (p->currentCols.size() < 2 && (p->stateReference[a - 2] != filledCols[a - 2] && p->stateReference[b - 2] != filledCols[b - 2]) &&
		(find(deadCols.begin(), deadCols.end(), a) == deadCols.end()  && find(deadCols.begin(), deadCols.end(), b) == deadCols.end()) &&
		identicalPairIsPlayable) {
		return true;
	}
	else if (p->currentCols.size() == 2 && (find(p->currentCols.begin(), p->currentCols.end(), a) != p->currentCols.end() ||
				find(p->currentCols.begin(), p->currentCols.end(), b) != p->currentCols.end() || a == b) &&
					(p->stateReference[a - 2] != filledCols[a - 2] && p->stateReference[b - 2] != filledCols[b - 2]) &&
					( find(deadCols.begin(), deadCols.end(), a) == deadCols.end()  && find(deadCols.begin(), deadCols.end(), b) == deadCols.end()) &&
					identicalPairIsPlayable) {
		if (find(p->currentCols.begin(), p->currentCols.end(), a) != p->currentCols.end() &&  find(p->currentCols.begin(), p->currentCols.end(), b) != p->currentCols.end())
			return true;
		else if (a != b && !((find(p->currentCols.begin(), p->currentCols.end(), a) != p->currentCols.end() &&  find(p->currentCols.begin(), p->currentCols.end(), b) == p->currentCols.end()) ||
							(find(p->currentCols.begin(), p->currentCols.end(), b) != p->currentCols.end() &&  find(p->currentCols.begin(), p->currentCols.end(), a) == p->currentCols.end()))) {

			return false;
		}
		return true;
	}
	else if (p->currentCols.size() == 3 && (find(p->currentCols.begin(), p->currentCols.end(), a) != p->currentCols.end() &&
				find(p->currentCols.begin(), p->currentCols.end(), b) != p->currentCols.end()) &&
					(p->stateReference[a - 2] != filledCols[a - 2] && p->stateReference[b - 2] != filledCols[b - 2]) &&
					(find(deadCols.begin(), deadCols.end(), a) == deadCols.end()  && find(deadCols.begin(), deadCols.end(), b) == deadCols.end()) &&
					identicalPairIsPlayable) {
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
	for(int i : player1.claimedCols) {
		if(find(deadCols.begin(), deadCols.end(), i) == deadCols.end()){
			deadCols.push_back(i + 2); // + 2 to adjust to game board (1-12)
			player2.stateReference[i] = 0;
			player2.state[i] = 0;
		}
	}

	for(int j : player2.claimedCols){
		if(find(deadCols.begin(), deadCols.end(), j) == deadCols.end()){
			deadCols.push_back(j + 2); // + 2 to adjust to game board (1-12)
			player1.stateReference[j] = 0;
			player1.state[j] = 0;
		}
	}
}

void GameState::startOver() {
	deadCols.clear();
	player1.startOver();
	player2.startOver();
}
