#ifndef LEVEL_H
#define LEVEL_H

#include "sdl.h"
#include "global_var.h"
#include "background.h"
#include "texture.h"
#include "direction.h"

extern int amount_platforms;
extern const int amount_levels;
enum Level_number {
  LEVEL_1,
  LEVEL_2,
  LEVEL_3
};

enum Blocks {
  BLOCK_SPAWN_HERO = '@',
  BLOCK_SPAWN_SLIME = 'S',
  BLOCK_SPACE = ' ',
  BLOCK_GROUND = 'g',
  BLOCK_GROUND_UP = 'u',
  BLOCK_GROUND_DOWN = 'd',
  BLOCK_GROUND_LEFT = 'l',
  BLOCK_GROUND_RIGHT = 'r',
  BLOCK_GROUND_UP_LEFT = 'q',
  BLOCK_GROUND_UP_RIGHT = 'w',
  BLOCK_GROUND_DOWN_RIGHT = 's',
  BLOCK_GROUND_DOWN_LEFT = 'a',
  BLOCK_PLATFORM_BASE = 'p',
};

enum Platrorm_type {
  PLATFORM_INACTIVE,
  PLATFORM_BASE,
  PLATFORM_DISAPPEARING,
  PLATFORM_BREAKING
};

extern struct Level {
  Level_number number; 
  SDL_Point amount_blocks;
  char** map;
  Sprite blocks;
  int texture_size_edge_block;
  int real_size_edge_block;
} *level;

struct Platform {
  Platrorm_type type;
  SDL_FPoint coordinates;
  SDL_Rect hitbox;
  int amount_sprite;
  direction_movement direction;
  float speed;
  union {
    struct {
      double counter_time;
      double active_time;
      double inactive_time;
      int is_active;
    } disappearing;
    struct {
      int was_the_hero_standing_on_the_platform;
      double max_remaining_time;
      double remaining_time;
    } breaking;
  } special;
}; 

extern struct Platform *platforms;

void init_level(Level_number number);
void load_level(const char* load_file);
Level* malloc_level();
void de_init_level();
void updating_level();
void draw_level();
void init_platforms();
void de_init_platforms();
void updating_platforms();
void special_platform_action(Platform* platform);
void move_platform(Platform* platform);
void collision_platform_with_blocks(Platform* platform);
void draw_platforms();

#endif