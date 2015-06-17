#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include "Human_Player.h"
#include "AI_Player.h"
using namespace std;

class GameState{
public:
	// ********** Variables ***********
	Human_Player player1;
	AI_Player player2;
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

#endif /* _GAME_STATE_H_ */