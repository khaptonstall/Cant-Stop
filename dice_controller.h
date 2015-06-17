#ifndef _DICE_CONTROLLER_H_
#define _DICE_CONTROLLER_H_

#include <SDL2/SDL.h>

using namespace std;

class dice_controller {
private:
	SDL_Rect dice_pair1;
	SDL_Rect dice_pair2;
	SDL_Rect dice_pair3;
	SDL_Rect dice_pair4;
	SDL_Rect dice_pair5;
	SDL_Rect dice_pair6;

public:
	dice_controller(dice_view dv, int offset_x, int offset_y, int scale) {
		dice_pair1 = dv.get_rect(1);
		dice_pair1.x /= scale;
		dice_pair1.y /= scale;
		dice_pair1.x += offset_x / scale;
		dice_pair1.y += offset_y;
		dice_pair1.w /= scale;
		dice_pair1.h /= scale;

		dice_pair2 = dv.get_rect(2);
		dice_pair2.x /= scale;
		dice_pair2.y /= scale;
		dice_pair2.x += offset_x / scale;
		dice_pair2.y += offset_y;
		dice_pair2.w /= scale;
		dice_pair2.h /= scale;

		dice_pair3 = dv.get_rect(3);
		dice_pair3.x /= scale;
		dice_pair3.y /= scale;
		dice_pair3.x += offset_x / scale;
		dice_pair3.y += offset_y;
		dice_pair3.w /= scale;
		dice_pair3.h /= scale;

		dice_pair4 = dv.get_rect(4);
		dice_pair4.x /= scale;
		dice_pair4.y /= scale;
		dice_pair4.x += offset_x / scale;
		dice_pair4.y += offset_y;
		dice_pair4.w /= scale;
		dice_pair4.h /= scale;

		dice_pair5 = dv.get_rect(5);
		dice_pair5.x /= scale;
		dice_pair5.y /= scale;
		dice_pair5.x += offset_x / scale;
		dice_pair5.y += offset_y;
		dice_pair5.w /= scale;
		dice_pair5.h /= scale;

		dice_pair6 = dv.get_rect(6);
		dice_pair6.x /= scale;
		dice_pair6.y /= scale;
		dice_pair6.x += offset_x / scale;
		dice_pair6.y += offset_y;
		dice_pair6.w /= scale;
		dice_pair6.h /= scale;
	}

	int input(int x, int y) {
		SDL_Point p = {x, y};
		if (SDL_EnclosePoints(&p, 1, &dice_pair1, NULL)) return 1;
		else if (SDL_EnclosePoints(&p, 1, &dice_pair2, NULL)) return 2;
		else if (SDL_EnclosePoints(&p, 1, &dice_pair3, NULL)) return 3;
		else if (SDL_EnclosePoints(&p, 1, &dice_pair4, NULL)) return 4;
		else if (SDL_EnclosePoints(&p, 1, &dice_pair5, NULL)) return 5;
		else if (SDL_EnclosePoints(&p, 1, &dice_pair6, NULL)) return 6;
		else return 0;
	}
};

#endif /* _DICE_CONTROLLER_H_ */ 