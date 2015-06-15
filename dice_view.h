#ifndef _DICE_VIEW_H_
#define _DICE_VIEW_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

using namespace std;

class dice_view {
private:
	vector<string> dice_path;
	const char* dice_bg_path = "res/dice_bg.png";

	SDL_Surface** s_dice;
	SDL_Surface* s_dice_bg;

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
	}

public:
	dice_view() {
		s_dice = new SDL_Surface*[6];
		dice_path = vector<string>(6);
		for (int i = 0; i < 6; i++) {
			int temp = i + 1;
			dice_path[i] = "res/dice" + to_string(temp) + ".png";
			s_dice[i] = IMG_Load(dice_path[i].c_str());
		}

		s_dice_bg = IMG_Load(dice_bg_path);
		generate_rect();
	}

	SDL_Surface* get_surface(vector<int> rolls) {
		SDL_Surface* output;
		output = SDL_ConvertSurface(s_dice_bg, s_dice_bg->format, s_dice_bg->flags);

		int roll1, roll2, roll3, roll4;
		roll1 = rolls[0];
		roll2 = rolls[1];
		roll3 = rolls[2];
		roll4 = rolls[3];

		SDL_BlitSurface(s_dice[roll1 - 1], NULL, output, &pair11);
		SDL_BlitSurface(s_dice[roll2 - 1], NULL, output, &pair12);
		SDL_BlitSurface(s_dice[roll3 - 1], NULL, output, &pair13);
		SDL_BlitSurface(s_dice[roll4 - 1], NULL, output, &pair14);

		SDL_BlitSurface(s_dice[roll1 - 1], NULL, output, &pair21);
		SDL_BlitSurface(s_dice[roll3 - 1], NULL, output, &pair22);
		SDL_BlitSurface(s_dice[roll2 - 1], NULL, output, &pair23);
		SDL_BlitSurface(s_dice[roll4 - 1], NULL, output, &pair24);

		SDL_BlitSurface(s_dice[roll1 - 1], NULL, output, &pair31);
		SDL_BlitSurface(s_dice[roll4 - 1], NULL, output, &pair32);
		SDL_BlitSurface(s_dice[roll3 - 1], NULL, output, &pair33);
		SDL_BlitSurface(s_dice[roll2 - 1], NULL, output, &pair34);

		return output;
	}

	SDL_Rect get_rect(int n) {
		SDL_Rect output;
		if (n == 1) {
			output.x = pair11.x;
			output.y = pair11.y;
			output.w = pair14.x - pair11.x + pair14.w;
			output.h = pair14.y - pair11.y + pair14.h;
			return output;
		}
		else if (n == 2) {
			output.x = pair21.x;
			output.y = pair21.y;
			output.w = pair24.x - pair21.x + pair24.w;
			output.h = pair24.y - pair21.y + pair24.h;
			return output;
		}
		else {
			output.x = pair31.x;
			output.y = pair31.y;
			output.w = pair34.x - pair31.x + pair34.w;
			output.h = pair34.y - pair31.y + pair34.h;
			return output;
		}
	}
};

#endif /* _DICE_VIEW_H_ */