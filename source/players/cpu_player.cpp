#include "../../include/players/cpu_player.h"

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