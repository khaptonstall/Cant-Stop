#ifndef _MONTE_CARLO_PLAYER_H_
#define _MONTE_CARLO_PLAYER_H_

class GameState;

#include "Player.h"
#include "dice_probability.h"

using namespace std;
typedef pair<pair<int,int>,int> Node;

class monte_carlo_player : public Player {
private:
	dice_probability dice_p;
	bool roll;
	static int const SELECT_DELAY;
	int timer;
	int last_ticks;
public:
	monte_carlo_player();
 	pair<int, int> select_dice(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice = 0);
	int select_decision(GameState* game_state, int selected_decision = 0);
	Node MCTS(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p);
	Node rollOut(GameState* game_state, Player* p, Node childNode, bool rollAgain);
	vector<pair<int,int> > rollDice(GameState* g, Player* p);
	int findValue(GameState* game_state, Player* p, pair<int,int> roll);

};

#endif /* _MONTE_CARLO_PLAYER_H_ */
