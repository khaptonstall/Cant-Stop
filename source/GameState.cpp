#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

#include "GameState.h"

using namespace std;

vector<int> const GameState::filledCols = {3,5,7,9,11,13,11,9,7,5,3};

GameState::GameState() {
	player1 = new probability_player();
	player2 = new monte_carlo_player();
	player1->name = "Player 1";
	player2->name = "Player 2";

}

GameState::~GameState() {
	delete player1;
	delete player2;
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
		output.push_back(rand() % 6 + 1);
		// output.push_back(distribution(rd));
	}
	return output;
}

// Function: canStop
// Input:
// Output: bool
// Description: If a player is on top of another player they cannot stop, return false.
//	Otherwise return true.
bool GameState::canStop(){
	for(int i = 0; i < player1->stateReference.size(); i++){
		if( (player1->stateReference[i] == player2->state[i] && player1->stateReference[i] != 0) ||
		(player2->stateReference[i] == player1->state[i] && player2->stateReference[i] != 0)) {
			return false;
		}
	}
	return true;
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
	for(int i : player1->claimedCols) {
		if(find(deadCols.begin(), deadCols.end(), i) == deadCols.end()){
			deadCols.push_back(i); // + 2 to adjust to game board (1-12)
			player2->stateReference[i - 2] = 0;
			player2->state[i - 2] = 0;
		}
	}

	for(int j : player2->claimedCols){
		if(find(deadCols.begin(), deadCols.end(), j) == deadCols.end()){
			deadCols.push_back(j); // + 2 to adjust to game board (1-12)
			player1->stateReference[j - 2] = 0;
			player1->state[j - 2] = 0;
		}
	}
}

bool GameState::checkSpace(int col, int prog) {
	bool output = false;
	if (player1->stateReference[col] == prog) {
		output = true;
		return output;
	}
	else if (player2->stateReference[col] == prog) {
		output = true;
		return output;
	}
	return output;
}

void GameState::startOver() {
	deadCols.clear();
	player1->startOver();
	player2->startOver();
}

int GameState::gameResult() {
	if (player1->claimedCols.size() >= 3)
		return 1;
	else if (player2->claimedCols.size() >= 3)
		return 2;
	else
		return 0;
}

// Return -1 for loss, 0 for no result yet, 1 for win
int GameState::checkPlayerResult(Player* &p) {
	int result = gameResult();
	if (result == 0) return 0;
	else if (result == 1 and p == player1)
		return 1;
	else if (result == 2 and p == player2)
		return 1;
	else
		return -1;
}

// Check player with closest token to a player p and return the distance
int GameState::tokenDistance(int col, Player* p) {
	if (p == player1) {
		return abs(player1->stateReference[col] - player2->state[col]);
	}
	else if (p == player2) {
		return abs(player2->stateReference[col] - player2->state[col]);
	}
	else return -1;
}


int GameState::tokenDisplacement(int col, Player* p) {
	if (p == player1) {
		return player1->stateReference[col] - player2->state[col];
	}
	else if (p == player2) {
		return player2->stateReference[col] - player1->state[col];
	}
	else
		return -100;
}

double GameState::getPlayerScore(int p) {
	vector<double> columnScores;
	if (p == 1) {
		columnScores = vector<double>(player1->stateReference.begin(),
										player1->stateReference.end());
	}
	else {
		columnScores = vector<double>(player2->stateReference.begin(),
										player2->stateReference.end());
	}

	for (int i = 0; i < columnScores.size(); i++) {
		columnScores[i] /= (double)filledCols[i];
		// cout << columnScores[i] << " ";
	}
	// cout << endl;
	sort(columnScores.begin(), columnScores.end());

	return (columnScores[10] + columnScores[9] + columnScores[8]) / 3.0;
}
