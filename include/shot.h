#ifndef SHOT_H
#define SHOT_H

#include "sdl.h"
#include "global_var.h"
#include "texture.h"

enum Shot_type {
  SHOT_TYPE_ORDINARY,
  SHOT_TYPE_HOMING
};

enum Shot_creator {
  SHOT_CREATOR_HERO,
  SHOT_CREATOR_ENEMY
};

struct Shot {
  Shot_type type;
  Shot_creator creator;
  SDL_FPoint coordinates;
  SDL_Rect hitbox;
  struct {
    Texture* current;
    int current_number_sprite;
    double sprite_time_counter;
  } texture;
};

extern struct Shot_container {
  Texture** textures;
  Shot** all;
} shots;

void init_shot_container();
void init_shot_textures();
void de_init_shot_textures();
void de_init_shot_container();
Shot* create_shot(Shot_type type, Shot_creator creator, SDL_FPoint coordinates);
void destroy_shot(Shot* shot);

void move_ordinary_shot(Shot* shot);
void move_homing_shot(Shot* shot);

#endif