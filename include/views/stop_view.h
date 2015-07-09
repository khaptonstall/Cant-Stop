#ifndef _STOP_VIEW_H_
#define _STOP_VIEW_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

class stop_view {
private:
	SDL_Surface* s_panel;
	SDL_Surface* s_highlight;
	SDL_Surface* s_highlight_bad;
	const char* panel_path = "res/continue_stop.png";
	const char* highlight_path = "res/stop_highlight.png";
	const char* highlight_bad_path = "res/stop_highlight_bad.png";

	SDL_Rect r_continue;
	SDL_Rect r_stop;

	int offset_x;
	int offset_y;
	int scale;

	void generate_rect() {
		r_continue = { 15, 15, 195, 60 };
		r_stop = { 15, 90, 195, 60 };
	}

public:
	stop_view() {
		s_panel = IMG_Load(panel_path);
		s_highlight = IMG_Load(highlight_path);
		s_highlight_bad = IMG_Load(highlight_bad_path);
		generate_rect();
	}

	SDL_Surface* get_surface(int mouse_x, int mouse_y, bool canStop) {
		SDL_Surface* output = SDL_ConvertSurface(s_panel, s_panel->format, s_panel->flags);
		mouse_x -= offset_x / scale;
		mouse_x *= scale;

		mouse_y -= offset_y / scale;
		mouse_y *= scale;

		SDL_Point p = {mouse_x, mouse_y};

		if (!canStop) {
			SDL_BlitSurface(s_highlight_bad, NULL, output, &r_stop);
		}

		if (SDL_EnclosePoints(&p, 1, &r_continue, NULL)) {
			SDL_BlitSurface(s_highlight, NULL, output, &r_continue);
		}
		else if (canStop && SDL_EnclosePoints(&p, 1, &r_stop, NULL)) {
			SDL_BlitSurface(s_highlight, NULL, output, &r_stop);
		}

		return output;
	}

	SDL_Rect get_rect(int n) {
		SDL_Rect output;
		if (n == 2) {
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

	void set_scaling_offset(int x, int y, int s) {
		offset_x = x;
		offset_y = y;
		scale = s;
	}

};

#endif /* _STOP_VIEW_H_ */