#ifndef _SIMPLE_PLAYER_H_
#define _SIMPLE_PLAYER_H_

class GameState;

#include "players/cpu_player.h"
#include "players/Player.h"

#include <vector>

using namespace std;

class simple_player : public cpu_player {
private:
	uint16_t selection_delay;
	uint16_t timer;
	uint16_t last_ticks;

public:
	simple_player(string log_path = "", uint16_t delay = 0);
	~simple_player();

	pair<int, int> select_dice_impl(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice = 0);
	int select_decision_impl(GameState* game_state, int selected_decision = 0);

};

#endif /* _SIMPLE_PLAYER_H_ */