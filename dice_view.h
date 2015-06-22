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
	vector<string> dice_path;
	const char* dice_bg_path = "res/dice_bg.png";
	const char* dice_bad_path = "res/dice_bad.png";

	SDL_Surface** s_dice;
	SDL_Surface* s_dice_bg;
	SDL_Surface* s_dice_bad;
	SDL_Rect blocked_dice[6];


	SDL_Surface** s_dice_text;
	SDL_Color text_color = {255, 255, 255};

	SDL_Rect pair11, pair12, pair13, pair14, pair21, pair22, pair23, pair24, pair31, pair32, pair33, pair34;

	void generate_rect() {
		int height = 800;

		pair11.x = 72;
		pair11.y = 72;
		pair12.x = 310;
		pair12.y = 72;

		pair13.x = 632;
		pair13.y = 72;
		pair14.x = 873;
		pair14.y = 72;

		pair21.x = 72;
		pair21.y = 312;
		pair22.x = 312;
		pair22.y = 312;

		pair23.x = 632;
		pair23.y = 312;
		pair24.x = 873;
		pair24.y = 312;

		pair31.x = 72;
		pair31.y = 555;
		pair32.x = 312;
		pair32.y = 555;

		pair33.x = 632;
		pair33.y = 555;
		pair34.x = 873;
		pair34.y = 555;

		pair11.w = pair12.w = pair13.w = pair14.w = pair21.w = pair22.w = pair23.w = pair24.w = pair31.w = pair32.w = pair33.w = pair34.w = 170;
		pair11.h = pair12.h = pair13.h = pair14.h = pair21.h = pair22.h = pair23.h = pair24.h = pair31.h = pair32.h = pair33.h = pair34.h = 170;

		blocked_dice[0].x = pair11.x;
		blocked_dice[0].y = pair11.y;
		blocked_dice[0].w = pair12.x + pair12.w;
		blocked_dice[0].h = pair12.y + pair12.h;

		blocked_dice[1].x = pair13.x;
		blocked_dice[1].y = pair13.y;
		blocked_dice[1].w = pair14.x + pair14.w;
		blocked_dice[1].h = pair14.y + pair14.h;

		blocked_dice[2].x = pair21.x;
		blocked_dice[2].y = pair21.y;
		blocked_dice[2].w = pair22.x + pair22.w;
		blocked_dice[2].h = pair22.y + pair22.h;

		blocked_dice[3].x = pair23.x;
		blocked_dice[3].y = pair23.y;
		blocked_dice[3].w = pair24.x + pair24.w;
		blocked_dice[3].h = pair24.y + pair24.h;

		blocked_dice[4].x = pair31.x;
		blocked_dice[4].y = pair31.y;
		blocked_dice[4].w = pair32.x + pair32.w;
		blocked_dice[4].h = pair32.y + pair32.h;

		blocked_dice[5].x = pair33.x;
		blocked_dice[5].y = pair33.y;
		blocked_dice[5].w = pair34.x + pair34.w;
		blocked_dice[5].h = pair34.y + pair34.h;
	}

public:
	dice_view() {
		s_dice = new SDL_Surface*[6];
		s_dice_text	= new SDL_Surface*[11];
		dice_path = vector<string>(6);
		for (int i = 0; i < 6; i++) {
			int temp = i + 1;
			dice_path[i] = "res/dice" + to_string(temp) + ".png";
			s_dice[i] = IMG_Load(dice_path[i].c_str());
		}

		TTF_Font* lucida_console;
		TTF_Font* lucida_console_bg;
		lucida_console = TTF_OpenFont("res/lucida-console.ttf", 116);
		lucida_console_bg = TTF_OpenFont("res/lucida-console.ttf", 100);


		for (int i = 0; i < 11; i++) {
			SDL_Surface* s_temp_white = TTF_RenderText_Solid(lucida_console, to_string(i+2).c_str(), {255,255,255});
			SDL_Surface* s_temp_black = TTF_RenderText_Solid(lucida_console_bg, to_string(i+2).c_str(), {0,0,0});

			SDL_Rect r_temp_text;
			r_temp_text.x = ((0 + s_temp_black->w) / 2) - (s_temp_white->w / 2);
			r_temp_text.y = ((0 + s_temp_black->h) / 2) - (s_temp_white->h / 2);
			r_temp_text.w = s_temp_white->w;
			r_temp_text.h = s_temp_white->h;

			s_dice_text[i] = s_temp_black;
			//mSDL_BlitSurface(s_temp_white, NULL, s_dice_text[i], &r_temp_text);
			SDL_FreeSurface(s_temp_white);
		}

		s_dice_bg = IMG_Load(dice_bg_path);
		s_dice_bad = IMG_Load(dice_bad_path);
		generate_rect();

	}

	SDL_Surface* get_surface(vector<int> rolls, vector<bool>* bad_dice = nullptr) {
		SDL_Surface* output;
		output = SDL_ConvertSurface(s_dice_bg, s_dice_bg->format, s_dice_bg->flags);

		int roll1, roll2, roll3, roll4;
		roll1 = rolls[0];
		roll2 = rolls[1];
		roll3 = rolls[2];
		roll4 = rolls[3];

		SDL_Rect text_dst;

		SDL_BlitSurface(s_dice[roll1 - 1], NULL, output, &pair11);
		SDL_BlitSurface(s_dice[roll2 - 1], NULL, output, &pair12);

		if ((*bad_dice)[0] == false) {
			text_dst.x = ((pair11.x + pair11.w + pair12.x) / 2) - (s_dice_text[roll1 + roll2 - 2]->w / 2);
			text_dst.y = ((pair11.y + pair12.y + pair12.w) / 2) - (s_dice_text[roll1 + roll2 - 2]->h / 2);
			SDL_BlitSurface(s_dice_text[roll1 + roll2 - 2], NULL, output, &text_dst);
		}
		
		SDL_BlitSurface(s_dice[roll3 - 1], NULL, output, &pair13);
		SDL_BlitSurface(s_dice[roll4 - 1], NULL, output, &pair14);

		if ((*bad_dice)[1] == false) {
			text_dst.x = ((pair13.x + pair13.w + pair14.x) / 2) - (s_dice_text[roll3 + roll4 - 2]->w / 2);
			text_dst.y = ((pair13.y + pair14.y + pair14.w) / 2) - (s_dice_text[roll3 + roll4 - 2]->h / 2);
			SDL_BlitSurface(s_dice_text[roll3 + roll4 - 2], NULL, output, &text_dst);
		}

		SDL_BlitSurface(s_dice[roll1 - 1], NULL, output, &pair21);
		SDL_BlitSurface(s_dice[roll3 - 1], NULL, output, &pair22);

		if ((*bad_dice)[2] == false) {
			text_dst.x = ((pair21.x + pair21.w + pair22.x) / 2) - (s_dice_text[roll1 + roll3 - 2]->w / 2);
			text_dst.y = ((pair21.y + pair22.y + pair22.w) / 2) - (s_dice_text[roll1 + roll3 - 2]->h / 2);
			SDL_BlitSurface(s_dice_text[roll1 + roll3 - 2], NULL, output, &text_dst);
		}

		SDL_BlitSurface(s_dice[roll2 - 1], NULL, output, &pair23);
		SDL_BlitSurface(s_dice[roll4 - 1], NULL, output, &pair24);

		if ((*bad_dice)[3] == false) {
			text_dst.x = ((pair23.x + pair23.w + pair24.x) / 2) - (s_dice_text[roll2 + roll4 - 2]->w / 2);
			text_dst.y = ((pair23.y + pair24.y + pair24.w) / 2) - (s_dice_text[roll2 + roll4 - 2]->h / 2);
			SDL_BlitSurface(s_dice_text[roll2 + roll4 - 2], NULL, output, &text_dst);
		}

		SDL_BlitSurface(s_dice[roll1 - 1], NULL, output, &pair31);
		SDL_BlitSurface(s_dice[roll4 - 1], NULL, output, &pair32);

		if ((*bad_dice)[4] == false) {
			text_dst.x = ((pair31.x + pair31.w + pair32.x) / 2) - (s_dice_text[roll1 + roll4 - 2]->w / 2);
			text_dst.y = ((pair31.y + pair32.y + pair32.w) / 2) - (s_dice_text[roll1 + roll4 - 2]->h / 2);
			SDL_BlitSurface(s_dice_text[roll1 + roll4 - 2], NULL, output, &text_dst);
		}

		SDL_BlitSurface(s_dice[roll3 - 1], NULL, output, &pair33);
		SDL_BlitSurface(s_dice[roll2 - 1], NULL, output, &pair34);

		if ((*bad_dice)[5] == false) {
			text_dst.x = ((pair33.x + pair33.w + pair34.x) / 2) - (s_dice_text[roll3 + roll2 - 2]->w / 2);
			text_dst.y = ((pair33.y + pair34.y + pair34.w) / 2) - (s_dice_text[roll3 + roll2 - 2]->h / 2);
			SDL_BlitSurface(s_dice_text[roll3 + roll2 - 2], NULL, output, &text_dst);
		}


		for (int i = 0; i < bad_dice->size(); i++) {
			if ((*bad_dice)[i] == true) {
				SDL_BlitSurface(s_dice_bad, NULL, output, &blocked_dice[i]);
			}

		}

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
};

#endif /* _DICE_VIEW_H_ */