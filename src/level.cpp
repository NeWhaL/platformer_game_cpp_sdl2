#include "../include/level.h"

const char* filename_maps[] = {
  "../maps/map_1.txt",
  "map_2.txt",
  "map_3.txt"
};

Platform* platforms = NULL;
int amount_platforms = 0;

const int amount_levels = 3;
Level *level;

void init_level(Level_number number) {
  level = malloc_level();
  level->number = number; 
  FILE* f = fopen(filename_maps[number], "r");
  if (f)
    for (int i = 0; fgets(level->map[i], level->amount_blocks.x, f) != NULL; ++i)
      fgetc(f);
  fclose(f);
  level->texture_size_edge_block = 16;
  level->real_size_edge_block = level->texture_size_edge_block * 2;
  level->blocks.texture = load_texture_from_file("../game_images/blocks/blocks_map_1.png", 
    &level->blocks.size);
  level->blocks.amount_sprite = level->blocks.size.w / level->texture_size_edge_block * 
    level->blocks.size.h / level->texture_size_edge_block;
  init_platforms();
}

Level* malloc_level() {
  Level* tmp_level = (Level*)malloc(sizeof(Level));
  int row = 34;
  int col = 61;
  tmp_level->amount_blocks = { col, row };
  tmp_level->map = (char**)malloc(sizeof(char*) * row);
  for (int i = 0; i < row; ++i)
    tmp_level->map[i] = (char*)malloc(sizeof(char) * col);
  return tmp_level; 
}

void de_init_level() {
  de_init_platforms();
  int row = level->amount_blocks.y;
  SDL_DestroyTexture(level->blocks.texture);
  for (int i = 0; i < row; ++i)
    free(level->map[i]);
  free(level->map);
  free(level);
}

void draw_level() {
  switch (level->number) {
    case LEVEL_1:
      draw_background(&backgrounds[BACKGROUND_LEVEL_1]);
    break;
    case LEVEL_2:
      draw_background(&backgrounds[BACKGROUND_LEVEL_2]);
    break;
    case LEVEL_3:
      draw_background(&backgrounds[BACKGROUND_LEVEL_3]);
    break;
  }
  SDL_Rect srcrect = { 0, 0, level->texture_size_edge_block, level->texture_size_edge_block };
  SDL_Rect dstrect = { 0, 0, level->real_size_edge_block, level->real_size_edge_block };
  int draw_it = 1;
  for (int i = 0; i < level->amount_blocks.y; ++i) {
    for (int j = 0; j < level->amount_blocks.x; ++j) {
      switch (level->map[i][j]) {
        case BLOCK_GROUND: {
          srcrect.x = srcrect.w;
          srcrect.y = srcrect.h;
        } break;
        case BLOCK_GROUND_UP: {
          srcrect.x = 5 * srcrect.w;
          srcrect.y = 0;
        } break;
        case BLOCK_GROUND_RIGHT: {
          srcrect.x = 6 * srcrect.w;
          srcrect.y = srcrect.h;
        } break;
        case BLOCK_GROUND_DOWN: {
          srcrect.x = 5 * srcrect.w;
          srcrect.y = 2 * srcrect.h;
        } break;
        case BLOCK_GROUND_LEFT: {
          srcrect.x = 4 * srcrect.w;
          srcrect.y = srcrect.h; 
        } break;
        case BLOCK_GROUND_UP_LEFT: {
          srcrect.x = 4 * srcrect.w;
          srcrect.y = 0;
        } break;
        case BLOCK_GROUND_UP_RIGHT: {
          srcrect.x = 6 * srcrect.w;
          srcrect.y = 0;
        } break;
        case BLOCK_GROUND_DOWN_LEFT: {
          srcrect.x = 4 * srcrect.w;
          srcrect.y = 2 * srcrect.h;
        } break;
        case BLOCK_GROUND_DOWN_RIGHT: {
          srcrect.x = 6 * srcrect.w;
          srcrect.y = 2 * srcrect.h;
        } break;
        default: {
          draw_it = 0;
        }
      }
      if (draw_it)
        SDL_RenderCopy(renderer, level->blocks.texture, &srcrect, &dstrect);
      draw_it = 1;
      dstrect.x += dstrect.w;
    }
    dstrect.x = 0;
    dstrect.y += dstrect.h;
  } 
  draw_platforms();
}

void updating_level() {
  updating_platforms();
}

void init_platforms() {
  amount_platforms = 100;
  platforms = (Platform*)malloc(sizeof(Platform) * amount_platforms);
  amount_platforms = 0;
  int is_prev_platform = 0;
  for (int i = 0; i < level->amount_blocks.y; ++i) {
    for (int j = 0; j < level->amount_blocks.x; ++j) {
      Blocks b_type = (Blocks)level->map[i][j];
      if (b_type == BLOCK_PLATFORM_BASE) {
        if (is_prev_platform) {
          platforms[amount_platforms].amount_sprite++;
          continue;
        }
        platforms[amount_platforms].amount_sprite = 1;
        platforms[amount_platforms].coordinates = { float(j * level->real_size_edge_block), float(i * level->real_size_edge_block) };
        platforms[amount_platforms].speed = 50;
        platforms[amount_platforms].type = PLATFORM_BASE;
        is_prev_platform = 1;
      } else if (is_prev_platform) {
        is_prev_platform = 0;
        amount_platforms++;
      }
    }
  }
  platforms = (Platform*)realloc(platforms, sizeof(Platform) * amount_platforms);
}

void de_init_platforms() {
  free(platforms);
}

void updating_platforms() { 
  int direction = 1;
  for (int k = 0; k < amount_platforms; ++k) {
    for (int i = 0; i < level->amount_blocks.y; ++i) {
      for (int j = 0; j < level->amount_blocks.x; ++j) {
        SDL_Rect hitbox_block = { level->real_size_edge_block * j, level->real_size_edge_block * i, 
          level->real_size_edge_block, level->real_size_edge_block };
        switch (collision_with_block(&platforms[i].hitbox, &hitbox_block)) {
          case COLLISION_LEFT:
            direction = 1;
          break;
          case COLLISION_RIGHT:
            direction = -1;
          break;
        }
      }
    }
    platforms[k].coordinates.x += direction * speed_dt(platforms[k].speed);
  }
}

void draw_platforms() {
  SDL_Rect srcrect = { 0, 0, level->texture_size_edge_block, level->texture_size_edge_block };
  srcrect.y = srcrect.h * 3;
  SDL_Rect dstrect = { 0, 0, level->real_size_edge_block, level->real_size_edge_block };
  for (int i = 0; i < amount_platforms; ++i) {
    dstrect.x = platforms[i].coordinates.x;
    dstrect.y = platforms[i].coordinates.y;
    if (platforms[i].amount_sprite == 1) {
      srcrect.x = 3 * level->texture_size_edge_block;
      SDL_RenderCopy(renderer, level->blocks.texture, &srcrect, &dstrect);
    } else {
      for (int j = 0; j < platforms[i].amount_sprite; ++j) {
        if (j == 0)
          srcrect.x = 0;
        else if (j + 1 == platforms[i].amount_sprite)
          srcrect.x = 2 * level->texture_size_edge_block;
        else
          srcrect.x = level->texture_size_edge_block;
        SDL_RenderCopy(renderer, level->blocks.texture, &srcrect, &dstrect);
        dstrect.x += level->real_size_edge_block;
      }
    }
  }
}