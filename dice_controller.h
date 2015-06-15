#ifndef _DICE_CONTROLLER_H_
#define _DICE_CONTROLLER_H_

#include <SDL2/SDL.h>

using namespace std;

class dice_controller {
private:
	SDL_Rect dice_pair1;
	SDL_Rect dice_pair2;
	SDL_Rect dice_pair3;

public:
	dice_controller(dice_view dv, int offset, int scale) {
		dice_pair1 = dv.get_rect(1);
		dice_pair1.x /= scale;
		dice_pair1.y /= scale;
		dice_pair1.x += offset;
		//dice_pair1.y += offset;
		dice_pair1.w /= scale;
		dice_pair1.h /= scale;

		dice_pair2 = dv.get_rect(2);
		dice_pair2.x /= scale;
		dice_pair2.y /= scale;
		dice_pair2.x += offset;
		//dice_pair2.y += offset;
		dice_pair2.w /= scale;
		dice_pair2.h /= scale;

		dice_pair3 = dv.get_rect(3);
		dice_pair3.x /= scale;
		dice_pair3.y /= scale;
		dice_pair3.x += offset;
		//dice_pair3.y += offset;
		dice_pair3.w /= scale;
		dice_pair3.h /= scale;
	}

	int input(int x, int y) {
		SDL_Point p = {x, y};
		if (SDL_EnclosePoints(&p, 1, &dice_pair1, NULL)) return 1;
		else if (SDL_EnclosePoints(&p, 1, &dice_pair2, NULL)) return 2;
		else if (SDL_EnclosePoints(&p, 1, &dice_pair3, NULL)) return 3;
		else return -1;
	}
};

#endif /* _DICE_CONTROLLER_H_ */ 