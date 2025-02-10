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

collision_direction collision_with_block(const SDL_Rect *hitbox_anyone, const SDL_Rect* hitbox_block) {
  // return (hitbox_anyone->x + hitbox_anyone->w >= hitbox_block->x &&
  //        !(hitbox_anyone->y >= hitbox_block->y + hitbox_block->h ||
  //        hitbox_anyone->y + hitbox_anyone->h <= hitbox_block->y)) &&
  //        (hitbox_anyone->x <= hitbox_block->x + hitbox_block->w &&
  //        !(hitbox_anyone->y >= hitbox_block->y + hitbox_block->h ||
  //        hitbox_anyone->y + hitbox_anyone->h <= hitbox_block->y));
  collision_direction collision = COLLISION_NONE;  
  if (hitbox_anyone->x + hitbox_anyone->w >= hitbox_block->x &&
		!(hitbox_anyone->y >= hitbox_block->y + hitbox_block->h ||
			hitbox_anyone->y + hitbox_anyone->h <= hitbox_block->y) &&
			hitbox_anyone->x < hitbox_block->x) {
		collision = COLLISION_LEFT;
  } 
	if (hitbox_anyone->x <= hitbox_block->x + hitbox_block->w &&
		!(hitbox_anyone->y >= hitbox_block->y + hitbox_block->h ||
			hitbox_anyone->y + hitbox_anyone->h <= hitbox_block->y) &&
			hitbox_anyone->x + hitbox_anyone->w > hitbox_block->x + hitbox_block->w) {
		collision = COLLISION_RIGHT;
  } 
	if (hitbox_anyone->y + hitbox_anyone->h >= hitbox_block->y &&
		!(hitbox_anyone->x >= hitbox_block->x + hitbox_block->w ||
			hitbox_anyone->x + hitbox_anyone->w <= hitbox_block->x) &&
			hitbox_anyone->y < hitbox_block->y) {
		collision = COLLISION_UP;
  }
	if (hitbox_anyone->y <= hitbox_block->y + hitbox_block->h &&
		!(hitbox_anyone->x >= hitbox_block->x + hitbox_block->w ||
			hitbox_anyone->x + hitbox_anyone->w <= hitbox_block->x) &&
			hitbox_anyone->y +hitbox_anyone->h > hitbox_block->y + hitbox_block->h) {
		collision = COLLISION_DOWN;
  }
  return collision;
}

void synchronize_hitbox_with_coordinates(SDL_Rect* hitbox, SDL_FPoint coordinates) {
  hitbox->x = (int)coordinates.x;
  hitbox->y = (int)coordinates.y;
}