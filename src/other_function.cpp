#include "../include/other_function.h"

void updating_mouse_state() {
	mouse_buttons_state = SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
}

void updating_dt() {
	static double last_time = 0;
	double new_time = SDL_GetTicks();
	dt = new_time - last_time;
	last_time = new_time;
}

float speed_dt(float speed) {
  return speed * dt / 1000;
}