#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include "Player.cpp"
using namespace std;

class GameState{
public:
	// ********** Variables ***********
	Player player1;
	Player player2;
	vector<int> deadCols;
	vector<int> filledCols = {3,5,7,9,11,13,11,9,7,5,3}; // Vector to compare when a player is at the top of the column

	// ********** Methods ************
	vector< pair<int,int> > rollDice();
	void checkForDeadCols();
	vector<int> rollDice(bool b);
	bool validateDice(vector<int> d, Player* p);
	bool validatePair(int a, int b, Player* p);
	bool validatePair(int a, Player* p);
};

vector<int> GameState::rollDice(bool b) {
	vector<int> output;
	for (int i = 0; i < 4; i++) {
		output.push_back(rand() % 6 + 1);
	}
	return output;
}

bool GameState::validateDice(vector<int> d, Player* p) {
	vector<pair<int,int> > pairs;
	pairs.push_back(pair<int,int>(d[0] + d[1], d[2] + d[3]));
	pairs.push_back(pair<int,int>(d[0] + d[2], d[1] + d[3]));
	pairs.push_back(pair<int,int>(d[0] + d[3], d[2] + d[1]));

	if (p->currentCols.size() == 0 || p->currentCols.size() == 1){ //All combinations are legal
		return true;

	}else if(p->currentCols.size() == 2){ 
		for(int i = 0; i < pairs.size(); i++){
			//As long as one number in the pair is currently in play:
			if(find(p->currentCols.begin(), p->currentCols.end(), pairs[i].first) != p->currentCols.end() ||
			   find(p->currentCols.begin(), p->currentCols.end(), pairs[i].second) != p->currentCols.end()){
				return true;
			//Else If neither in the pair is currently in play:
			}else if(pairs[i].first == pairs[i].second){ //Check if the pair is the same number, if it is it is playable:
				return true;
			}else{
				return true;
			}
		}
	}else if( p->currentCols.size() == 3){
		for(int i = 0; i < pairs.size(); i++){
			if(find(p->currentCols.begin(), p->currentCols.end(), pairs[i].first) != p->currentCols.end() &&
			   find(p->currentCols.begin(), p->currentCols.end(), pairs[i].second) != p->currentCols.end()){
				return true;

			}
			else if(find(p->currentCols.begin(), p->currentCols.end(), pairs[i].first) != p->currentCols.end() &&
				find(p->currentCols.begin(), p->currentCols.end(), pairs[i].second) == p->currentCols.end() ){
				return true;
			}
			else if(find(p->currentCols.begin(), p->currentCols.end(), pairs[i].first) == p->currentCols.end() &&
				find(p->currentCols.begin(), p->currentCols.end(), pairs[i].second) != p->currentCols.end() ){
				return true;
			}
		}
	}
	return false;
}

bool GameState::validatePair(int a, int b, Player* p) {
	if (p->currentCols.size() < 2) {
		return true;
	}
	else if (p->currentCols.size() == 2 && (find(p->currentCols.begin(), p->currentCols.end(), a) != p->currentCols.end() ||
		find(p->currentCols.begin(), p->currentCols.end(), b) != p->currentCols.end())) {
		return true;
	}
	else if (p->currentCols.size() == 3 && (find(p->currentCols.begin(), p->currentCols.end(), a) != p->currentCols.end() &&
		find(p->currentCols.begin(), p->currentCols.end(), b) != p->currentCols.end())) {
		return true;
	}
	else return false;
}

bool GameState::validatePair(int a, Player* p) {
	if (p->currentCols.size() < 3) {
		return true;
	}
	else if (find(p->currentCols.begin(), p->currentCols.end(), a) != p->currentCols.end()) {
		return true;
	}
	else return false;
}


// GameState::rollDice()
// Input: 
// Output: vector< pair<int,int> > combinations
// Description: (1) Get four rand dice rolls, 
//				(2) Check how many columns the player is currently occupying
//				(3) Add possible pairs to the combinations vector to display to player
vector< pair<int,int> > GameState::rollDice(){
	vector< pair<int,int> > pairs;

	int dice1 = rand() % 6 + 1;
	int dice2 = rand() % 6 + 1;
	int dice3 = rand() % 6 + 1;
	int dice4 = rand() % 6 + 1;
	cout << dice1 << " " << dice2 << " " << dice3 << " " << dice4 << '\n';

	pair<int,int> pair1 = make_pair((dice1 + dice2), (dice3 + dice4));
	pair<int,int> pair2 = make_pair((dice1 + dice3), (dice2 + dice4));
	pair<int,int> pair3 = make_pair((dice2 + dice3), (dice1 + dice4));

	//pairs.push_back(pair1);
	//pairs.push_back(pair2);
	//pairs.push_back(pair3);
	for(int i = 0; i < filledCols.size(); i++){
		if(player1.stateReference[i] == filledCols[i] || player2.stateReference[i] == filledCols[i]){
			//cout << 
			deadCols.push_back(i);
		}
	}

	if(find(deadCols.begin(), deadCols.end(), pair1.first) == deadCols.end() &&
			find(deadCols.begin(), deadCols.end(), pair1.second) == deadCols.end() ){
			pairs.push_back(pair1);
	}else{
			if(find(deadCols.begin(), deadCols.end(), pair1.first) != deadCols.end()){
				pair1.first = -1;
			}

			if(find(deadCols.begin(), deadCols.end(), pair1.second) != deadCols.end()){
				pair1.second = -1;
			}
			pairs.push_back(pair1);
		}	

	if(find(deadCols.begin(), deadCols.end(), pair2.first) == deadCols.end() &&
			find(deadCols.begin(), deadCols.end(), pair2.second) == deadCols.end() ){
			pairs.push_back(pair2);
	}else{
			if(find(deadCols.begin(), deadCols.end(), pair2.first) != deadCols.end()){
				pair2.first = -1;
			}

			if(find(deadCols.begin(), deadCols.end(), pair2.second) != deadCols.end()){
				pair2.second = -1;
			}
			pairs.push_back(pair2);
		}	

			if(find(deadCols.begin(), deadCols.end(), pair3.first) == deadCols.end() &&
			find(deadCols.begin(), deadCols.end(), pair3.second) == deadCols.end() ){
			pairs.push_back(pair3);
	}else{
			if(find(deadCols.begin(), deadCols.end(), pair3.first) != deadCols.end()){
				pair3.first = -1;
			}

			if(find(deadCols.begin(), deadCols.end(), pair3.second) != deadCols.end()){
				pair3.second = -1;
			}
			pairs.push_back(pair3);
		}	


		/*
	//Remove dead columns from available options
	for(int i = 0; i < pairs.size(); i++){
		if(find(deadCols.begin(), deadCols.end(), pairs[i].first) != deadCols.end() &&
			find(deadCols.begin(), deadCols.end(), pairs[i].second) != deadCols.end() ){
			cout << "Erase: " << pairs[i].first << " and " << pairs[i].second << '\n';
			pairs.erase(pairs.begin() + i);

		}else{
			if(find(deadCols.begin(), deadCols.end(), pairs[i].first) != deadCols.end()){
				cout << "Erase: " << pairs[i].first << '\n';
				pairs[i].first = -1;
			}

			if(find(deadCols.begin(), deadCols.end(), pairs[i].second) != deadCols.end()){
				cout << "Erase: " << pairs[i].second << '\n';
				pairs[i].second = -1;
			}
		}	
	}

*/
	return pairs;
}


void GameState::checkForDeadCols(){
	for(int i = 0; i < player1.claimedCols.size(); i++){
		if(find(deadCols.begin(), deadCols.end(), player1.claimedCols[i]) == deadCols.end()){
			cout << "DEAD COL: " << (player1.claimedCols[i] + 2) << '\n';
			deadCols.push_back(player1.claimedCols[i] + 2); // + 2 to adjust to game board (1-12)
		}
	}

	for(int j = 0; j < player2.claimedCols.size(); j++){
		if(find(deadCols.begin(), deadCols.end(), player2.claimedCols[j]) == deadCols.end()){
			deadCols.push_back(player2.claimedCols[j] + 2); // + 2 to adjust to game board (1-12)
		}
	}
}

