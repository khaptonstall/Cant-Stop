#include "log_view.h"

#include <iostream>

log_view::log_view() {
	// do nothing
}

void log_view::init(uint8_t maximum_lines) {
	text_size = 15;

	max_lines = maximum_lines;
	max_height = maximum_lines * text_size + 3;
	current_line = 0;

	font = TTF_OpenFont("res/Monaco-Linux.ttf", text_size);

	if (font == NULL) {
		cout << "Font load failed" << endl;
	}

	s_bg = SDL_CreateRGBSurface(
		0, 195, max_height,
		32, 0, 0, 0, 255
	);

	s_text = SDL_CreateRGBSurface(
		0, 195, max_height,
		32, 0, 0, 0, 0
	);
}

log_view::~log_view() {
	SDL_FreeSurface(s_bg);
	SDL_FreeSurface(s_text);
}

void log_view::println(string text) {
	SDL_Surface* tmp;
	SDL_Color white = {255, 255, 255};

	if (current_line == max_lines) {
		tmp = SDL_CreateRGBSurface(
			0, 195, max_height,
			32, 0, 0, 0, 0
		);

		SDL_Rect shift = {0, 15, 0, 0};
		shift.w = tmp->w;
		shift.h = tmp->h - text_size;

		SDL_Rect shift_dst = {0, 0, 0, 0};
		shift_dst.w = tmp->w;
		shift_dst.h = tmp->h - text_size;

		SDL_BlitSurface(s_text, &shift, tmp, &shift_dst);

		SDL_FreeSurface(s_text);
		s_text = tmp;
		current_line--;
	}

	tmp = TTF_RenderText_Solid(font, text.c_str(), white);
	SDL_Rect text_dst = {0, 0, 0, 0};
	text_dst.y = text_size * current_line;
	text_dst.w = tmp->w;
	text_dst.h = tmp->h;
	SDL_BlitSurface(tmp, NULL, s_text, &text_dst);

	SDL_FreeSurface(tmp);
	current_line++;
}

void log_view::clear() {
	SDL_FreeSurface(s_text);
	s_text = SDL_CreateRGBSurface(
		0, 195, max_height,
		32, 0, 0, 0, 0
	);

	current_line = 0;
}

SDL_Surface* log_view::get_surface() {
	SDL_Surface* output;

	output = SDL_ConvertSurface(
		s_bg,
		s_bg->format,
		s_bg->flags
	);

	SDL_Rect dst = {0, 0, s_text->w, s_text->h};

	SDL_BlitSurface(s_text, NULL, output, &dst);
	return output;
}