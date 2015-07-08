#ifndef _LOG_VIEW_H_
#define _LOG_VIEW_H_

#include <cstdint>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

class log_view {
private:
	const char* p_font;
	TTF_Font* font;

	SDL_Surface* s_text;
	SDL_Surface* s_bg;

	uint8_t text_size;

	uint16_t max_height;
	uint16_t max_lines;

	uint8_t current_line;


public:
	log_view(uint8_t maximum_lines);
	~log_view();

	void println(string text);

	void clear();

	SDL_Surface* get_surface();
};

#endif /* _LOG_VIEW_H_ */