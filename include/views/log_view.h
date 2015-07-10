#ifndef _LOG_VIEW_H_
#define _LOG_VIEW_H_

#include <cstdint>
#include <string>
#include <list>
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

	list<string> text;

	bool testing_mode;

public:
	log_view();
	void init(uint8_t maximum_lines, bool testing);
	~log_view();

	void println(string text);

	void clear();

	SDL_Surface* get_surface();
};

#endif /* _LOG_VIEW_H_ */