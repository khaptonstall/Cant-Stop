#ifndef _Q1P_PLAYER_H_
#define _Q1P_PLAYER_H_

class GameState;

#include "players/cpu_player.h"
#include "players/Player.h"

#include <vector>
#include <string>
#include <map>

using namespace std;

typedef vector<int> boardstate;
typedef pair<int, int> dicesums;
typedef double qvalue;
// typedef pair<dicesums, qvalue> action;

class q1p_player : public cpu_player {
private:
	bool learning;

	fstream learning_file;
	vector<string> results;

	bool apply_learning;

	map<boardstate, map<dicesums, qvalue> > qmap;

	static const double LEARNING_RATE;
	static const double DISCOUNT_FACTOR;

	dicesums last_sums;

public:
	q1p_player(string log_path = "", int delay = 0, string learning_path = "", bool apply = false);
	~q1p_player();

	pair<int, int> select_dice_impl(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice = 0);
	int select_decision_impl(GameState* game_state, int selected_decision = 0);

	void startOver();
	void revert();

};

#endif /* _Q1P_PLAYER_H_ */