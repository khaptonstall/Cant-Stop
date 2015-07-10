#include "players/cpu_player.h"

cpu_player::cpu_player(string log_path) {
	if (log_path == "") {
		is_logging = false;
		return;
	}

	is_logging = true;

	log_file.open(log_path, fstream::out | fstream::app);
	if (log_file.failbit) {
		is_logging = false;
		cout << "Failed to open file " << log_path << endl;
	}
}

cpu_player::~cpu_player() {
	log_file.close();
}

pair<int, int> cpu_player::select_dice(GameState* game_state,
		vector<pair<int, int> > rolled_pairs,
		Player* p, int selected_dice) {
	return select_dice_impl(game_state, rolled_pairs, p);
}

int cpu_player::select_decision(GameState* game_state,
		int selected_decision) {
	return select_decision_impl(game_state);
}