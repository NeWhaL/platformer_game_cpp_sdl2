#include "../include/enemy_slime.h"

Enemy_container *enemy_container;

void malloc_enemy_container() {
  enemy_container = (Enemy_container*)malloc(sizeof(Enemy_container));
  enemy_container->amount_enemies = 0;
  enemy_container->enemies = NULL;
  for (int i = 0; i < level->amount_blocks.y; ++i) {
    for (int j = 0; j < level->amount_blocks.x; ++j) {
      Blocks block_spawn_enemy = Blocks(level->map[i][j]);
      if (block_spawn_enemy == BLOCK_SPAWN_SLIME)
        enemy_container->amount_enemies++; 
    }
  }
  enemy_container->enemies = (Enemy_base**)malloc(sizeof(Enemy_base*) * enemy_container->amount_enemies);
}

// void init_enemies(Level_number level_number) {
void init_enemies() {
  malloc_enemy_container();
  const char* slime_data_file = "../game_data/level_1/slime.txt";
  // switch (level_number) {
  //   case LEVEL_1: {
  //     slime_data_file = "../game_data/level_1/slime.txt";
  //   } break;
  //   case LEVEL_2: {
  //     slime_data_file = "../game_data/level_2/slime.txt";
  //   } break;
  //   case LEVEL_3: {
  //     slime_data_file = "../game_data/level_3/slime.txt";
  //   } break;
  // }

  //здесь проинициализировать все текстуры противников
  init_textures_enemies();
  // init_texture(&enemy_container->textures.slime, "../game_images/enemy/slime/");

  //Данные для каждого слайма
  Enemy_slime slime;
  FILE* f = fopen(slime_data_file, "r");
  if (f) {
    slime.base.health = read_from_file_double(f);
    slime.base.damage = read_from_file_double(f);
    slime.base.speed = read_from_file_double(f); 
  }
  fclose(f);
  int current_index_enemy = 0;
  for (int i = 0; i < level->amount_blocks.y; ++i) {
    for (int j = 0; j < level->amount_blocks.x; ++j) {
      Blocks block_spawn_enemy = Blocks(level->map[i][j]);
      Enemy_base* enemy = NULL;
      switch (block_spawn_enemy) {
        case BLOCK_SPAWN_SLIME: {
          Enemy_slime* new_slime = (Enemy_slime*)malloc(sizeof(Enemy_slime));
          *new_slime = slime;
          new_slime->base.full_enemy = new_slime;
          new_slime->base.type = ENEMY_SLIME;
          new_slime->current_state = ENEMY_SLIME_WALK;
          new_slime->prev_state = new_slime->current_state;
          new_slime->base.coordinates = { 
            float(level->real_size_edge_block * j),
            float(level->real_size_edge_block * i)
          };
          new_slime->base.hitbox.w = enemy_container->textures[new_slime->base.type][new_slime->current_state].sprites[0].size.w;
          new_slime->base.hitbox.h = enemy_container->textures[new_slime->base.type][new_slime->current_state].sprites[0].size.h;
          synchronize_hitbox_with_coordinates(&new_slime->base.hitbox, new_slime->base.coordinates);
          new_slime->base.direction = DIRECTION_LEFT;
          new_slime->base.texture.current_number_sprite = 0;
          new_slime->base.current_speed_gravity = 0;
          new_slime->base.is_standing = 0;
          new_slime->base.texture.sprite_time_counter = 0;
          new_slime->base.texture.current = &enemy_container->textures[new_slime->base.type][new_slime->current_state];
          enemy = &new_slime->base;
        } break;
      }
      if (enemy != NULL)
        enemy_container->enemies[current_index_enemy++] = enemy;
    }
  }
}

void malloc_texture_enemy(Enemy_type type, int amount_textures) {
  enemy_container->textures[type] = (Texture*)malloc(sizeof(Texture) * amount_textures);
}

void init_textures_enemies() {
  enemy_container->textures = (Texture**)malloc(sizeof(Texture*) * ENEMY_AMOUNT);
  malloc_texture_enemy(ENEMY_SLIME, ENEMY_SLIME_AMOUNT_STATE); 
  //также с остальными противниками
  init_texture(&enemy_container->textures[ENEMY_SLIME][ENEMY_SLIME_ATTACK], "../game_images/enemy/slime/attack/");
  init_texture(&enemy_container->textures[ENEMY_SLIME][ENEMY_SLIME_WALK], "../game_images/enemy/slime/walk/");
  init_texture(&enemy_container->textures[ENEMY_SLIME][ENEMY_SLIME_DEATH], "../game_images/enemy/slime/death/");
}

void de_init_enemies() {
  de_init_texture(&enemy_container->textures[ENEMY_SLIME][ENEMY_SLIME_WALK]);
  de_init_texture(&enemy_container->textures[ENEMY_SLIME][ENEMY_SLIME_ATTACK]);
  de_init_texture(&enemy_container->textures[ENEMY_SLIME][ENEMY_SLIME_DEATH]);
  for (int i = 0; i < ENEMY_AMOUNT; ++i)
    free(enemy_container->textures[i]);
  free(enemy_container->textures);

  for (int i = 0; i < enemy_container->amount_enemies; ++i) {
    Enemy_base* enemy = enemy_container->enemies[i];
    switch (enemy->type) {
      case ENEMY_SLIME: {
        Enemy_slime* slime = (Enemy_slime*)enemy->full_enemy;
        free(slime);
      } break;
    }
  }
  free(enemy_container->enemies);
  free(enemy_container);
}

void updating_enemies() {
  for (int i = 0; i < enemy_container->amount_enemies; ++i) {
    Enemy_base* enemy = enemy_container->enemies[i];
    gravity_enemy(enemy);
    switch (enemy->type) {
      case ENEMY_SLIME: {
        Enemy_slime* slime = (Enemy_slime*)enemy->full_enemy;
        if (slime->current_state == ENEMY_SLIME_DEATH) {
          collision_with_blocks_enemy(enemy);
          determine_current_texture_enemy_slime(slime);
          set_current_sprite_enemy(enemy);
          synchronize_hitbox_with_coordinates(&enemy->hitbox, enemy->coordinates);
          continue;
        }
        move_enemy_slime(slime);
        collision_enemy_slime_with_hero(slime);
        determine_current_texture_enemy_slime(slime);
      } break;
    }
    collision_with_blocks_enemy(enemy);
    set_current_sprite_enemy(enemy);
    synchronize_hitbox_with_coordinates(&enemy->hitbox, enemy->coordinates);
  }
}

void gravity_enemy(Enemy_base* enemy) {
  if (enemy->current_speed_gravity < speed_gravity)
    enemy->current_speed_gravity += speed_dt(speed_gravity);
  enemy->coordinates.y += speed_dt(enemy->current_speed_gravity); 
}

void collision_with_blocks_enemy(Enemy_base* enemy) {
  SDL_Rect position_block = { 0, 0, level->real_size_edge_block, level->real_size_edge_block };
  int is_enemy_standing = 0;
  for (int i = 0; i < level->amount_blocks.y; ++i) {
    for (int j = 0; j < level->amount_blocks.x; ++j) {
      Blocks b_type = Blocks(level->map[i][j]);
      if (b_type != BLOCK_SPACE && b_type != BLOCK_PLATFORM_BASE && b_type != BLOCK_SPAWN_HERO &&
          b_type != BLOCK_SPAWN_SLIME) {
        position_block.x = j * level->real_size_edge_block;
        position_block.y = i * level->real_size_edge_block;
        switch (collision_of_two_objects(&enemy->hitbox, &position_block)) {
          case COLLISION_LEFT: {
            enemy->coordinates.x -= speed_dt(enemy->speed);
            enemy->direction = DIRECTION_LEFT;
          } break;
          case COLLISION_RIGHT: {
            enemy->coordinates.x += speed_dt(enemy->speed);
            enemy->direction = DIRECTION_RIGHT;
          } break;
          case COLLISION_UP: {
            enemy->coordinates.y -= speed_dt(enemy->current_speed_gravity);
            enemy->current_speed_gravity = 0;
            is_enemy_standing = 1;
          } break;
          case COLLISION_DOWN: {
            enemy->coordinates.y += speed_dt(enemy->current_speed_gravity);
          } break;
        }
      }
    }
  }
  enemy->is_standing = is_enemy_standing;
  synchronize_hitbox_with_coordinates(&enemy->hitbox, enemy->coordinates);
}

void move_enemy_slime(Enemy_slime* enemy) {
  if (enemy->base.direction == DIRECTION_LEFT)
    enemy->base.coordinates.x -= speed_dt(enemy->base.speed);
  else
    enemy->base.coordinates.x += speed_dt(enemy->base.speed);
}

void draw_enemies() {
  for (int i = 0; i < enemy_container->amount_enemies; ++i) {
    Enemy_base* enemy = enemy_container->enemies[i];
    switch (enemy->type) {
      case ENEMY_SLIME: {
        if (enemy->direction == DIRECTION_LEFT) {
          SDL_RenderCopy(
            renderer, 
            enemy->texture.current->sprites[enemy->texture.current_number_sprite].sprite,
            NULL,
            &enemy->hitbox
          );
        } else {
          SDL_RenderCopyEx(
            renderer,
            enemy->texture.current->sprites[enemy->texture.current_number_sprite].sprite,
            NULL,
            &enemy->hitbox,
            0, NULL, SDL_FLIP_HORIZONTAL
          );
        }
      } break;
    }
  }
}

void collision_enemy_slime_with_hero(Enemy_slime* enemy) {
  switch (collision_of_two_objects(&hero->hitbox, &enemy->base.hitbox)) {
    case COLLISION_UP: {
      hero->coordinates.y -= speed_dt(hero->current_speed_gravity);
    } break;
    case COLLISION_DOWN: {
      hero->coordinates.y += speed_dt(hero->current_speed_gravity);
    } break;
    case COLLISION_RIGHT: {
      if (enemy->base.direction == DIRECTION_LEFT) {
        if (hero->speed * current_coefficient_jerk_hero() > enemy->base.speed)
          enemy->base.coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero() + enemy->base.speed); 
        else
          enemy->base.coordinates.x -= speed_dt(enemy->base.speed); 
      }
      else {
        if (hero->speed * current_coefficient_jerk_hero() > enemy->base.speed)
          enemy->base.coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero() + enemy->base.speed); 
        else
          enemy->base.coordinates.x -= speed_dt(enemy->base.speed); 
      }
    } break;
    case COLLISION_LEFT: {
      if (enemy->base.direction == DIRECTION_LEFT) {
        if (hero->speed * current_coefficient_jerk_hero() > enemy->base.speed)
          enemy->base.coordinates.x += speed_dt(hero->speed * current_coefficient_jerk_hero() + enemy->base.speed); 
        else
          enemy->base.coordinates.x += speed_dt(enemy->base.speed); 
      }
      else {
        if (hero->speed * current_coefficient_jerk_hero() > enemy->base.speed)
          enemy->base.coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero() + enemy->base.speed); 
        else
          enemy->base.coordinates.x -= speed_dt(enemy->base.speed); 
      }
    } break;
  }
}

void set_current_sprite_enemy(Enemy_base* enemy) {
  switch (enemy->type) {
    case ENEMY_SLIME: {
      Enemy_slime* slime = (Enemy_slime*)enemy->full_enemy;
      set_current_sprite_enemy_slime(slime);
    } break;
  }
}

void set_current_sprite_enemy_slime(Enemy_slime* enemy) {
  set_current_sprite(
    enemy->base.texture.current,
    &enemy->base.texture.current_number_sprite,
    &enemy->base.hitbox,
    &enemy->base.coordinates,
    enemy->base.direction,
    &enemy->base.texture.sprite_time_counter,
    enemy->current_state != enemy->prev_state
  );
  if (enemy->current_state != enemy->prev_state)
    enemy->prev_state = enemy->current_state;
}

void determine_current_texture_enemy_slime(Enemy_slime* enemy) {
  if (enemy->prev_state == enemy->current_state) {
    return;
  } else {
    enemy->base.texture.current = &enemy_container->textures[enemy->base.type][enemy->current_state]; 
    enemy->base.texture.current_number_sprite = 0;
  }
}