#ifndef SHOT_H
#define SHOT_H

#include "sdl.h"
#include "global_var.h"
#include "texture.h"

enum Shot_type {
  SHOT_TYPE_ORDINARY,
  SHOT_TYPE_HOMING,
  SHOT_TYPE_AMOUNT
};

enum Shot_creator {
  SHOT_CREATOR_HERO,
  SHOT_CREATOR_ENEMY
};

enum Shot_state {
  SHOT_STATE_MOVE,
  SHOT_STATE_DEATH,
  SHOT_STATE_AMOUNT
};

struct Shot {
  Shot_type type;
  Shot_creator creator;
  SDL_FPoint coordinates;
  SDL_Rect hitbox;
  direction_movement direction;
  float range;
  float speed;
  struct {
    Texture* current;
    int current_number_sprite;
    double sprite_time_counter;
  } texture;
};

extern struct Shot_container {
  Texture** textures;
  int capacity; //сколько выстрелов может находиться в контейнере
  int coefficient_capacity; //при перевыделении памяти нужно
  int amount_shots; //сколько выстрелов лежит в контейнере по факту
  Shot** all;
} shots;

void init_shot_container();
void init_shot_textures();
void de_init_shot_textures();
void de_init_shot_container();
Shot* create_shot(Shot_type type, Shot_creator creator, SDL_FPoint coordinates, float range, direction_movement direction = DIRECTION_NONE);
void destroy_shot(Shot* shot);
void add_shot_in_shots_container(SDL_FPoint coordinates_spawn, Shot_creator creator_type, 
                                 Shot_type shot_type, direction_movement direction, float range);

void move_ordinary_shot(Shot* shot);
void move_homing_shot(Shot* shot);
void render_copy_shot(Shot* shot);
void draw_shots();

#endif