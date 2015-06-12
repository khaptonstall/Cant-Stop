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

public:
	board_view() {
		s_board = IMG_Load(board_path.c_str());
		s_blue_token = IMG_Load(blue_path.c_str());
		s_red_token = IMG_Load(red_path.c_str());
		s_green_token = IMG_Load(green_path.c_str());
		s_yellow_token = IMG_Load(yellow_path.c_str());
		s_temp_token = IMG_Load(temp_path.c_str());


	}

	SDL_Surface* get_surface(vector<int> b, vector<int> r, vector<int> g, vector<int> y, vector<int> t) {
		SDL_Surface* output;

		output = SDL_ConvertSurface(s_board, s_board->format, s_board->flags);
		for (int i = 0; i < b.size(); i++) {
			if (b[i] != 0) {
				SDL_Rect dst;
				dst.w = s_blue_token->w;
				dst.h = s_blue_token->h;
				dst.x = 105 + (i * s_blue_token->w) + 32 * i;
				dst.y = s_board->h - 30 - (b[i] * s_blue_token->h + 32 * b[i]);

				SDL_BlitSurface(s_blue_token, NULL, output, &dst);
			}
		}

		for (int i = 0; i < r.size(); i++) {
			if (r[i] != 0) {
				SDL_Rect dst;
				dst.w = s_red_token->w;
				dst.h = s_red_token->h;
				dst.x = 105 + (i * s_red_token->w) + 32 * i;
				dst.y = s_board->h - 30 - (r[i] * s_red_token->h + 32 * r[i]);

				SDL_BlitSurface(s_red_token, NULL, output, &dst);
			}
		}

		for (int i = 0; i < g.size(); i++) {
			if (g[i] != 0) {
				SDL_Rect dst;
				dst.w = s_green_token->w;
				dst.h = s_green_token->h;
				dst.x = 105 + (i * s_green_token->w) + 32 * i;
				dst.y = s_board->h - 30 - (g[i] * s_green_token->h + 32 * g[i]);

				SDL_BlitSurface(s_green_token, NULL, output, &dst);
			}
		}

		for (int i = 0; i < y.size(); i++) {
			if (y[i] != 0) {
				SDL_Rect dst;
				dst.w = s_yellow_token->w;
				dst.h = s_yellow_token->h;
				dst.x = 105 + (i * s_yellow_token->w) + 32 * i;
				dst.y = s_board->h - 30 - (y[i] * s_yellow_token->h + 32 * y[i]);

				SDL_BlitSurface(s_yellow_token, NULL, output, &dst);
			}
		}

		return output;

	}

};