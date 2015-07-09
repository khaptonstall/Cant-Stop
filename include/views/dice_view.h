#ifndef _DICE_VIEW_H_
#define _DICE_VIEW_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <stdlib.h>

using namespace std;

class dice_view {
private:
	vector<string> p_dice;
	const char* p_dice_bg 				= "res/dice_bg.png";
	const char* p_dice_good 			= "res/dice_good.png";
	const char* p_dice_bad 				= "res/dice_bad.png";
	const char* p_dice_highlight 		= "res/dice_highlight.png";
	const char* p_dice_good_wide 		= "res/dice_good_wide.png";
	const char* p_dice_bad_wide 		= "res/dice_bad_wide.png";
	const char* p_dice_highlight_wide 	= "res/dice_highlight_wide.png";

	SDL_Surface* s_dice[6];
	SDL_Surface* s_dice_bg;
	SDL_Surface* s_dice_good;
	SDL_Surface* s_dice_bad;
	SDL_Surface* s_dice_highlight;
	SDL_Surface* s_dice_good_wide;
	SDL_Surface* s_dice_bad_wide;
	SDL_Surface* s_dice_highlight_wide;

	SDL_Rect r_dice_frame[6];
	SDL_Rect r_dice_frame_wide[3];

	SDL_Surface* s_dice_text[11];
	SDL_Color color_white = {255, 255, 255};
	SDL_Color color_black = {0, 0, 0};

	SDL_Rect pair11, pair12, pair13, pair14, pair21, pair22, pair23, pair24, pair31, pair32, pair33, pair34;

	int offset_x, offset_y, scale;

	void generate_rect() {
		r_dice_frame[0] = {  10, 110, 90, 45 };
		r_dice_frame[1] = { 125, 110, 90, 45 };
		r_dice_frame[2] = {  10,  60, 90, 45 };
		r_dice_frame[3] = { 125,  60, 90, 45 };
		r_dice_frame[4] = {  10,  10, 90, 45 };
		r_dice_frame[5] = { 125,  10, 90, 45 };

		r_dice_frame_wide[0] = { 10, 110, 205, 45 };
		r_dice_frame_wide[1] = { 10,  60, 205, 45 };
		r_dice_frame_wide[2] = { 10,  10, 205, 45 };

		pair11 = {  15, 115, 35, 35 };
		pair12 = {  60, 115, 35, 35 };
		pair13 = { 130, 115, 35, 35 };
		pair14 = { 175, 115, 35, 35 };
		pair21 = {  15,  65, 35, 35 };
		pair22 = {  60,  65, 35, 35 };
		pair23 = { 130,  65, 35, 35 };
		pair24 = { 175,  65, 35, 35 };
		pair31 = {  15,  15, 35, 35 };
		pair32 = {  60,  15, 35, 35 };
		pair33 = { 130,  15, 35, 35 };
		pair34 = { 175,  15, 35, 35 };
	}

public:
	dice_view(const bool testing) {
		if (!testing)
			// Generate dice paths and load surfaces
			p_dice = vector<string>(6);
			for (int i = 0; i < 6; i++) {
				p_dice[i] = "res/dice" + to_string(i + 1) + ".png";
				s_dice[i] = IMG_Load(p_dice[i].c_str());
			}
			s_dice_bg = IMG_Load(p_dice_bg);
			s_dice_good = IMG_Load(p_dice_good);
			s_dice_bad = IMG_Load(p_dice_bad);
			s_dice_highlight = IMG_Load(p_dice_highlight);
			s_dice_good_wide = IMG_Load(p_dice_good_wide);
			s_dice_bad_wide = IMG_Load(p_dice_bad_wide);
			s_dice_highlight_wide = IMG_Load(p_dice_highlight_wide);

			TTF_Font* lucida_console;
			lucida_console = TTF_OpenFont("res/lucida-console.ttf", 24);

			for (int i = 0; i < 11; i++) {
				TTF_SetFontOutline(lucida_console, 2);
				s_dice_text[i] = TTF_RenderText_Solid(lucida_console, to_string(i+2).c_str(), {1,1,1});
				TTF_SetFontOutline(lucida_console, 0);
				SDL_Surface* s_temp = TTF_RenderText_Solid(lucida_console, to_string(i+2).c_str(), {255,255,255});

				SDL_Rect r_temp;
				r_temp.x = s_dice_text[i]->w / 2 - s_temp->w / 2;
				r_temp.y = (s_dice_text[i]->h / 2) - (s_temp->h / 2 - 2);
				r_temp.w = s_temp->w;
				r_temp.h = s_temp->h;

				SDL_BlitSurface(s_temp, NULL, s_dice_text[i], &r_temp);
				SDL_FreeSurface(s_temp);
			}

			generate_rect();

	}

	SDL_Surface* get_surface(vector<int> rolls, int mouse_x, int mouse_y, vector<bool>* bad_dice = nullptr, GameState* gs = nullptr, Player* pl = nullptr) {
		SDL_Surface* output;
		output = SDL_ConvertSurface(s_dice_bg, s_dice_bg->format, s_dice_bg->flags);

		mouse_x -= offset_x / scale;
		mouse_y -= offset_y / scale;
		mouse_x *= scale;
		mouse_y *= scale;

		SDL_Point p = { mouse_x, mouse_y };

		int roll1, roll2, roll3, roll4;
		roll1 = rolls[0];
		roll2 = rolls[1];
		roll3 = rolls[2];
		roll4 = rolls[3];

		SDL_Rect text_dst;

		if (gs->validatePair(roll1 + roll2, roll3 + roll4, pl)) {
			SDL_BlitSurface(s_dice_good_wide, NULL, output, &r_dice_frame_wide[0]);
			text_dst.x = ((pair11.x + pair11.w + pair12.x) / 2) - (s_dice_text[roll1 + roll2 - 2]->w / 2);
			text_dst.y = ((pair11.y + pair12.y + pair12.w) / 2) - (s_dice_text[roll1 + roll2 - 2]->h / 2);
			text_dst.w = s_dice_text[roll1 + roll2 - 2]->w;
			text_dst.h = s_dice_text[roll1 + roll2 - 2]->h;
			SDL_BlitSurface(s_dice_text[roll1 + roll2 - 2], NULL, output, &text_dst);
		}
		if (!(*bad_dice)[0] && !gs->validatePair(roll1 + roll2, roll3 + roll4, pl)) {
			SDL_BlitSurface(s_dice_good, NULL, output, &r_dice_frame[0]);
		}
		if (!(*bad_dice)[1] && !gs->validatePair(roll1 + roll2, roll3 + roll4, pl)) {
			SDL_BlitSurface(s_dice_good, NULL, output, &r_dice_frame[1]);
		}

		SDL_BlitSurface(s_dice[roll1 - 1], NULL, output, &pair11);
		SDL_BlitSurface(s_dice[roll2 - 1], NULL, output, &pair12);
		SDL_BlitSurface(s_dice[roll3 - 1], NULL, output, &pair13);
		SDL_BlitSurface(s_dice[roll4 - 1], NULL, output, &pair14);

		if (!(*bad_dice)[0] && !(*bad_dice)[1]) {
			text_dst.x = ((pair11.x + pair11.w + pair12.x) / 2) - (s_dice_text[roll1 + roll2 - 2]->w / 2);
			text_dst.y = ((pair11.y + pair12.y + pair12.w) / 2) - (s_dice_text[roll1 + roll2 - 2]->h / 2);
			text_dst.w = s_dice_text[roll1 + roll2 - 2]->w;
			text_dst.h = s_dice_text[roll1 + roll2 - 2]->h;
			SDL_BlitSurface(s_dice_text[roll1 + roll2 - 2], NULL, output, &text_dst);

			text_dst.x = ((pair13.x + pair13.w + pair14.x) / 2) - (s_dice_text[roll3 + roll4 - 2]->w / 2);
			text_dst.y = ((pair13.y + pair13.y + pair14.w) / 2) - (s_dice_text[roll3 + roll4 - 2]->h / 2);
			text_dst.w = s_dice_text[roll3 + roll4 - 2]->w;
			text_dst.h = s_dice_text[roll3 + roll4 - 2]->h;
			SDL_BlitSurface(s_dice_text[roll3 + roll4 - 2], NULL, output, &text_dst);
		}
		else if (!(*bad_dice)[0]) {
			text_dst.x = ((pair11.x + pair11.w + pair12.x) / 2) - (s_dice_text[roll1 + roll2 - 2]->w / 2);
			text_dst.y = ((pair11.y + pair12.y + pair12.w) / 2) - (s_dice_text[roll1 + roll2 - 2]->h / 2);
			text_dst.w = s_dice_text[roll1 + roll2 - 2]->w;
			text_dst.h = s_dice_text[roll1 + roll2 - 2]->h;
			SDL_BlitSurface(s_dice_text[roll1 + roll2 - 2], NULL, output, &text_dst);
		}
		else if (!(*bad_dice)[1]) {
			text_dst.x = ((pair13.x + pair13.w + pair14.x) / 2) - (s_dice_text[roll3 + roll4 - 2]->w / 2);
			text_dst.y = ((pair13.y + pair13.y + pair14.w) / 2) - (s_dice_text[roll3 + roll4 - 2]->h / 2);
			text_dst.w = s_dice_text[roll3 + roll4 - 2]->w;
			text_dst.h = s_dice_text[roll3 + roll4 - 2]->h;
			SDL_BlitSurface(s_dice_text[roll3 + roll4 - 2], NULL, output, &text_dst);
		}

		if ((*bad_dice)[0] && (*bad_dice)[1])
			SDL_BlitSurface(s_dice_bad_wide, NULL, output, &r_dice_frame_wide[0]);
		else if ((*bad_dice)[0])
			SDL_BlitSurface(s_dice_bad, NULL, output, &r_dice_frame[0]);
		else if ((*bad_dice)[1])
			SDL_BlitSurface(s_dice_bad, NULL, output, &r_dice_frame[1]);
		if (gs->validatePair(roll1 + roll2, roll3 + roll4, pl) && SDL_EnclosePoints(&p, 1, &r_dice_frame_wide[0], NULL))
			SDL_BlitSurface(s_dice_highlight_wide, NULL, output, &r_dice_frame_wide[0]);
		if (!(*bad_dice)[0] && !gs->validatePair(roll1 + roll2, roll3 + roll4, pl) && SDL_EnclosePoints(&p, 1, &r_dice_frame[0], NULL))
			SDL_BlitSurface(s_dice_highlight, NULL, output, &r_dice_frame[0]);
		else if (!(*bad_dice)[1] && !gs->validatePair(roll1 + roll2, roll3 + roll4, pl) && SDL_EnclosePoints(&p, 1, &r_dice_frame[1], NULL))
			SDL_BlitSurface(s_dice_highlight, NULL, output, &r_dice_frame[1]);

		// ===

		if (gs->validatePair(roll1 + roll3, roll2 + roll4, pl))
			SDL_BlitSurface(s_dice_good_wide, NULL, output, &r_dice_frame_wide[1]);
		if (!(*bad_dice)[2] && !gs->validatePair(roll1 + roll3, roll2 + roll4, pl))
			SDL_BlitSurface(s_dice_good, NULL, output, &r_dice_frame[2]);
		if (!(*bad_dice)[3] && !gs->validatePair(roll1 + roll3, roll2 + roll4, pl))
			SDL_BlitSurface(s_dice_good, NULL, output, &r_dice_frame[3]);

		SDL_BlitSurface(s_dice[roll1 - 1], NULL, output, &pair21);
		SDL_BlitSurface(s_dice[roll3 - 1], NULL, output, &pair22);
		SDL_BlitSurface(s_dice[roll2 - 1], NULL, output, &pair23);
		SDL_BlitSurface(s_dice[roll4 - 1], NULL, output, &pair24);

		if (!(*bad_dice)[2] && !(*bad_dice)[3]) {
			text_dst.x = ((pair21.x + pair21.w + pair22.x) / 2) - (s_dice_text[roll1 + roll3 - 2]->w / 2);
			text_dst.y = ((pair21.y + pair22.y + pair22.w) / 2) - (s_dice_text[roll1 + roll3 - 2]->h / 2);
			text_dst.w = s_dice_text[roll1 + roll3 - 2]->w;
			text_dst.h = s_dice_text[roll1 + roll3 - 2]->h;
			SDL_BlitSurface(s_dice_text[roll1 + roll3 - 2], NULL, output, &text_dst);
			
			text_dst.x = ((pair23.x + pair23.w + pair24.x) / 2) - (s_dice_text[roll2 + roll4 - 2]->w / 2);
			text_dst.y = ((pair23.y + pair23.y + pair24.w) / 2) - (s_dice_text[roll2 + roll4 - 2]->h / 2);
			text_dst.w = s_dice_text[roll2 + roll4 - 2]->w;
			text_dst.h = s_dice_text[roll2 + roll4 - 2]->h;
			SDL_BlitSurface(s_dice_text[roll2 + roll4 - 2], NULL, output, &text_dst);
		}
		else if (!(*bad_dice)[2]) {
			text_dst.x = ((pair21.x + pair21.w + pair22.x) / 2) - (s_dice_text[roll1 + roll3 - 2]->w / 2);
			text_dst.y = ((pair21.y + pair22.y + pair22.w) / 2) - (s_dice_text[roll1 + roll3 - 2]->h / 2);
			text_dst.w = s_dice_text[roll1 + roll3 - 2]->w;
			text_dst.h = s_dice_text[roll1 + roll3 - 2]->h;
			SDL_BlitSurface(s_dice_text[roll1 + roll3 - 2], NULL, output, &text_dst);
		}
		else if (!(*bad_dice)[3]) {
			text_dst.x = ((pair23.x + pair23.w + pair24.x) / 2) - (s_dice_text[roll2 + roll4 - 2]->w / 2);
			text_dst.y = ((pair23.y + pair23.y + pair24.w) / 2) - (s_dice_text[roll2 + roll4 - 2]->h / 2);
			text_dst.w = s_dice_text[roll2 + roll4 - 2]->w;
			text_dst.h = s_dice_text[roll2 + roll4 - 2]->h;
			SDL_BlitSurface(s_dice_text[roll2 + roll4 - 2], NULL, output, &text_dst);
		}

		if ((*bad_dice)[2] && (*bad_dice)[3])
			SDL_BlitSurface(s_dice_bad_wide, NULL, output, &r_dice_frame_wide[1]);
		else if ((*bad_dice)[2])
			SDL_BlitSurface(s_dice_bad, NULL, output, &r_dice_frame[2]);
		else if ((*bad_dice)[3])
			SDL_BlitSurface(s_dice_bad, NULL, output, &r_dice_frame[3]);
		if (gs->validatePair(roll1 + roll3, roll2 + roll4, pl) && SDL_EnclosePoints(&p, 1, &r_dice_frame_wide[1], NULL))
			SDL_BlitSurface(s_dice_highlight_wide, NULL, output, &r_dice_frame_wide[1]);
		if (!(*bad_dice)[2] && !gs->validatePair(roll1 + roll3, roll2 + roll4, pl) && SDL_EnclosePoints(&p, 1, &r_dice_frame[2], NULL))
			SDL_BlitSurface(s_dice_highlight, NULL, output, &r_dice_frame[2]);
		else if (!(*bad_dice)[3] && !gs->validatePair(roll1 + roll3, roll2 + roll4, pl) && SDL_EnclosePoints(&p, 1, &r_dice_frame[3], NULL))
			SDL_BlitSurface(s_dice_highlight, NULL, output, &r_dice_frame[3]);

		// ====

		if (gs->validatePair(roll1 + roll4, roll3 + roll2, pl))
			SDL_BlitSurface(s_dice_good_wide, NULL, output, &r_dice_frame_wide[2]);
		if (!(*bad_dice)[4] && !gs->validatePair(roll1 + roll4, roll3 + roll2, pl))
			SDL_BlitSurface(s_dice_good, NULL, output, &r_dice_frame[4]);
		if (!(*bad_dice)[5] && !gs->validatePair(roll1 + roll4, roll3 + roll2, pl))
			SDL_BlitSurface(s_dice_good, NULL, output, &r_dice_frame[5]);

		SDL_BlitSurface(s_dice[roll1 - 1], NULL, output, &pair31);
		SDL_BlitSurface(s_dice[roll4 - 1], NULL, output, &pair32);
		SDL_BlitSurface(s_dice[roll3 - 1], NULL, output, &pair33);
		SDL_BlitSurface(s_dice[roll2 - 1], NULL, output, &pair34);

		if (!(*bad_dice)[4] && !(*bad_dice)[5]) {
			text_dst.x = ((pair31.x + pair31.w + pair32.x) / 2) - (s_dice_text[roll1 + roll4 - 2]->w / 2);
			text_dst.y = ((pair31.y + pair32.y + pair32.w) / 2) - (s_dice_text[roll1 + roll4 - 2]->h / 2);
			text_dst.w = s_dice_text[roll1 + roll4 - 2]->w;
			text_dst.h = s_dice_text[roll1 + roll4 - 2]->h;
			SDL_BlitSurface(s_dice_text[roll1 + roll4 - 2], NULL, output, &text_dst);
			
			text_dst.x = ((pair33.x + pair33.w + pair34.x) / 2) - (s_dice_text[roll3 + roll2 - 2]->w / 2);
			text_dst.y = ((pair33.y + pair33.y + pair34.w) / 2) - (s_dice_text[roll3 + roll2 - 2]->h / 2);
			text_dst.w = s_dice_text[roll3 + roll2 - 2]->w;
			text_dst.h = s_dice_text[roll3 + roll2 - 2]->h;
			SDL_BlitSurface(s_dice_text[roll3 + roll2 - 2], NULL, output, &text_dst);
		}
		else if (!(*bad_dice)[4]) {
			text_dst.x = ((pair31.x + pair31.w + pair32.x) / 2) - (s_dice_text[roll1 + roll4 - 2]->w / 2);
			text_dst.y = ((pair31.y + pair32.y + pair32.w) / 2) - (s_dice_text[roll1 + roll4 - 2]->h / 2);
			text_dst.w = s_dice_text[roll1 + roll4 - 2]->w;
			text_dst.h = s_dice_text[roll1 + roll4 - 2]->h;
			SDL_BlitSurface(s_dice_text[roll1 + roll4 - 2], NULL, output, &text_dst);
		}
		else if (!(*bad_dice)[5]) {
			text_dst.x = ((pair33.x + pair33.w + pair34.x) / 2) - (s_dice_text[roll3 + roll2 - 2]->w / 2);
			text_dst.y = ((pair33.y + pair33.y + pair34.w) / 2) - (s_dice_text[roll3 + roll2 - 2]->h / 2);
			text_dst.w = s_dice_text[roll3 + roll2 - 2]->w;
			text_dst.h = s_dice_text[roll3 + roll2 - 2]->h;
			SDL_BlitSurface(s_dice_text[roll3 + roll2 - 2], NULL, output, &text_dst);
		}

		if ((*bad_dice)[4] && (*bad_dice)[5])
			SDL_BlitSurface(s_dice_bad_wide, NULL, output, &r_dice_frame_wide[2]);
		else if ((*bad_dice)[4])
			SDL_BlitSurface(s_dice_bad, NULL, output, &r_dice_frame[4]);
		else if ((*bad_dice)[5])
			SDL_BlitSurface(s_dice_bad, NULL, output, &r_dice_frame[5]);
		if (gs->validatePair(roll1 + roll4, roll3 + roll2, pl) && SDL_EnclosePoints(&p, 1, &r_dice_frame_wide[2], NULL))
			SDL_BlitSurface(s_dice_highlight_wide, NULL, output, &r_dice_frame_wide[2]);
		if (!(*bad_dice)[4] && !gs->validatePair(roll1 + roll4, roll3 + roll2, pl) && SDL_EnclosePoints(&p, 1, &r_dice_frame[4], NULL))
			SDL_BlitSurface(s_dice_highlight, NULL, output, &r_dice_frame[4]);
		else if (!(*bad_dice)[5] && !gs->validatePair(roll1 + roll4, roll3 + roll2, pl) && SDL_EnclosePoints(&p, 1, &r_dice_frame[5], NULL))
			SDL_BlitSurface(s_dice_highlight, NULL, output, &r_dice_frame[5]);

		return output;
	}

	SDL_Rect get_rect(int n) {
		SDL_Rect output;
		if (n == 1) {
			output.x = pair11.x;
			output.y = pair11.y;
			output.w = pair12.x - pair11.x + pair12.w;
			output.h = pair12.y - pair11.y + pair12.h;
			return output;
		}
		else if (n == 3) {
			output.x = pair21.x;
			output.y = pair21.y;
			output.w = pair22.x - pair21.x + pair22.w;
			output.h = pair22.y - pair21.y + pair22.h;
			return output;
		}
		else if (n == 5) {
			output.x = pair31.x;
			output.y = pair31.y;
			output.w = pair32.x - pair31.x + pair32.w;
			output.h = pair32.y - pair31.y + pair32.h;
			return output;
		}
		else if (n == 2) {
			output.x = pair13.x;
			output.y = pair13.y;
			output.w = pair14.x - pair13.x + pair14.w;
			output.h = pair14.y - pair13.y + pair14.h;
			return output;
		}
		else if (n == 4) {
			output.x = pair23.x;
			output.y = pair23.y;
			output.w = pair24.x - pair23.x + pair24.w;
			output.h = pair24.y - pair23.y + pair24.h;
			return output;
		}
		else if (n == 6) {
			output.x = pair33.x;
			output.y = pair33.y;
			output.w = pair34.x - pair33.x + pair34.w;
			output.h = pair34.y - pair33.y + pair34.h;
			return output;
		}
		else return output;
	}

	int get_width() {
		return s_dice_bg->w;
	}

	int get_height() {
		return s_dice_bg->h;
	}

	void set_scaling_offset(int x, int y, int s) {
		offset_x = x;
		offset_y = y;
		scale = s;
	}
};

#endif /* _DICE_VIEW_H_ */