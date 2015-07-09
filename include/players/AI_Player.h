#ifndef _AI_PLAYER_H_
#define _AI_PLAYER_H_

class GameState;

#include "players/Player.h"

using namespace std;

class AI_Player : public Player {
private:

public:
	pair<int, int> select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice = 0);
	int select_decision(GameState* game_state, int selected_decision = 0);
};

#endif /* _AI_PLAYER_H_ */