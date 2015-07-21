#ifndef _INFLUENCE_PLAYER_H_
#define _INFLUENCE_PLAYER_H_

class GameState;

#include "players/cpu_player.h"
#include "dice_probability.h"

using namespace std;

class influence_player : public cpu_player {
private:
	dice_probability dice_p;

public:
	influence_player(string log_path = "", int delay = 0);
	~influence_player();

	pair<int, int> select_dice_impl(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice = 0);
	int select_decision_impl(GameState* game_state, int selected_decision = 0);

	void start_over_impl();

	void revert_impl();
};

#endif /* _PROBABILITY_PLAYERz_H_ */