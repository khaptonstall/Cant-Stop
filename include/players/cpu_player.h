#ifndef _CPU_PLAYER_H_
#define _CPU_PLAYER_H_

class GameState;
class Player;

#include <fstream>
#include <vector>
#include <iostream>

#include "players/Player.h"

using namespace std;

class cpu_player : public Player {
private:
	bool is_logging;

	fstream log_file;

	virtual pair<int, int> select_dice_impl(GameState* game_state,
			vector<pair<int, int> > rolled_pairs,
			Player* p, int selected_dice = 0) = 0;

	virtual int select_decision_impl(GameState* game_state,
			int selected_decision = 0) = 0;

public:
	cpu_player(string log_path = "");
	~cpu_player();

	pair<int, int> select_dice(GameState* game_state,
			vector<pair<int, int> > rolled_pairs,
			Player* p, int selected_dice = 0);

	int select_decision(GameState* game_state,
			int selected_decision = 0);
};

#endif /* _CPU_PLAYER_H_ */