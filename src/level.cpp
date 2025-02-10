#include "../include/level.h"

const char* filename_maps[] = {
  // "../maps/map_test.txt",
  "../maps/map_1.txt",
  "map_2.txt",
  "map_3.txt"
};

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
  level->block.texture = load_texture_from_file("../game_images/blocks/blocks_map_1.png", &level->block.size);
  level->block.amount_sprite = level->block.size.w / level->texture_size_edge_block * 
      level->block.size.h / level->texture_size_edge_block;
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
  int row = level->amount_blocks.y;
  SDL_DestroyTexture(level->block.texture);
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
          srcrect.x = level->texture_size_edge_block;
          srcrect.y = level->texture_size_edge_block;
        } break;
        case BLOCK_GROUND_UP: {
          srcrect.x = 5 * level->texture_size_edge_block;
          srcrect.y = 0;
        } break;
        case BLOCK_GROUND_RIGHT: {
          srcrect.x = 6 * level->texture_size_edge_block;
          srcrect.y = level->texture_size_edge_block;
        } break;
        case BLOCK_GROUND_DOWN: {
          srcrect.x = 5 * level->texture_size_edge_block;
          srcrect.y = 2 * level->texture_size_edge_block;
        } break;
        case BLOCK_GROUND_LEFT: {
          srcrect.x = 4 * level->texture_size_edge_block;
          srcrect.y = level->texture_size_edge_block; 
        } break;
        case BLOCK_GROUND_UP_LEFT: {
          srcrect.x = 4 * level->texture_size_edge_block;
          srcrect.y = 0;
        } break;
        case BLOCK_GROUND_UP_RIGHT: {
          srcrect.x = 6 * level->texture_size_edge_block;
          srcrect.y = 0;
        } break;
        case BLOCK_GROUND_DOWN_LEFT: {
          srcrect.x = 4 * level->texture_size_edge_block;
          srcrect.y = 2 * level->texture_size_edge_block;
        } break;
        case BLOCK_GROUND_DOWN_RIGHT: {
          srcrect.x = 6 * level->texture_size_edge_block;
          srcrect.y = 2 * level->texture_size_edge_block;
        } break;
        default: {
          draw_it = 0;
        }
      }
      if (draw_it)
        SDL_RenderCopy(renderer, level->block.texture, &srcrect, &dstrect);
      draw_it = 1;
      dstrect.x += level->real_size_edge_block;
    }
    dstrect.x = 0;
    dstrect.y += level->real_size_edge_block;
  }
  
}

void updating_level() {

}