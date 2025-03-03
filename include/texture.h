#ifndef TEXTURE_H
#define TEXTURE_H

#include "sdl.h"
#include "global_var.h"
#include "direction.h"
#include "other_function.h"

struct Sprite {
  SDL_Texture* sprite;
  SDL_Rect size;
  double rendering_time;
};

struct Texture {
  Sprite* sprites;
  int amount_sprite;
  int repeat_animation;
};

SDL_Texture* load_texture_from_file(const char* filename, SDL_Rect* size = NULL);
void init_texture(Texture* some_texture, const char* path_to_the_sprite_directory);
void de_init_texture(Texture* texture);
double time_for_one_texture_iteration(Texture* texture);
void set_current_sprite(Texture *texture, int *current_number_sprite, SDL_Rect *hitbox, SDL_FPoint *coordinates,
	direction_movement direction, double *sprite_time_counter, int texture_changed);
void hitbox_change_due_new_sprite(SDL_Rect* hitbox, direction_movement direction, Texture* current_texture, int current_number_sprite = 0,
		float* height_difference = NULL, float* width_difference = NULL);

#endif