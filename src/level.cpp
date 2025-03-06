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
  level->blocks.sprite = load_texture_from_file("../game_images/blocks/blocks_map_1.png", 
    &level->blocks.size);
  SDL_SetTextureBlendMode(level->blocks.sprite, SDL_BLENDMODE_BLEND);
  init_platforms();
}

void load_level(const char* load_file) {

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
  SDL_DestroyTexture(level->blocks.sprite);
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
        SDL_RenderCopy(renderer, level->blocks.sprite, &srcrect, &dstrect);
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
  Platform* platform = &platforms[amount_platforms];
  for (int i = 0; i < level->amount_blocks.y; ++i) {
    for (int j = 0; j < level->amount_blocks.x; ++j) {
      Blocks b_type = (Blocks)level->map[i][j];
      if (b_type == BLOCK_PLATFORM_BASE || 
          b_type == BLOCK_PLATFORM_DISAPPEARING ||
          b_type == BLOCK_PLATFORM_BREAKING) {
        if (is_prev_platform) { 
          platform->amount_sprite++;
          continue;
        }
        platform->amount_sprite = 1;
        platform->coordinates = { float(j * level->real_size_edge_block), float(i * level->real_size_edge_block) };
        platform->speed = 50;
          platform->hitbox = {
            int(platform->coordinates.x),
            int(platform->coordinates.y),
            level->real_size_edge_block,
            level->real_size_edge_block
          };
        platform->direction = DIRECTION_LEFT;
        is_prev_platform = 1;
        switch (b_type) {
          case BLOCK_PLATFORM_BASE: {
            platform->type = PLATFORM_BASE;
          } break;
          case BLOCK_PLATFORM_DISAPPEARING: {
            platform->type = PLATFORM_DISAPPEARING;
            platform->special.disappearing.counter_time = 0;
            platform->special.disappearing.active_time = 3000;
            platform->special.disappearing.inactive_time = 1000;
            platform->special.disappearing.is_active = 1;
          } break;
          case BLOCK_PLATFORM_BREAKING: {
            platform->type = PLATFORM_BREAKING;
            platform->special.breaking.max_remaining_time = 1500;
            platform->special.breaking.remaining_time = platform->special.breaking.max_remaining_time;
            platform->special.breaking.was_the_hero_standing_on_the_platform = 0;
          } break;
        }      
      } else if (is_prev_platform) {
        platform->hitbox.w *= platform->amount_sprite;
        is_prev_platform = 0;
        platform = &platforms[++amount_platforms];
      }

      // Blocks b_type = (Blocks)level->map[i][j];
      // if (b_type == BLOCK_PLATFORM_BASE) {
      //   if (is_prev_platform) {
      //     platform->amount_sprite++;
      //     continue;
      //   }
      //   platform->amount_sprite = 1;
      //   platform->coordinates = { float(j * level->real_size_edge_block), float(i * level->real_size_edge_block) };
      //   platform->speed = 50;
      //   platform->type = PLATFORM_BREAKING;
      //   platform->hitbox = {
      //     int(platform->coordinates.x),
      //     int(platform->coordinates.y),
      //     level->real_size_edge_block,
      //     level->real_size_edge_block
      //   };
      //   platform->direction = DIRECTION_LEFT;
      //   is_prev_platform = 1;
      //   switch (platform->type) {
      //     case PLATFORM_DISAPPEARING: {
      //       platform->special.disappearing.counter_time = 0;
      //       platform->special.disappearing.active_time = 3000;
      //       platform->special.disappearing.inactive_time = 1000;
      //       platform->special.disappearing.is_active = 1;
      //     } break;
      //     case PLATFORM_BREAKING: {
      //       platform->special.breaking.max_remaining_time = 1500;
      //       platform->special.breaking.remaining_time = platform->special.breaking.max_remaining_time;
      //       platform->special.breaking.was_the_hero_standing_on_the_platform = 0;
      //     } break;
      //   }
      // } else if (is_prev_platform) {
      //   platform->hitbox.w *= platform->amount_sprite;
      //   is_prev_platform = 0;
      //   platform = &platforms[++amount_platforms];
      // }
    }
  }
  platforms = (Platform*)realloc(platforms, sizeof(Platform) * amount_platforms);
}

void de_init_platforms() {
  free(platforms);
}

void updating_platforms() {
  for (int i = 0; i < amount_platforms; ++i) {
    if (platforms[i].type == PLATFORM_INACTIVE)
      continue;
    move_platform(&platforms[i]);
    collision_platform_with_blocks(&platforms[i]);
    special_platform_action(&platforms[i]);
  }
}

void special_platform_action(Platform* platform) {
  switch (platform->type) {
    case PLATFORM_BASE: {
      //Возможно, какое-то специальное действие с платформой, где type == PLATFORM_BASE
    } break;
    case PLATFORM_DISAPPEARING: {
      platform->special.disappearing.counter_time += dt;
      if (platform->special.disappearing.is_active) {
        if (platform->special.disappearing.counter_time > platform->special.disappearing.active_time) {
          platform->special.disappearing.is_active = 0;
          platform->special.disappearing.counter_time = 0;
        }
      } else {
        if (platform->special.disappearing.counter_time > platform->special.disappearing.inactive_time) {
          platform->special.disappearing.is_active = 1;
          platform->special.disappearing.counter_time = 0;
        }
      }
    } break;
    case PLATFORM_BREAKING: {
      if (platform->special.breaking.was_the_hero_standing_on_the_platform) {
        platform->special.breaking.remaining_time -= dt;
        if (platform->special.breaking.remaining_time <= 0)
          platform->type = PLATFORM_INACTIVE;
      }
    } break;
  }
}

void move_platform(Platform* platform) {
  if (platform->direction == DIRECTION_LEFT)
    platform->coordinates.x -= speed_dt(platform->speed);
  else
    platform->coordinates.x += speed_dt(platform->speed); 
  synchronize_hitbox_with_coordinates(&platform->hitbox, platform->coordinates);
}

void collision_platform_with_blocks(Platform* platform) {
  for (int i = 0; i < level->amount_blocks.y; ++i) {
    for (int j = 0; j < level->amount_blocks.x; ++j) {
      Blocks b_type = Blocks(level->map[i][j]);
      if (b_type == BLOCK_SPACE || b_type == BLOCK_PLATFORM_BASE || b_type == BLOCK_SPAWN_HERO ||
          b_type == BLOCK_SPAWN_SLIME || b_type == BLOCK_PLATFORM_BREAKING || b_type == BLOCK_PLATFORM_DISAPPEARING)
        continue;
      SDL_Rect hitbox_block = { level->real_size_edge_block * j, level->real_size_edge_block * i,
        level->real_size_edge_block, level->real_size_edge_block };
      switch (collision_of_two_objects(&platform->hitbox, &hitbox_block)) {
        case COLLISION_LEFT:
          platform->direction = DIRECTION_LEFT;
        break;
        case COLLISION_RIGHT:
          platform->direction = DIRECTION_RIGHT;
        break;
      }
    }
  }
  synchronize_hitbox_with_coordinates(&platform->hitbox, platform->coordinates);
}

void draw_platforms() {
  SDL_Rect srcrect = { 0, 0, level->texture_size_edge_block, level->texture_size_edge_block };
  srcrect.y = srcrect.h * 3;
  SDL_Rect dstrect = { 0, 0, level->real_size_edge_block, level->real_size_edge_block };
  for (int i = 0; i < amount_platforms; ++i) {
    Platform *platform = &platforms[i];
    dstrect.x = platform->coordinates.x;
    dstrect.y = platform->coordinates.y;
    switch (platform->type) {
      case PLATFORM_BASE: {

      } break;
      case PLATFORM_DISAPPEARING: {
      if (!platform->special.disappearing.is_active)
        SDL_SetTextureAlphaMod(level->blocks.sprite, 128);
      } break;
      case PLATFORM_BREAKING: {
        if (platform->special.breaking.was_the_hero_standing_on_the_platform) {
          SDL_SetTextureAlphaMod(level->blocks.sprite, 255 * 
            platform->special.breaking.remaining_time / platform->special.breaking.max_remaining_time);
        }
      } break;
      default: {
        continue;
      } 
    }
    if (platform->amount_sprite == 1) {
      srcrect.x = 3 * level->texture_size_edge_block;
      SDL_RenderCopy(renderer, level->blocks.sprite, &srcrect, &dstrect);
    } else {
      for (int j = 0; j < platform->amount_sprite; ++j) {
        if (j == 0)
          srcrect.x = 0;
        else if (j + 1 == platform->amount_sprite)
          srcrect.x = 2 * level->texture_size_edge_block;
        else
          srcrect.x = level->texture_size_edge_block;
        SDL_RenderCopy(renderer, level->blocks.sprite, &srcrect, &dstrect);
        dstrect.x += level->real_size_edge_block;
      }
    }
    SDL_SetTextureAlphaMod(level->blocks.sprite, 255);
  }
}