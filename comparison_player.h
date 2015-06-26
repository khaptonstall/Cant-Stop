#ifndef _COMPARISON_PLAYER_H_
#define _COMPARISON_PLAYER_H_

class GameState;

#include "Player.h"
#include "dice_probability.h"

using namespace std;

class comparison_player : public Player {
private:
	dice_probability dice_p;

	static int const SELECT_DELAY;
	int timer;
	int last_ticks;

	int agreements;
	int disagreements;

public:
	comparison_player();
	pair<int, int> select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice = 0);
	int select_decision(GameState* game_state, int selected_decision = 0);
	bool rollOut(GameState*, Player*);
	int get_agreements();
	int get_disagreements();
};

#endif /* _COMPARISON_PLAYER_H_ */