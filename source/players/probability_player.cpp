#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <SDL2/SDL_timer.h>

#include "GameState.h"
#include "players/probability_player.h"

#include "dice_probability.h"

using namespace std;

probability_player::probability_player(string log_path, int delay)
	: cpu_player(log_path, delay) {
	
}

// Function: select_dice
// Input: GameState*, vector<pair<int,int> >, Player*, int
// Output: pair<int,int>
// Desciption: Currently picks the first valid pair of dice
pair<int, int> probability_player::select_dice_impl(GameState* game_state, vector<pair<int, int> > rolled_pairs, Player* p, int selected_dice) {
	// Check what columns player is on
	vector<int> tokens;
	for (int i = 0; i < 11; i++) {
		if (state[i] != stateReference[i] && state[i] != filledCols[i])
			tokens.push_back(i + 2);
	}

	vector<int> inProgressCols;
	for (int i = 0; i < 11; i++) {
		if (!(stateReference[i] == 0) and !(stateReference[i] == filledCols[i])) {
			inProgressCols.push_back(i + 2);
		}
	}

	// If dice options not any of your in progress columns pick most probable.
	bool diceMatchCol = false;
	for (pair<int,int> rp : rolled_pairs) {
		if (game_state->validatePair(rp.first, p)) {
			if(find(inProgressCols.begin(), inProgressCols.end(), rp.first) != inProgressCols.end()) {
				diceMatchCol = true;
			}
		}
	}

	// No dice pairs match with your currently progressed columns
	if (!diceMatchCol) {
		double highestProb = 0;
		pair<int,int> highestPair = make_pair(0,0);

		for (pair<int,int> rp : rolled_pairs) {
			if (game_state->validatePair(rp.first, rp.second, p)) {
				// vector<int> tmpCols = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
				// tmpCols[rp.first - 2] = 0;
				// tmpCols[rp.second - 2] = 0;
				// if (rp.first == rp.second and highestPair.first == highestPair.second) {
				// 	if (dice_p.get_probability(rp.first, rp.second, 0) < highestProb) {
				// 		highestProb = dice_p.get_probability(rp.first, rp.second, 0);
				// 		highestPair = rp;
				// 	}
				// } else if (rp.first == rp.second) {
				// 	highestProb = dice_p.get_probability(rp.first, rp.second, 0);
				// 	highestPair = rp;
				// } else {
					double tmpProb = dice_p.get_probability(rp.first, rp.second, 0);
					if (tmpProb > highestProb) {
						highestProb = tmpProb;
						highestPair = rp;
					}
				// }
			}
			else {
				if (game_state->validatePair(rp.first, p)) {
					double tmpProb = dice_p.get_probability(rp.first, 0, 0);
					if (tmpProb > highestProb) {
						highestProb = tmpProb;
						highestPair = make_pair(rp.first, -1);
					}
				}

				if (game_state->validatePair(rp.second, p)) {
					double tmpProb = dice_p.get_probability(rp.second, 0, 0);
					if (tmpProb > highestProb) {
						highestProb = tmpProb;
						highestPair = make_pair(rp.second, -1);
					}
				}
			}
		}
		// cout << "Choice based on probability." << endl;
		return highestPair;
	}

	// Some or all dice rolls match to your currently progressed columns
	if (tokens.size() < 3) {
		double optTravel = 0.0;
		pair<int,int> optPair = make_pair(0,0);

		for (pair<int,int> rp :rolled_pairs) {
			if (game_state->validatePair(rp.first, rp.second, p)) {
				double tmpTravel = (double)stateReference[rp.first - 2] / filledCols[rp.first - 2]
								+ (double)stateReference[rp.second - 2] / filledCols[rp.second - 2];
				if (tmpTravel > optTravel) {
					optTravel = tmpTravel;
					optPair = rp;
				}
			}
			else {
				if (game_state->validatePair(rp.first, p)) {
					double tmpTravel = (double)stateReference[rp.first - 2] / filledCols[rp.first - 2];
					if (tmpTravel > optTravel) {
						optTravel = tmpTravel;
						optPair = make_pair(rp.first, -1);
					}
				}

				if (game_state->validatePair(rp.second, p)) {
					double tmpTravel = (double)stateReference[rp.second - 2] / filledCols[rp.second - 2];
					if (tmpTravel > optTravel) {
						optTravel = tmpTravel;
						optPair = make_pair(rp.second, -1);
					}
				}
			}
		}
		// cout << "Choice based on progress of column." << endl;
		return optPair;
	}

	double optDistance = 2.0;
	double optTravel = 0.0;
	pair<int,int> optDice = make_pair(0,0);

	for (pair<int,int> rp : rolled_pairs) {
		if (game_state->validatePair(rp.first, rp.second, p)) {
			double tmpDistance = 0.0;
			if (rp.first == rp.second) {
				tmpDistance += (double)(filledCols[rp.first - 2] - (stateReference[rp.first - 2] + 2)) / filledCols[rp.first - 2];
			}
			else {
				tmpDistance += (double)(filledCols[rp.first - 2] - (stateReference[rp.first - 2] + 1)) / filledCols[rp.first - 2];
				tmpDistance += (double)(filledCols[rp.second - 2] - (stateReference[rp.second - 2] + 1)) / filledCols[rp.second - 2];
				tmpDistance /= 2;
			}

			if (tmpDistance < optDistance) {
				optDistance = tmpDistance;
				optTravel = 1.0 / filledCols[rp.first - 2] + 1.0 / filledCols[rp.second - 2];
				optDice = rp;
			}
			else if (tmpDistance == optDistance) {
				double tmpTravel = 1.0 / filledCols[rp.first - 2] + 1.0 / filledCols[rp.second - 2];
				if (tmpTravel > optTravel) {
					optDistance = tmpDistance;
					optTravel = tmpTravel;
					optDice = rp;
				}
			}
		}
		else {
			if (game_state->validatePair(rp.first, p)) {
				double tmpDistance;
				tmpDistance = (double)(filledCols[rp.first - 2] - (stateReference[rp.first - 2] + 1)) / filledCols[rp.first - 2];

				if (tmpDistance < optDistance) {
					optDistance = tmpDistance;
					optTravel = 1.0 / filledCols[rp.first - 2];
					optDice = make_pair(rp.first, -1);
				}
				else if (tmpDistance == optDistance) {
					double tmpTravel = 1.0 / filledCols[rp.first - 2];
					if (tmpTravel > optTravel) {
						optDistance = tmpDistance;
						optTravel = tmpTravel;
						optDice = make_pair(rp.first, -1);
					}
				}
			}

			if (game_state->validatePair(rp.second,p)) {
				double tmpDistance;
				tmpDistance = (double)(filledCols[rp.second - 2] - (stateReference[rp.second - 2] + 1)) / filledCols[rp.second - 2];

				if (tmpDistance < optDistance) {
					optDistance = tmpDistance;
					optTravel = 1.0 / filledCols[rp.second - 2];
					optDice = make_pair(rp.second, -1);
				}
				else if (tmpDistance == optDistance) {
					double tmpTravel = 1.0 / filledCols[rp.second - 2];
					if (tmpTravel > optTravel) {
						optDistance = tmpDistance;
						optTravel = tmpTravel;
						optDice = make_pair(rp.second, -1);
					}
				}
			}
		}
	}

	// cout << "Choice based on distance from top." << endl;
	if (optDistance != 2.0) return optDice;

	cout << "WTF" << endl;
	return make_pair(-1, -1);
}



// Function: select_decision
// Input: GameState*, int
// Output: int
// Desciption returning 1 = continue, returning 2 = stop
int probability_player::select_decision_impl(GameState* game_state, int selected_decision) {
	// if (game_state->canStop() == false){
	// 	return 1;
	// }

	vector<int> tokens;
	for (int i = 0; i < 11; i++) {
		if (state[i] != stateReference[i] && state[i] != filledCols[i])
			tokens.push_back(i + 2);
	}

	// Less than three tokens, continue
	if (tokens.size() < 3 && game_state->deadCols.size() == 0)
		return 1;

	for (int i = tokens.size(); i < 3; i++) {
		tokens.push_back(0);
	}

	// Stop iif you're at the top of a column and used all 3 tokens
	for (int i = 0; i < 11; i++) {
		if (stateReference[i] == game_state->filledCols[i] && find(currentCols.begin(), currentCols.end(), i+2) != currentCols.end() &&
			tokens.size() == 3)
			return 2;
	}

	// If outlook is positive, continue
	double progress = 0;
	for (int i = 0; i < 11; i++) {
		//progress += state[i];
		progress += (stateReference[i] - state[i]) / ((double)filledCols[i]);
	}

	double expected_progress = dice_p.get_expected_progress(tokens[0], tokens[1], tokens[2]);
	double successful_probability = dice_p.get_probability(tokens[0], tokens[1], tokens[2]);

	// cout << "DEBUG: " << tokens.size() << ", " << game_state->deadCols.size() << endl;
	// If deadCols > 0 and tokens < 3, successful probability is chance of getting a non-dead column
	if (find(tokens.begin(), tokens.end(), 0) != tokens.end() and game_state->deadCols.size() != 0) {
		vector<int> liveCols;
		for (int i = 2; i <= 12; i++) {
			if (find(game_state->deadCols.begin(), game_state->deadCols.end(), i) == game_state->deadCols.end()) {
				// cout << "Pushing on: " << i << endl;
				liveCols.push_back(i);
			}
		}
		successful_probability = dice_p.get_probability(liveCols);
		// cout << "There are dead columns and the new probability is: " << successful_probability << endl;
	}

	if (find(tokens.begin(), tokens.end(), 0) != tokens.end() and game_state->deadCols.size() != 0) {
		vector<int> liveCols;
		for (int i = 2; i <= 12; i++) {
			if (find(game_state->deadCols.begin(), game_state->deadCols.end(), i) == game_state->deadCols.end()) {
				// cout << "Pushing on: " << i << endl;
				liveCols.push_back(i);
			}
		}
		successful_probability = dice_p.get_probability(liveCols);
		// cout << "There are dead columns and the new probability is: " << successful_probability << endl;
	}

	vector<double> relative_eprogresses;
	for (int t : tokens) {
		if (t != 0)
			relative_eprogresses.push_back(expected_progress / filledCols[t - 2]);
	}

	expected_progress = 0;
	for (double rp : relative_eprogresses) {
		expected_progress += rp;
	}

	expected_progress /= relative_eprogresses.size();

	// cout << "g = " << expected_progress << endl;

	// cout << "p = " << successful_probability << endl;
	// cout << "h = " << progress << endl;
	double lhs = successful_probability * (progress + expected_progress);
	double rhs = progress;

	// cout << "p(h+g) = " << lhs << " and h = " << rhs << endl;
	//cout << "p(h+g) = " << lhs << " and h = " << rhs << endl;

	if (lhs >= rhs)
		return 1;

	// Otherwise, stop!
	return 2;
}

void probability_player::start_over_impl() {

}

void probability_player::revert_impl() {

}