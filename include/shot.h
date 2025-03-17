#ifndef SHOT_H
#define SHOT_H

#include "sdl.h"
#include "global_var.h"
#include "texture.h"
#include "level.h"
#include "other_function.h"

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
  Shot_state current_state;
  Shot_state prev_state;
  SDL_FPoint coordinates;
  SDL_Rect hitbox;
  float range;
  float speed;
  float damage;
  Shot* next;
  Shot* prev;
  double death_time;
  union {
    direction_movement ordinary;
    SDL_FPoint homing;
  } direction;
  struct {
    Texture* current;
    int current_number_sprite;
    double sprite_time_counter;
  } texture;
};

extern struct Shot_container {
  Texture** textures;
  Shot* head;
  Shot* tail;
} shots;

void init_shot_container();
void init_shot_textures();
void de_init_shot_textures();
void de_init_shot_container();
Shot* create_shot(Shot_type type, Shot_creator creator, SDL_FPoint coordinates, float range, float damage, direction_movement direction = DIRECTION_NONE);
void destroy_shot(Shot* shot);
void add_shot_in_shots_container(SDL_FPoint coordinates_spawn, Shot_creator creator_type, 
                                 Shot_type shot_type, direction_movement direction, float range, float damage);
void update_shots();
void update_range_shot(Shot* shot);
void set_current_sprite_shot(Shot* shot);
void set_current_texture_shot(Texture* texture);
void determine_current_texture_shot(Shot* shot);
void collision_shot_with_blocks(Shot* shot);
void collision_shot_with_platforms(Shot* shot);
void collision_shot_with_enemies(Shot* shot);
void collision_shot_with_hero(Shot* shot);
void death_shot(Shot* shot);

void move_ordinary_shot(Shot* shot);
void move_homing_shot(Shot* shot);
void determine_direction_homing_shot(Shot* current);
void render_copy_shot(Shot* shot);
void draw_shots();

#endif