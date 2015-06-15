#ifndef _STOP_VIEW_H_
#define _STOP_VIEW_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

class stop_view {
private:
	SDL_Surface s_panel;
	const char* panel_path = "res/continue_stop.png";

	SDL_Rect r_continue;
	SDL_Rect r_stop;

	void generate_rect() {
		r_continue.x = 75;
		r_continue.y = 75;
		r_continue.w = 1040 - 75;
		r_continue.h = 360 - 75;

		r_stop.x = 75;
		r_stop.y = 435;
		r_stop.w = 1040 - 75;
		r_stop.h = 720 - 435;
	}

public:
	stop_view() {
		s_panel = IMG_Load(panel_path);
		generate_rect();
	}

	SDL_Surface* get_surface() {
		return s_panel;
	}

	SDL_Rect get_rect(int n) {
		SDL_Rect output;
		if (n == 0) {
			output.x = r_continue.x;
			output.y = r_continue.y;
			output.w = r_continue.w;
			output.h = r_continue.h;
			return output;
		}
		else {
			output.x = r_stop.x;
			output.y = r_stop.y;
			output.w = r_stop.w;
			output.h = r_stop.h;
			return output;
		}
	}

};

#endif /* _STOP_VIEW_H_ */