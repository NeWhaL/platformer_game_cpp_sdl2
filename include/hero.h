#ifndef HERO_H
#define HERO_H

#include "sdl.h"
#include "global_var.h"
#include "application.h"
#include "texture.h"
#include "level.h"
#include <math.h>
#include "direction.h"

extern struct personage {
  SDL_Rect hitbox;
  SDL_FPoint coordinates;
  struct personage_textures {
    Texture idle;
  } textures;
  Texture* current_texture;
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
void set_current_sprite_hero(double time_one_frame);
void collision_with_blocks_hero();

#endif