#ifndef _ROLLOUT_PLAYER_H_
#define _ROLLOUT_PLAYER_H_

class GameState;

#include "Player.h"
#include "dice_probability.h"

using namespace std;

class rollout_player : public Player {
private:
	dice_probability dice_p;

	const int SELECT_DELAY = 2000;
	int timer = 0;
	int last_ticks = 0;
public:
	//pair<int, int> select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice = 0);
	//int select_decision(GameState* game_state, int selected_decision, vector<pair<int, int> > rolled_pairs, Player* p);

 	pair<int, int> select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice = 0);
	int select_decision(GameState* game_state, int selected_decision = 0);

	bool rollOut(GameState* game_state, Player* p);
};

#endif /* _ROLLOUT_PLAYER_H_ */