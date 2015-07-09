#ifndef _DUMB_PLAYER_H_
#define _DUMB_PLAYER_H_

class GameState;

#include "players/Player.h"
#include "dice_probability.h"

using namespace std;

class dumb_player : public Player {
private:
	dice_probability dice_p;

	static int const SELECT_DELAY;
	int timer;
	int last_ticks;

public:
	dumb_player();
	pair<int, int> select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice = 0);
	int select_decision(GameState* game_state, int selected_decision = 0);
};

#endif /* _DUMB_PLAYER_H_ */