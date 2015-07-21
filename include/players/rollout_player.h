#ifndef _ROLLOUT_PLAYER_H_
#define _ROLLOUT_PLAYER_H_

class GameState;

#include "players/cpu_player.h"
#include "dice_probability.h"

using namespace std;

class rollout_player : public cpu_player {
private:
	dice_probability dice_p;

public:
	rollout_player(string log_path = "", int delay = 0);
 	pair<int, int> select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice = 0);
	int select_decision(GameState* game_state, int selected_decision = 0);

	bool rollOut(GameState* game_state, Player* p);
};

#endif /* _ROLLOUT_PLAYER_H_ */
