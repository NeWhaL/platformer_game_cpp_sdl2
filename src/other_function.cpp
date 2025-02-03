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

SDL_Texture* load_texture_from_file(const char* filename, SDL_Rect* size) {
	SDL_Surface* surface = IMG_Load(filename);
	if (!surface) {
		printf("Не удалось загрузить изображение с именем %s!\nОшибка: %s", filename, SDL_GetError());
		de_init_application(1);
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (size)
		*size = { 0, 0, surface->w, surface->h };
	SDL_FreeSurface(surface);
	return texture;
}