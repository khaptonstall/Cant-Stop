#ifndef _OPTIONS_VIEW_H_
#define _OPTIONS_VIEW_H_

#include <SDL2/SDL.h>

using namespace std;

class options_view {
private:
	SDL_Surface* s_options;
	const char* p_options = "res/options.png";

public:
	options_view() {
		s_options = IMG_Load(p_options);
	}

	SDL_Surface* get_surface() {
		return SDL_ConvertSurface(s_options, s_options->format, s_options->flags);
	}

};

#endif /* _OPTIONS_VIEW_H_ */