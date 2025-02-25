#ifndef ENEMY_BASE_H
#define ENEMY_BASE_H

#include "sdl.h"
#include "global_var.h"
#include "level.h"
#include "texture.h"
#include "direction.h"
#include "other_function.h"

struct Enemy_textures {
  Texture slime;
};

enum Enemy_type {
  ENEMY_SLIME,
};

struct Enemy_base {
  void* full_enemy;
  float health;
  float damage;
  float speed;
  direction_movement direction;
  Enemy_type type;
  SDL_FPoint coordinates;
  SDL_Rect hitbox;
  int current_number_sprite;
  float current_speed_gravity;
  int is_standing;
};

extern struct Enemy_container {
  int amount_enemies;
  Enemy_base** enemies;
  Enemy_textures textures;
} *enemy_container;

void malloc_enemy_container();
// void init_enemies(Level_number number);
void init_enemies();
void de_init_enemies();
void updating_enemies();
void gravity_enemy(Enemy_base* enemy);
void collision_with_blocks_enemy(Enemy_base* enemy);
void draw_enemies();

#endif