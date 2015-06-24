#include <SDL2/SDL.h>

using namespace std;

class options_controller {
private:
	SDL_Rect r_newgame;

public:
	options_controller(options_view& ov, int offset_x, int offset_y, int scale) {
		r_newgame = ov.get_rect(1);
		r_newgame.x /= scale;
		r_newgame.y /= scale;
		r_newgame.x += offset_x / scale;
		r_newgame.y += offset_y / scale;
		r_newgame.w /= scale;
		r_newgame.h /= scale;

		ov.set_scaling_offset(offset_x, offset_y, scale);
	}

	int input(int x, int y) {
		SDL_Point p = {x, y};

		if (SDL_EnclosePoints(&p, 1, &r_newgame, NULL)) return 1;
		return 0;
	}

};