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

collision_direction collision_of_two_objects(const SDL_Rect *hitbox_anyone, const SDL_Rect* hitbox_block) {
  if (hitbox_anyone->x + hitbox_anyone->w >= hitbox_block->x &&
		!(hitbox_anyone->y >= hitbox_block->y + hitbox_block->h ||
			hitbox_anyone->y + hitbox_anyone->h <= hitbox_block->y) &&
			hitbox_anyone->x < hitbox_block->x) {
		return COLLISION_LEFT;
  } 
	if (hitbox_anyone->x <= hitbox_block->x + hitbox_block->w &&
		!(hitbox_anyone->y >= hitbox_block->y + hitbox_block->h ||
			hitbox_anyone->y + hitbox_anyone->h <= hitbox_block->y) &&
			hitbox_anyone->x + hitbox_anyone->w > hitbox_block->x + hitbox_block->w) {
		return COLLISION_RIGHT;
  }
	if (hitbox_anyone->y + hitbox_anyone->h >= hitbox_block->y &&
		!(hitbox_anyone->x >= hitbox_block->x + hitbox_block->w ||
			hitbox_anyone->x + hitbox_anyone->w <= hitbox_block->x) &&
			hitbox_anyone->y < hitbox_block->y) {
		return COLLISION_UP;
  }
	if (hitbox_anyone->y <= hitbox_block->y + hitbox_block->h &&
		!(hitbox_anyone->x >= hitbox_block->x + hitbox_block->w ||
			hitbox_anyone->x + hitbox_anyone->w <= hitbox_block->x) &&
			hitbox_anyone->y +hitbox_anyone->h > hitbox_block->y + hitbox_block->h) {
		return COLLISION_DOWN;
  }
  return COLLISION_NONE;
}

void synchronize_hitbox_with_coordinates(SDL_Rect* hitbox, SDL_FPoint coordinates) {
  hitbox->x = (int)coordinates.x;
  hitbox->y = (int)coordinates.y;
}

void gravity(SDL_FPoint* coordinates) {
	coordinates->y += speed_dt(speed_gravity);
}

double read_from_file_double(FILE* file) {
	double data = 0.;
	fscanf(file, "%lf", &data);
	fgetc(file);
	return data;
}

float the_distance_between_the_centers_of_two_rect(SDL_Rect* rect_1, SDL_Rect* rect_2) {
  float distance_x = (rect_1->x + rect_1->w / 2.0) - (rect_2->x + rect_2->w / 2.0);
  float distance_y = (rect_1->y + rect_1->h / 2.0) - (rect_2->y + rect_2->h / 2.0);
  return sqrt(distance_x * distance_x + distance_y * distance_y);
}