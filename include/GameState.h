#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include "players/Human_Player.h"
#include "players/probability_player.h"
#include "players/rollout_player.h"
#include "players/simple_player.h"
#include "players/q1p_player.h"

using namespace std;

class GameState {
public:
	// ********** Variables ***********
	Player* player1;
	Player* player2;
	vector<int> deadCols;
	static vector<int> const filledCols; // Vector to compare when a player is at the top of the column
	// ********** Methods ************
	GameState();
	~GameState();
	vector< pair<int,int> > rollDice();
	void checkForDeadCols();
	vector<int> rollDice(bool b);
	bool validateDice(vector<int> d, Player* p);
	bool validatePair(int a, int b, Player* p);
	bool validatePair(int a, Player* p);
	bool checkSpace(int col, int prog);
	void startOver();
	bool canStop();
};

#endif /* _GAME_STATE_H_ */
