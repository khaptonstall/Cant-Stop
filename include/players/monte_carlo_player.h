#ifndef _MONTE_CARLO_PLAYER_H_
#define _MONTE_CARLO_PLAYER_H_

class GameState;

#include "cpu_player.h"
#include "dice_probability.h"

using namespace std;
typedef pair<pair<int,int>,int> Node;

class monte_carlo_player : public cpu_player {
private:
	dice_probability dice_p;
	bool rollAgain;
	static int const SELECT_DELAY;
	int timer;
	int last_ticks;
public:
	monte_carlo_player();
	~monte_carlo_player();
 	pair<int, int> select_dice_impl(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice = 0);
	int select_decision_impl(GameState* game_state, int selected_decision = 0);
	Node MCTS(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p);
	vector<pair<int,int> > rollDice(GameState* g, Player* p);
	int findValue(GameState* game_state, Player* p, pair<int,int> roll);

	void start_over_impl();
	void revert_impl();
};

#endif /* _MONTE_CARLO_PLAYER_H_ */
