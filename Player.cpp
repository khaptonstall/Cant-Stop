#include <iostream>
#include <vector>
#include <algorithm>

#include "Player.h"

#include "GameState.h"

using namespace std;

vector<int> const Player::filledCols = {3,5,7,9,11,13,11,9,7,5,3};

Player::Player() {
	state = vector<int>(11, 0);
	stateReference = vector<int>(11, 0);

}

// Player::chooseDice()
// Input: pair<int,int> dice
// Output: void
// Description: (1)  Check to make sure each dice is valid (Invalid dice are set to -1)
//				(2)  Increment the player's token in the column in the state
//				(3)  If the dice roll is not currently in the set of the currentCols, add it
void Player::chooseDice( pair<int, int> dice ){
	if(dice.first > 0){
		stateReference[(dice.first - 2)] = stateReference[(dice.first - 2)] + 1;
		if(find(currentCols.begin(), currentCols.end(), dice.first) == currentCols.end() && currentCols.size() < 3){
			currentCols.push_back(dice.first);
			//cout << "Pushed back: " << dice.first << '\n';
		}
	}

	if(dice.second > 0){
		stateReference[(dice.second - 2)] = stateReference[(dice.second - 2)] + 1;
		if(find(currentCols.begin(), currentCols.end(), dice.second) == currentCols.end()  && currentCols.size() < 3){
			currentCols.push_back(dice.second);
			//cout << "Pushed back: " << dice.second << '\n';

		}
	}
} 


// Player::displayCombinations()
// Input: vector< pair <int,int> > combinations
// Output: void
// Description: (1)  For all combinations, display them to the user
void Player::displayCombinations(vector< pair<int,int> > combinations){
	for(int i = 0; i < combinations.size(); i++){
		if(combinations[i].first > 0){
			if(combinations[i].second > 0){
				cout << combinations[i].first << " + " << combinations[i].second << '\n';
			}else{
				cout << combinations[i].first << '\n';
			}
		}else if(combinations[i].first < 0){
			if(combinations[i].second > 0){
				cout << combinations[i].second << '\n';
			}
		}
	}
}

void Player::checkForWin(){
	// int a = 3;
	// for(int i = 0; i < ((stateReference.size() / 2) + 1); i++){
	// 	//cout << stateReference[i] << '\n';
	// 	if(stateReference[i] == a && find(claimedCols.begin(), claimedCols.end(), i) == claimedCols.end()){
	// 		claimedCols.push_back(i);
	// 		//cout << "GOT IT" << i << '\n';
	// 	}
	// 	a += 2;
	// }
	// //cout << "a = " << a << '\n';
	// a = 11;
	// for(int i = ((stateReference.size() /2) + 1); i < stateReference.size(); i++){
	// 	//cout << stateReference[i] << '\n';
	// 	if(stateReference[i] == a && find(claimedCols.begin(), claimedCols.end(), i) == claimedCols.end()){
	// 		claimedCols.push_back(i);
	// 		//cout << "GOT IT" << i << '\n';
	// 	}
	// 	a -= 2;
	// }

	for (int i = 0; i < stateReference.size(); i++) {
		if (stateReference[i] == filledCols[i] && find(claimedCols.begin(), claimedCols.end(), i) == claimedCols.end())
			claimedCols.push_back(i);
	}

}

vector< pair<int,int> > Player::filterDice(vector< pair<int,int> > pairs){
	vector< pair<int,int> > combinations;

	if (currentCols.size() == 0 || currentCols.size() == 1){ //All combinations are legal
		for(int i = 0; i < pairs.size(); i++){
			combinations.push_back(pairs[i]);
		}
		

	}else if(currentCols.size() == 2){ 
		for(int i = 0; i < pairs.size(); i++){
			//As long as one number in the pair is currently in play:
			if(find(currentCols.begin(), currentCols.end(), pairs[i].first) != currentCols.end() ||
			   find(currentCols.begin(), currentCols.end(), pairs[i].second) != currentCols.end()){
				combinations.push_back(pairs[i]);
			//Else If neither in the pair is currently in play:
			}else if(pairs[i].first == pairs[i].second){ //Check if the pair is the same number, if it is it is playable:
				combinations.push_back(pairs[i]);
			}else{
				pair<int,int> option1 = make_pair(pairs[i].first, -1);
				pair<int,int> option2 = make_pair(pairs[i].second, -1);
				combinations.push_back(option1);
				combinations.push_back(option2);
			}
		}
	}else if( currentCols.size() == 3){
		for(int i = 0; i < pairs.size(); i++){
			if(find(currentCols.begin(), currentCols.end(), pairs[i].first) != currentCols.end() &&
			   find(currentCols.begin(), currentCols.end(), pairs[i].second) != currentCols.end()){
				combinations.push_back(pairs[i]);

			}
			else if(find(currentCols.begin(), currentCols.end(), pairs[i].first) != currentCols.end() &&
				find(currentCols.begin(), currentCols.end(), pairs[i].second) == currentCols.end() ){
				pair<int,int> option = make_pair(pairs[i].first, -1);
				combinations.push_back(option);

			}
			else if(find(currentCols.begin(), currentCols.end(), pairs[i].first) == currentCols.end() &&
				find(currentCols.begin(), currentCols.end(), pairs[i].second) != currentCols.end() ){
				pair<int,int> option = make_pair(-1, pairs[i].second);
				combinations.push_back(option);
			}
		}
	}
	return combinations;
}

void Player::startOver() {
	state.clear();
	state = vector<int>(11, 0);
	stateReference = state;
	currentCols.clear();
	claimedCols.clear();
}