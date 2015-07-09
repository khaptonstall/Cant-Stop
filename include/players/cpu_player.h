#ifndef _CPU_PLAYER_H_
#define _CPU_PLAYER_H_

#include <fstream>
#include <iostream>

using namespace std;

class cpu_player {
private:
	bool is_logging;

	fstream log_file;
public:
	cpu_player(string log_path = "");
	~cpu_player();
};

#endif /* _CPU_PLAYER_H_ */