#ifndef ENEMY_BASE_H
#define ENEMY_BASE_H

#include "sdl.h"
#include "global_var.h"
#include "level.h"
#include "texture.h"
#include "direction.h"
#include "other_function.h"
#include "hero.h"

enum Enemy_type {
  ENEMY_INACTIVE = -1,
  ENEMY_SLIME,
  ENEMY_AMOUNT
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
  float current_speed_gravity;
  int is_standing;
  struct {
    Texture* current;
    int current_number_sprite;
    double sprite_time_counter;
  } texture;
};

extern struct Enemy_container {
  int amount_enemies;
  Enemy_base** enemies;
  Texture** textures;
} *enemy_container;

void malloc_enemy_container();
// void init_enemies(Level_number number);
void init_enemies();
void init_textures_enemies();
void malloc_texture_enemy(Enemy_type type, int amount_textures);
void de_init_enemies();
void updating_enemies();
void gravity_enemy(Enemy_base* enemy);
void collision_with_blocks_enemy(Enemy_base* enemy);
void draw_enemies();
void set_current_sprite_enemy(Enemy_base* enemy);
void enemy_death(Enemy_base* enemy);

#endif