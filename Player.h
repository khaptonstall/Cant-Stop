#ifndef _PLAYER_H_
#define _PLAYER_H_

class GameState;

using namespace std;

class Player{
public:
	// ********** Variables ************
	bool turn = false;
	vector<int> state = vector<int>(11, 0);
	vector<int> stateReference = state;
	vector<int> currentCols; //Player's current columns. MAX=3
	vector<int> claimedCols; //Columns the player has maxxed out
	string name = "";
	vector<int> filledCols = {3,5,7,9,11,13,11,9,7,5,3};


	// ************ Methods *************
	vector< pair<int,int> > rollDice();
	void chooseDice( pair<int, int> );
	void changeTurns();
	void displayCombinations(vector< pair<int,int> >);
	void checkForWin();
	vector< pair<int,int> > filterDice(	vector< pair<int,int> > pairs);

	virtual pair<int, int> select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice = 0) { return pair<int, int>(-1, -1); };
	virtual int select_decision(GameState* game_state, int selected_decision = 0) { return 0; };

	void startOver();
	//virtual pair<int, int> select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice = 0) { return pair<int, int>(-1, -1); };
	//virtual int select_decision(GameState* game_state, int selected_decision = 0, vector<pair<int, int> > rolled_pairs, Player* p) { return 0; };
};

#endif /* _PLAYER_H_ */