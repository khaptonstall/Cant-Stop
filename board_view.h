#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

using namespace std;

class board_view {
private:
	string board_path = "res/board.png";
	string blue_path = "res/token_blue.png";
	string red_path = "res/token_red.png";
	string green_path = "res/token_green.png";
	string yellow_path = "res/token_yellow.png";
	string temp_path = "res/token_temp.png";

	SDL_Surface* s_board;
	SDL_Surface* s_blue_token;
	SDL_Surface* s_red_token;
	SDL_Surface* s_green_token;
	SDL_Surface* s_yellow_token;
	SDL_Surface* s_temp_token;

	SDL_Rect r_start[11];
	double board_spacing[11];

	void generate_rectangles() {
		r_start[0]  = {  25, 190, 30, 30 };
		r_start[1]  = {  65, 155, 30, 30 };
		r_start[2]  = { 105, 120, 30, 30 };
		r_start[3]  = { 145,  85, 30, 30 };
		r_start[4]  = { 185,  50, 30, 30 };
		r_start[5]  = { 225,  15, 30, 30 };
		r_start[6]  = { 265,  50, 30, 30 };
		r_start[7]  = { 305,  85, 30, 30 };
		r_start[8]  = { 345, 120, 30, 30 };
		r_start[9]  = { 385, 155, 30, 30 };
		r_start[10] = { 425, 190, 30, 30 };

		board_spacing[0]  = 255   - 190;
		board_spacing[1]  = 205   - 155;
		board_spacing[2]  = 165   - 120;
		board_spacing[3]  = 127.5 -  85;
		board_spacing[4]  =  91   -  50;
		board_spacing[5]  =  55   -  15;
		board_spacing[6]  =  91   -  50;
		board_spacing[7]  = 127.5 -  85;
		board_spacing[8]  = 165   - 120;
		board_spacing[9]  = 205   - 155;
		board_spacing[10] = 255   - 190;
	}

public:
	board_view() {
		s_board = IMG_Load(board_path.c_str());
		s_blue_token = IMG_Load(blue_path.c_str());
		s_red_token = IMG_Load(red_path.c_str());
		s_green_token = IMG_Load(green_path.c_str());
		s_yellow_token = IMG_Load(yellow_path.c_str());
		s_temp_token = IMG_Load(temp_path.c_str());

		generate_rectangles();
	}

	SDL_Surface* get_surface(vector<int> b, vector<int> r, vector<int> g, vector<int> y, vector<int> t) {
		SDL_Surface* output;
		output = SDL_ConvertSurface(s_board, s_board->format, s_board->flags);

		for (int i = 0; i < b.size(); i++) {
			if (b[i] > 0) {
				SDL_Rect dst = r_start[i];
				dst.y = s_board->h - dst.y - dst.h;
				dst.y -= board_spacing[i] * (b[i] - 1);
				SDL_BlitSurface(s_blue_token, NULL, output, &dst);
			}
		}

		for (int i = 0; i < r.size(); i++) {
			if (r[i] > 0) {
				SDL_Rect dst = r_start[i];
				dst.y = s_board->h - dst.y - dst.h;
				dst.y -= board_spacing[i] * (r[i] - 1);

				if (r[i] == b[i]) {
					dst.x -= 3;
					dst.y -= 3;
				}

				SDL_BlitSurface(s_red_token, NULL, output, &dst);
			}
		}

		for (int i = 0; i < t.size(); i++) {
			if (t[i] > 0) {
				SDL_Rect dst = r_start[i];
				dst.y = s_board->h - dst.y - dst.h;
				dst.y -= board_spacing[i] * (t[i] - 1);

				if (t[i] == b[i] || t[i] == r[i]) {
					dst.x -= 3;
					dst.y -= 3;
				}

				SDL_BlitSurface(s_temp_token, NULL, output, &dst);
			}
		}

		return output;

	}

	int get_width() {
		return s_board->w;
	}

	int get_height() {
		return s_board->h;
	}

};