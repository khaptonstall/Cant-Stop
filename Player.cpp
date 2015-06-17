
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
using namespace std;

class Player{
public:
	// ********** Variables ************
	bool turn = false;
	vector<int> state = vector<int>(11, 0);
	vector<int> stateReference = state;
	vector<int> currentCols; //Player's current columns. MAX=3
	vector<int> claimedCols; //Columns the player has maxxed out


	// ************ Methods *************
	vector< pair<int,int> > rollDice();
	void chooseDice( pair<int, int> );
	void displayCombinations(vector< pair<int,int> >);
	void checkForWin();
	vector< pair<int,int> > filterDice(	vector< pair<int,int> > pairs);
};

/*
// Player::rollDice()
// Input: 
// Output: vector< pair<int,int> > combinations
// Description: (1) Get four rand dice rolls, 
//				(2) Check how many columns the player is currently occupying
//				(3) Add possible pairs to the combinations vector to display to player
vector< pair<int,int> > Player::rollDice(){
	vector< pair<int,int> > combinations;
	vector< pair<int,int> > pairs;

	int dice1 = rand() % 6 + 1;
	int dice2 = rand() % 6 + 1;
	int dice3 = rand() % 6 + 1;
	int dice4 = rand() % 6 + 1;
	cout << dice1 << " " << dice2 << " " << dice3 << " " << dice4 << '\n';

	pair<int,int> pair1 = make_pair((dice1 + dice2), (dice3 + dice4));
	pair<int,int> pair2 = make_pair((dice1 + dice3), (dice2 + dice4));
	pair<int,int> pair3 = make_pair((dice2 + dice3), (dice1 + dice4));

	pairs.push_back(pair1);
	pairs.push_back(pair2);
	pairs.push_back(pair3);


	if (currentCols.size() == 0 || currentCols.size() == 1){ //All combinations are legal
		combinations.push_back( pair1 );
		combinations.push_back( pair2 );
		combinations.push_back( pair3 );

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

*/
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
	int a = 3;
	for(int i = 0; i < ((stateReference.size() / 2) + 1); i++){
		//cout << stateReference[i] << '\n';
		if(stateReference[i] == a && find(claimedCols.begin(), claimedCols.end(), i) == claimedCols.end()){
			claimedCols.push_back(i);
			//cout << "GOT IT" << i << '\n';
		}
		a += 2;
	}
	//cout << "a = " << a << '\n';
	a = 11;
	for(int i = ((stateReference.size() /2) + 1); i < stateReference.size(); i++){
		//cout << stateReference[i] << '\n';
		if(stateReference[i] == a && find(claimedCols.begin(), claimedCols.end(), i) == claimedCols.end()){
			claimedCols.push_back(i);
			//cout << "GOT IT" << i << '\n';
		}
		a -= 2;
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

