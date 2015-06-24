#ifndef _OPTIONS_VIEW_H_
#define _OPTIONS_VIEW_H_

#include <SDL2/SDL.h>

using namespace std;

class options_view {
private:
	SDL_Surface* s_options;
	SDL_Surface* s_highlight;
	const char* p_options = "res/options.png";
	const char* p_highlight = "res/stop_highlight.png";

	SDL_Rect r_newgame;

	int offset_x, offset_y, scale;

	void generate_rect() {
		r_newgame = { 15, s_options->h - 325 - s_highlight->h, 195, 60 };
	}

public:
	options_view() {
		s_options = IMG_Load(p_options);
		s_highlight = IMG_Load(p_highlight);

		generate_rect();
	}

	SDL_Surface* get_surface(int mouse_x, int mouse_y) {
		SDL_Surface* output = SDL_ConvertSurface(s_options, s_options->format, s_options->flags);

		mouse_x -= offset_x / scale;
		mouse_x *= scale;
		mouse_y -= offset_y / scale;
		mouse_y *= scale;

		SDL_Point p = { mouse_x, mouse_y };

		if (SDL_EnclosePoints(&p, 1, &r_newgame, NULL))
			SDL_BlitSurface(s_highlight, NULL, output, &r_newgame);

		return output;
	}

	SDL_Rect get_rect(int a) {
		if (a == 1) {
			return { r_newgame.x, r_newgame.y, r_newgame.w, r_newgame.h };
		}
		return { 0, 0, 0, 0 };
	}

	void set_scaling_offset(int x, int y, int s) {
		offset_x = x;
		offset_y = y;
		scale = s;
	}

};

#endif /* _OPTIONS_VIEW_H_ */