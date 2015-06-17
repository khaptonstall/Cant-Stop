#ifndef _STOP_CONTROLLER_H_
#define _STOP_CONTROLLER_H_

#include <SDL2/SDL.h>

class stop_controller {
private:
	SDL_Rect r_continue;
	SDL_Rect r_stop;

public:
	stop_controller(stop_view sv, int offset_x, int offset_y, int scale) {
		r_continue = sv.get_rect(1);
		r_continue.x /= scale;
		r_continue.y /= scale;
		r_continue.x += offset_x / scale;
		r_continue.y += offset_y / scale;
		r_continue.w /= scale;
		r_continue.h /= scale;

		r_stop = sv.get_rect(2);
		r_stop.x /= scale;
		r_stop.y /= scale;
		r_stop.x += offset_x / scale;
		r_stop.y += offset_y / scale;
		r_stop.w /= scale;
		r_stop.h /= scale;
	}

	int input(int x, int y) {
		SDL_Point p = {x, y};
		if (SDL_EnclosePoints(&p, 1, &r_continue, NULL)) return 1;
		else if (SDL_EnclosePoints(&p, 1, &r_stop, NULL)) return 2;
		else return -1;
	}

};

#endif /* _STOP_CONTROLLER_H_ */