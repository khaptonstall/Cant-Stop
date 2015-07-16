#include "players/q1p_player.h"

#include <SDL2/SDL_timer.h>

#include "GameState.h"

#include <cctype>
#include <sstream>
#include <cmath>

const double q1p_player::LEARNING_RATE = 0.2;
const double q1p_player::DISCOUNT_FACTOR = 0.8;

q1p_player::q1p_player(string log_path, string learning_path, bool apply, uint16_t delay)
	: cpu_player(log_path) {
	selection_delay = delay;
	timer = 0;
	last_ticks = 0;

	apply_learning = apply;
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
	last_sums = dicesums(-3, -3);
}

q1p_player::~q1p_player() {
	if (!apply_learning) {
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
	if (!apply_learning) {
		int num_pairs = 0;
		for (pair<int, int> rp : rolled_pairs) {
			if (game_state->validatePair(rp.first, rp.second, p)) {
				num_pairs++;
			}
		}

		int random_option;
		if (num_pairs != 0) {
			random_option = rand() % num_pairs;
		}
		else random_option = 0;
		for (pair<int, int> rp : rolled_pairs) {
			if (game_state->validatePair(rp.first, rp.second, p) and random_option == 0) {
				double reward = pow(progress + 2, 1.25) / 2;
				boardstate new_bs = bs;
				new_bs[rp.first - 2] += 1;
				new_bs[rp.first - 2] += 1;
				double q_max;
				map<dicesums, qvalue> bs_actionmap = qmap[new_bs];
				q_max = (*(bs_actionmap.begin())).second;
				for (auto& kv : bs_actionmap) {
					if (kv.second > q_max)
						q_max = kv.second;
				}

				// cout << "selectdicedice: " << q_max << endl;
				if (q_max < -10000) q_max = 0;
				if (rp.first < rp.second) {
					if (last_sums.first != -3)
						qmap[bs][last_sums] += LEARNING_RATE * (reward + DISCOUNT_FACTOR * q_max - qmap[bs][last_sums]);
					qmap[bs][dicesums(rp.first, rp.second)] += LEARNING_RATE * (reward + DISCOUNT_FACTOR * q_max - qmap[new_bs][dicesums(rp.first, rp.second)]);
					last_sums = dicesums(rp.first, rp.second);
				}
				else {
					if (last_sums.first != -3)
						qmap[bs][last_sums] += LEARNING_RATE * (reward + DISCOUNT_FACTOR * q_max - qmap[bs][last_sums]);
					qmap[bs][dicesums(rp.second, rp.first)] += LEARNING_RATE * (reward + DISCOUNT_FACTOR * q_max - qmap[new_bs][dicesums(rp.second, rp.first)]);
					last_sums = dicesums(rp.second, rp.first);
				}


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
					double reward = pow(progress + 1, 1.25) / 2;
					boardstate new_bs = bs;
					new_bs[rp.first - 2] += 1;
					// new_bs[rp.first - 2] += 1;
					double q_max;
					map<dicesums, qvalue> bs_actionmap = qmap[new_bs];
					q_max = (*(bs_actionmap.begin())).second;
					for (auto& kv : bs_actionmap) {
						if (kv.second > q_max)
							q_max = kv.second;
					}

					// cout << "selectonedice: " << q_max << endl;
					if (q_max < -1000) q_max = 0;
					if (last_sums.first != -3)
						qmap[bs][last_sums] += LEARNING_RATE * (reward + DISCOUNT_FACTOR * q_max - qmap[bs][last_sums]);
					qmap[bs][dicesums(rp.first, -1)] += LEARNING_RATE * (reward + DISCOUNT_FACTOR * q_max - qmap[new_bs][dicesums(rp.first, -1)]);
					last_sums = dicesums(rp.first, -1);

				return make_pair(rp.first, -1);
			}
			else if (game_state->validatePair(rp.first, p)) random_option--;
		}
	}
	// Apply what was learned
	else {
		map<dicesums, qvalue> actionlist = qmap[bs];
		pair<dicesums, qvalue> bestaction = make_pair(dicesums(-2, -2), -1000);
		for (pair<int, int> rp : rolled_pairs) {
			if (game_state->validatePair(rp.first, rp.second, p)) {
				if (bestaction.first.first == -2) {
					if (rp.first < rp.second)
						bestaction = make_pair(rp, actionlist[rp]);
					else
						bestaction = make_pair(rp, actionlist[dicesums(rp.second, rp.first)]);
				}
				else {
					dicesums tmp_rp;
					if (rp.first > rp.second)
						tmp_rp = dicesums(rp.second, rp.first);
					else
						tmp_rp = dicesums(rp.first, rp.second);

					if (actionlist[tmp_rp] > bestaction.second)
						bestaction = make_pair(tmp_rp, actionlist[tmp_rp]);
				}
			}
		}
		if (bestaction.first.first != -2) return bestaction.first;

		for (pair<int, int> rp : rolled_pairs) {
			if (game_state->validatePair(rp.first, p)) {
				if (bestaction.first.first == -2) {
					bestaction = make_pair(dicesums(rp.first, -1), actionlist[make_pair(rp.first, -1)]);
				}
				else {
					if (actionlist[make_pair(rp.first, -1)] > bestaction.second)
						bestaction = make_pair(dicesums(rp.first, -1), actionlist[make_pair(rp.first, -1)]);
				}
			}
		}
		if (bestaction.first.first != -2) return bestaction.first;
	}

	// If somehow not in table do random
	cout << "Not in qmap, doing random" << endl;
	int num_pairs = 0;
	for (pair<int, int> rp : rolled_pairs) {
		if (game_state->validatePair(rp.first, rp.second, p)) {
			num_pairs++;
		}
	}

	int random_option;
	if (num_pairs != 0) {
		random_option = rand() % num_pairs;
	}
	else random_option = 0;
	for (pair<int, int> rp : rolled_pairs) {
		if (game_state->validatePair(rp.first, rp.second, p) and random_option == 0) {
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
	int progress = 0;
	for (int i = 0; i < state.size(); i++) {
		bs[i] = stateReference[i] - state[i];
		progress += bs[i];
	}

	// REWARD: The more progress made when stopping
	
	if (!apply_learning) {
		if (!game_state->canStop()) {
			return 1;
		}
		else if (rand() % 2) {
			// Continue
			last_sums = dicesums(-1, -1);
			return 1;
		}
		else {
			double reward = pow(progress + 2, 1.25) / 2;
			double q_max;
			map<dicesums, qvalue> bs_actionmap = qmap[bs];
			q_max = (*(bs_actionmap.begin())).second;
			for (auto& kv : bs_actionmap) {
				if (kv.second > q_max)
					q_max = kv.second;
			}

			// cout << "stopping: " << q_max << endl;
			if (q_max < -1000) q_max = 0;
			if (last_sums.first != -3)
				qmap[bs][last_sums] += LEARNING_RATE * (reward + DISCOUNT_FACTOR * q_max - qmap[bs][last_sums]);
			last_sums = dicesums(0, 0);
			return 2;
		}
	}
	else {
		map<dicesums, qvalue> actionlist = qmap[bs];
		if (!game_state->canStop()) {
			return 1;
		}
		else if (actionlist[dicesums(-1, -1)] == actionlist[dicesums(0, 0)]) {
			return rand() % 2 + 1;
		}
		else if (actionlist[dicesums(-1, -1)] > actionlist[dicesums(0, 0)]) {
			return 1;
		}
		else
			return 2;
	}

}

void q1p_player::startOver() {
	boardstate bs(11, 0);
	int progress = 0;
	for (int i = 0; i < state.size(); i++) {
		bs[i] = stateReference[i] - state[i];
		progress += bs[i];
	}

	// PUNISH or REWARD: For winning / losing the game
	if (!apply_learning) {
		// Reward
		if (claimedCols.size() >= 3) {
			results.push_back("W");
			double reward = 50;
			double q_max;
			map<dicesums, qvalue> bs_actionmap = qmap[bs];
			q_max = (*(bs_actionmap.begin())).second;
			for (auto& kv : bs_actionmap) {
				if (kv.second > q_max)
					q_max = kv.second;
			}
			// cout << "startoverwin: " << q_max << endl;
			if (q_max < -1000) q_max = 0;
			qmap[bs][last_sums] += LEARNING_RATE * (reward + DISCOUNT_FACTOR * q_max - qmap[bs][last_sums]);
		}
		// Punish
		else {
			results.push_back("L");

			double reward = -100;
			double q_max;
			map<dicesums, qvalue> bs_actionmap = qmap[bs];
			q_max = (*(bs_actionmap.begin())).second;
						cout << "startoverlose: " << q_max << endl;

			for (auto& kv : bs_actionmap) {
				if (kv.second > q_max)
					q_max = kv.second;
			}
			qmap[bs][last_sums] += LEARNING_RATE * (reward + DISCOUNT_FACTOR * q_max - qmap[bs][last_sums]);
		}
	}

	state.clear();
	state = vector<int>(11, 0);
	stateReference = state;
	currentCols.clear();
	claimedCols.clear();
}

void q1p_player::revert() {
	// PUNISH: For losing progress
	if (!apply_learning) {
		boardstate bs(11, 0);
		int progress = 0;
		for (int i = 0; i < state.size(); i++) {
			bs[i] = stateReference[i] - state[i];
			progress += bs[i];
		}

		double reward = -1 * pow(progress, 3) / 2;
		double q_max;
		map<dicesums, qvalue> bs_actionmap = qmap[bs];
		q_max = (*(bs_actionmap.begin())).second;
		for (auto& kv : bs_actionmap) {
			if (kv.second > q_max)
				q_max = kv.second;
		}

		qmap[bs][last_sums] += LEARNING_RATE * (reward + DISCOUNT_FACTOR * q_max - qmap[bs][last_sums]);
	}

	stateReference = state;
	currentCols.clear();
}