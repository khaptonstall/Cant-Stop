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
	uint16_t selection_delay;
	uint16_t timer;
	uint16_t last_ticks;

	bool learning;

	fstream learning_file;
	vector<string> results;

	map<boardstate, map<dicesums, qvalue> > qmap;

	static const double DISCOUNT_FACTOR;

public:
	q1p_player(string log_path = "", string learning_path = "", uint16_t delay = 0);
	~q1p_player();

	pair<int, int> select_dice_impl(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice = 0);
	int select_decision_impl(GameState* game_state, int selected_decision = 0);

	void startOver();
	void revert();

};

#endif /* _Q1P_PLAYER_H_ */