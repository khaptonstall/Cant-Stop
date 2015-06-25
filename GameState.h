#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include "Human_Player.h"
#include "AI_Player.h"
#include "probability_player.h"
#include "rollout_player.h"
using namespace std;

class GameState {
public:
	// ********** Variables ***********
	probability_player player1;
	rollout_player player2;
	vector<int> deadCols;
	static vector<int> const filledCols; // Vector to compare when a player is at the top of the column
	// ********** Methods ************
	GameState();
	vector< pair<int,int> > rollDice();
	void checkForDeadCols();
	vector<int> rollDice(bool b);
	bool validateDice(vector<int> d, Player* p);
	bool validatePair(int a, int b, Player* p);
	bool validatePair(int a, Player* p);
	void startOver();
	bool canStop();
};

#endif /* _GAME_STATE_H_ */
