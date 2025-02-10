#ifndef LEVEL_H
#define LEVEL_H

#include "sdl.h"
#include "global_var.h"
#include "background.h"
#include "texture.h"

extern const int amount_levels;
enum Level_number {
  LEVEL_1,
  LEVEL_2,
  LEVEL_3
};

enum Blocks {
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
};

extern struct Level {
  Level_number number; 
  SDL_Point amount_blocks;
  char** map;
  texture block;
  int texture_size_edge_block;
  int real_size_edge_block;
} *level;

void init_level(Level_number number);
Level* malloc_level();
void de_init_level();
void updating_level();
void draw_level();

#endif