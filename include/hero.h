#ifndef HERO_H
#define HERO_H

#include "sdl.h"
#include "global_var.h"
#include "application.h"
#include "texture.h"
#include "level.h"
#include <math.h>

enum direction_movement {
  DIRECTION_NONE,
  DIRECTION_LEFT,
  DIRECTION_RIGHT
};

struct personage_textures {
  texture idle;
};

extern struct personage {
  SDL_Rect hitbox;
  SDL_FPoint coordinates;
  personage_textures textures;
  texture* current_texture;
  int current_idle;
  direction_movement direction;
  float speed;
  float coefficient_jerk;
  int running;
  float health;
  int is_standing;
  float current_speed_gravity;
  float jump_height;
  int is_jumping;
} *hero;

void init_hero();
void de_init_hero();
void draw_hero();
void update_hero();
void move_hero();
void gravity_hero();
void jump_hero();
void set_current_sprite_hero(double time_one_frame);
void collision_with_blocks_hero();

#endif