#include "players/cpu_player.h"

#include "GameState.h"

cpu_player::cpu_player(string log_path) {
	if (log_path == "") {
		is_logging = false;
		return;
	}

	is_logging = true;

	log_file.open(log_path, fstream::out | fstream::app);
	if (log_file.fail()) {
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
	pair<int, int> output = select_dice_impl(game_state, rolled_pairs, p);

	if (is_logging and output != make_pair(-1, -1)) {
		for (int c : p->state) {
			log_file << c << " ";
		}
		log_file << ": ";
		log_file << output.first << " and " << output.second;
	}

	return output;
}

int cpu_player::select_decision(GameState* game_state,
		int selected_decision) {
	int output = select_decision_impl(game_state);

	if (is_logging and output > 0) {
		log_file << ": ";
		if (output == 1)
			log_file << "continue" << endl;
		else if (output == 2)
			log_file << "stop" << endl;
	}

	return output;
}