#ifndef _HUMAN_PLAYER_H_
#define _HUMAN_PLAYER_H_

class GameState;

#include "Player.h"

using namespace std;

class Human_Player : public Player {
public:
	pair<int, int> select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice = 0);
	int select_decision(GameState* game_state, int selected_decision = 0);
};

#endif /* _HUMAN_PLAYER_H_ */