#ifndef HERO_H
#define HERO_H

#include "sdl.h"
#include "global_var.h"
#include "application.h"

enum direction_movement {
  DIRECTION_NONE,
  DIRECTION_LEFT,
  DIRECTION_RIGHT
};

struct personage_textures;

extern struct personage {
  SDL_Rect hitbox;
  SDL_FPoint coordinates;
  personage_textures* textures;
  direction_movement direction;
  float speed;
  int running;
  float health;
} *hero;

struct personage_textures {
  SDL_Texture* idle;
  SDL_Rect size_idle;
};

void init_hero();
void de_init_hero();
void draw_hero();
void update_hero();
void move_hero();

#endif