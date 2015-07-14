#include "players/q1p_player.h"

#include <SDL2/SDL_timer.h>

#include "GameState.h"

#include <cctype>
#include <sstream>
#include <cmath>

const double DISCOUNT_FACTOR = 0.8;

q1p_player::q1p_player(string log_path, string learning_path, uint16_t delay)
	: cpu_player(log_path) {
	selection_delay = delay;
	timer = 0;
	last_ticks = 0;

	learning = true;

	learning_file.open(learning_path, fstream::in);
	if (learning_file.fail()) {
		learning = false;
		cout << "Failed to open file " << learning_path << endl;
	}
	else {
		boardstate read_bs;
		while (!learning_file.eof()) {
			string currline;
			getline(learning_file, currline);
			// cout << currline << endl;
			if (currline[0] == 'b') {
				read_bs.clear();
				istringstream iss(currline.substr(1));
				string currelement;
				while (getline(iss, currelement, ':')) {
					if (isdigit(currelement[0])) {
						// cout << currelement << ":";
						read_bs.push_back(stoi(currelement));
					}
				}
				// cout << endl;
			}
			else if (currline[0] == 'a') {
				// cout << currline << endl;
				istringstream iss(currline.substr(1));
				string currelement;
				getline(iss, currelement, ':');
				// First dice pair
				dicesums ds;
				ds.first = stoi(currelement);
				getline(iss, currelement, ':');
				// Second dice pair
				ds.second = stoi(currelement);
				getline(iss, currelement, ':');
				// Q value
				qvalue qv;
				qv = stof(currelement);
				qmap[read_bs][ds] = qv;
			}
		}
		learning_file.close();
	}

	learning_file.open(learning_path, fstream::out | fstream::trunc);
}

q1p_player::~q1p_player() {
	if (learning) {
		// for (string c : results) {
		// 	learning_file << c << endl;
		// }
		for (auto& kv : qmap) {
			learning_file << "b";
			for (int x : kv.first) {
				learning_file << x << ":";
			}
			learning_file << endl;
			for (auto& a : kv.second) {
				learning_file << "a" << a.first.first << ":" << a.first.second << ":";
				learning_file << a.second << endl;
			}
		}

		learning_file.close();
	}
}

pair<int, int> q1p_player::select_dice_impl(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice) {
	if (last_ticks == 0) {
		last_ticks = SDL_GetTicks();
		return make_pair(-1, -1);
	}
	else if (timer < selection_delay) {
		timer += SDL_GetTicks() - last_ticks;
		last_ticks = SDL_GetTicks();
		return make_pair(-1, -1);
	}
	else {
		timer = 0;
		last_ticks = 0;
	}

	boardstate bs(11, 0);
	int progress = 0;
	for (int i = 0; i < state.size(); i++) {
		bs[i] = stateReference[i] - state[i];
		progress += bs[i];
	}

	// REWARD: The more progress made

	int random_option;
	if (num_pairs != 0) {
		random_option = rand() % num_pairs;
	}
	else random_option = 0;
	for (pair<int, int> rp : rolled_pairs) {
		if (game_state->validatePair(rp.first, rp.second, p) and random_option == 0) {
			double reward = pow(progress + 2, 1.5) / 2;
			boardstate new_bs = bs;
			return rp;
		}
		else if (game_state->validatePair(rp.first, rp.second, p)) random_option--;
	}

	/////////

	num_pairs = 0;
	for (pair<int, int> rp : rolled_pairs) {
		if (game_state->validatePair(rp.first, p)) {
			num_pairs++;
		}
	}

	if (num_pairs != 0) {
		random_option = rand() % num_pairs;
	}
	else random_option = 0;
	for (pair<int, int> rp : rolled_pairs) {
		if (game_state->validatePair(rp.first, p) and random_option == 0) {
			return make_pair(rp.first, -1);
		}
		else if (game_state->validatePair(rp.first, p)) random_option--;
	}

	return make_pair(-1, -1);
}

int q1p_player::select_decision_impl(GameState* game_state, int selected_decision) {
	if (last_ticks == 0) {
		last_ticks = SDL_GetTicks();
		return 0;
	}
	else if (timer < selection_delay) {
		timer += SDL_GetTicks() - last_ticks;
		last_ticks = SDL_GetTicks();
		return 0;
	}
	else {
		timer = 0;
		last_ticks = 0;
	}

	boardstate bs(11, 0);
	for (int i = 0; i < state.size(); i++) {
		bs[i] = stateReference[i] - state[i];
	}

	// REWARD: The more progress made when stopping

	if (!game_state->canStop())
		return 1;
	else if (rand() % 2)
		return 1;
	else
		return 2;

}

void q1p_player::startOver() {
	// PUNISH or REWARD: For winning / losing the game
	if (learning) {
		// Reward
		if (claimedCols.size() >= 3)
			results.push_back("W");
		// Punish
		else
			results.push_back("L");
	}

	state.clear();
	state = vector<int>(11, 0);
	stateReference = state;
	currentCols.clear();
	claimedCols.clear();
}

void q1p_player::revert() {
	// PUNISH: For losing progress

	stateReference = state;
	currentCols.clear();
}