#include "../include/enemy_slime.h"
#include "../include/enemy_skeleton.h"
#include "../include/enemy_shooter.h"

Enemy_container *enemy_container = NULL;

void malloc_enemy_container() {
  enemy_container = (Enemy_container*)malloc(sizeof(Enemy_container));
  malloc_enemies();
}

void malloc_enemies() {
  enemy_container->amount_enemies = 0;
  enemy_container->enemies = NULL;
  for (int i = 0; i < level->amount_blocks.y; ++i) {
    for (int j = 0; j < level->amount_blocks.x; ++j) {
      Blocks block_spawn_enemy = Blocks(level->map[i][j]);
      if (block_spawn_enemy == BLOCK_SPAWN_SLIME ||
          block_spawn_enemy == BLOCK_SPAWN_SKELETON ||
          block_spawn_enemy == BLOCK_SPAWN_SHOOTER)
        enemy_container->amount_enemies++; 
    }
  }
  enemy_container->enemies = (Enemy_base**)malloc(sizeof(Enemy_base*) * enemy_container->amount_enemies);
}

void init_enemies(Level_number level_number) {
  if (!enemy_container) {
    malloc_enemy_container();
    init_textures_enemies();
  } else {
    de_init_enemies();
    malloc_enemies();
  }
  const char* slime_data_file;
  const char* skeleton_data_file;
  const char* shooter_data_file;
  switch (level_number) {
    case LEVEL_1: {
      slime_data_file = "../game_data/level_1/slime.txt";
      skeleton_data_file = "../game_data/level_1/skeleton.txt";
      shooter_data_file = "../game_data/level_1/shooter.txt";
    } break;
    case LEVEL_2: {
      slime_data_file = "../game_data/level_2/slime.txt";
      skeleton_data_file = "../game_data/level_2/skeleton.txt";
      shooter_data_file = "../game_data/level_2/shooter.txt";
    } break;
    case LEVEL_3: {
      slime_data_file = "../game_data/level_3/slime.txt";
      skeleton_data_file = "../game_data/level_3/skeleton.txt";
      shooter_data_file = "../game_data/level_3/shooter.txt";
    } break;
  }

  //Данные для каждого слайма
  Enemy_slime slime;
  FILE* f;
  if (f = fopen(slime_data_file, "r")) {
    slime.base.health = read_from_file_double(f);
    slime.base.damage = read_from_file_double(f);
    slime.base.speed = read_from_file_double(f); 
    slime.base.amount_experience = read_from_file_double(f);
    fclose(f);
  }
  //Данные для каждого скелета 
  Enemy_skeleton skeleton;
  if (f = fopen(skeleton_data_file, "r")) {
    skeleton.base.health = read_from_file_double(f);
    skeleton.base.damage = read_from_file_double(f);
    skeleton.base.speed = read_from_file_double(f);
    skeleton.reaction_range_walk = read_from_file_double(f);
    skeleton.base.amount_experience = read_from_file_double(f);
    fclose(f);
  }
  Enemy_shooter shooter;
  if (f = fopen(shooter_data_file, "r")) {
    shooter.base.health = read_from_file_double(f);
    shooter.base.damage = read_from_file_double(f);
    shooter.base.speed = read_from_file_double(f);
    shooter.responce_radius = read_from_file_double(f);
    shooter.attack_radius = read_from_file_double(f);
    shooter.attack.shot.speed = read_from_file_double(f);
    shooter.attack.shot.damage = read_from_file_double(f);
    shooter.attack.shot.range = read_from_file_double(f);
    shooter.base.amount_experience = read_from_file_double(f);
    fclose(f);
  }
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
          new_slime->base.hitbox.w = enemy_container->textures[new_slime->base.type][new_slime->current_state].sprites[0].size.w;
          new_slime->base.hitbox.h = enemy_container->textures[new_slime->base.type][new_slime->current_state].sprites[0].size.h;
          new_slime->base.texture.current = &enemy_container->textures[new_slime->base.type][new_slime->current_state];
          enemy = &new_slime->base;
        } break;
        case BLOCK_SPAWN_SKELETON: {
          Enemy_skeleton* new_skeleton = (Enemy_skeleton*)malloc(sizeof(Enemy_skeleton));
          *new_skeleton = skeleton;
          new_skeleton->base.full_enemy = new_skeleton;
          new_skeleton->prev_health = new_skeleton->base.health;
          new_skeleton->base.type = ENEMY_SKELETON;
          new_skeleton->current_state = ENEMY_SKELETON_IDLE;
          new_skeleton->prev_state = new_skeleton->current_state;
          new_skeleton->sprites_timer = 0;
          new_skeleton->base.hitbox.w = enemy_container->textures[new_skeleton->base.type][new_skeleton->current_state].sprites[0].size.w;
          new_skeleton->base.hitbox.h = enemy_container->textures[new_skeleton->base.type][new_skeleton->current_state].sprites[0].size.h;
          new_skeleton->base.texture.current = &enemy_container->textures[new_skeleton->base.type][new_skeleton->current_state];
          enemy = &new_skeleton->base;
        } break;
        case BLOCK_SPAWN_SHOOTER: {
          Enemy_shooter* new_shooter = (Enemy_shooter*)malloc(sizeof(Enemy_shooter));
          *new_shooter = shooter;
          new_shooter->base.full_enemy = new_shooter;
          new_shooter->prev_health = new_shooter->base.health;
          new_shooter->base.type = ENEMY_SHOOTER;
          new_shooter->current_state = ENEMY_SHOOTER_IDLE;
          new_shooter->prev_state = new_shooter->current_state;
          new_shooter->sprites_timer = 0;
          new_shooter->base.hitbox.w = enemy_container->textures[new_shooter->base.type][new_shooter->current_state].sprites[0].size.w;
          new_shooter->base.hitbox.h = enemy_container->textures[new_shooter->base.type][new_shooter->current_state].sprites[0].size.h;
          new_shooter->base.texture.current = &enemy_container->textures[new_shooter->base.type][new_shooter->current_state];
          new_shooter->was_the_shot_fired = 0;
          new_shooter->sprite_number_for_attack = 9;
          new_shooter->float_direction = { 0, 0 };
          new_shooter->attack.shot.type = SHOT_TYPE_HOMING;
          enemy = &new_shooter->base;
        } break;
      }
      if (!enemy)
        continue;
      enemy->coordinates = { 
        float(level->real_size_edge_block * j),
        float(level->real_size_edge_block * i)
      };
      synchronize_hitbox_with_coordinates(&enemy->hitbox, enemy->coordinates);
      enemy->direction = DIRECTION_LEFT;
      enemy->texture.current_number_sprite = 0;
      enemy->current_speed_gravity = 0;
      enemy->is_standing = 0;
      enemy->texture.sprite_time_counter = 0;
      enemy->death_time = 0;
      enemy_container->enemies[current_index_enemy++] = enemy;
    }
  }
}

void malloc_texture_enemy(Enemy_type type, int amount_textures) {
  enemy_container->textures[type] = (Texture*)malloc(sizeof(Texture) * amount_textures);
}

void init_textures_enemies() {
  enemy_container->textures = (Texture**)malloc(sizeof(Texture*) * ENEMY_AMOUNT);
  // enemy slime
  malloc_texture_enemy(ENEMY_SLIME, ENEMY_SLIME_AMOUNT_STATE); 
  init_texture(&enemy_container->textures[ENEMY_SLIME][ENEMY_SLIME_ATTACK], "../game_images/enemy/slime/attack/");
  init_texture(&enemy_container->textures[ENEMY_SLIME][ENEMY_SLIME_WALK], "../game_images/enemy/slime/walk/");
  init_texture(&enemy_container->textures[ENEMY_SLIME][ENEMY_SLIME_DEATH], "../game_images/enemy/slime/death/");
  // enemy skeleton
  malloc_texture_enemy(ENEMY_SKELETON, ENEMY_SKELETON_AMOUNT_STATE);
  init_texture(&enemy_container->textures[ENEMY_SKELETON][ENEMY_SKELETON_IDLE], "../game_images/enemy/skeleton/idle/");
  init_texture(&enemy_container->textures[ENEMY_SKELETON][ENEMY_SKELETON_WALK], "../game_images/enemy/skeleton/walk/");
  init_texture(&enemy_container->textures[ENEMY_SKELETON][ENEMY_SKELETON_ATTACK], "../game_images/enemy/skeleton/attack/");
  init_texture(&enemy_container->textures[ENEMY_SKELETON][ENEMY_SKELETON_HURT], "../game_images/enemy/skeleton/hurt/");
  init_texture(&enemy_container->textures[ENEMY_SKELETON][ENEMY_SKELETON_DEATH], "../game_images/enemy/skeleton/death/");
  // enemy shooter
  malloc_texture_enemy(ENEMY_SHOOTER, ENEMY_SHOOTER_AMOUNT_STATE);
  init_texture(&enemy_container->textures[ENEMY_SHOOTER][ENEMY_SHOOTER_IDLE], "../game_images/enemy/shooter/idle/");
  init_texture(&enemy_container->textures[ENEMY_SHOOTER][ENEMY_SHOOTER_WALK], "../game_images/enemy/shooter/walk/");
  init_texture(&enemy_container->textures[ENEMY_SHOOTER][ENEMY_SHOOTER_SHOT_ATTACK], "../game_images/enemy/shooter/shot_attack/");
  init_texture(&enemy_container->textures[ENEMY_SHOOTER][ENEMY_SHOOTER_HURT], "../game_images/enemy/shooter/hurt/");
  init_texture(&enemy_container->textures[ENEMY_SHOOTER][ENEMY_SHOOTER_DEATH], "../game_images/enemy/shooter/death/");
}

void de_init_enemy_texture(int enemy_type, int enemy_amount_state) {
  for (int state = 0; state < enemy_amount_state; ++state)
    de_init_texture(&enemy_container->textures[enemy_type][state]);
}

void de_init_enemy_container() {
  de_init_enemy_texture(ENEMY_SLIME, ENEMY_SLIME_AMOUNT_STATE);
  de_init_enemy_texture(ENEMY_SKELETON, ENEMY_SKELETON_AMOUNT_STATE);
  de_init_enemy_texture(ENEMY_SHOOTER, ENEMY_SHOOTER_AMOUNT_STATE);
  for (int i = 0; i < ENEMY_AMOUNT; ++i)
    free(enemy_container->textures[i]);
  free(enemy_container->textures);
  de_init_enemies(); 
  free(enemy_container);
}

void de_init_enemies() {
  for (int i = 0; i < enemy_container->amount_enemies; ++i)
    free(enemy_container->enemies[i]->full_enemy);
  free(enemy_container->enemies);
}

void updating_enemies() {
  for (int i = 0; i < enemy_container->amount_enemies; ++i) {
    Enemy_base* enemy = enemy_container->enemies[i];
    switch (enemy->type) {
      case ENEMY_SLIME: {
        update_enemy_slime((Enemy_slime*)enemy->full_enemy);
      } break;
      case ENEMY_SKELETON: {
        update_enemy_skeleton((Enemy_skeleton*)enemy->full_enemy);
      } break;
      case ENEMY_SHOOTER: {
        update_enemy_shooter((Enemy_shooter*)enemy->full_enemy);
      }
    }
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
      if (is_it_a_block(Blocks(level->map[i][j])))
        continue;
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
  enemy->is_standing = is_enemy_standing;
  synchronize_hitbox_with_coordinates(&enemy->hitbox, enemy->coordinates);
}

void draw_enemies() {
  for (int i = 0; i < enemy_container->amount_enemies; ++i) {
    Enemy_base* enemy = enemy_container->enemies[i];
    switch (enemy->type) {
      case ENEMY_SLIME: {
        draw_enemy_slime((Enemy_slime*)enemy->full_enemy);
      } break;
      case ENEMY_SKELETON: {
        draw_enemy_skeleton((Enemy_skeleton*)enemy->full_enemy);
      } break;
      case ENEMY_SHOOTER: {
        draw_enemy_shooter((Enemy_shooter*)enemy->full_enemy);
      } break;
    }
  }
}

void render_copy_enemy(Enemy_base* enemy, SDL_RendererFlip flip) {
  SDL_RenderCopyEx(
    renderer,
    enemy->texture.current->sprites[enemy->texture.current_number_sprite].sprite,
    NULL,
    &enemy->hitbox,
    0, NULL, flip
  );
}

int collision_enemy_with_hero(Enemy_base* enemy) {
  int is_collision = 0;
  switch (collision_of_two_objects(&hero->hitbox, &enemy->hitbox)) {
    case COLLISION_UP: {
      hero->coordinates.y -= speed_dt(hero->current_speed_gravity);
      is_collision = 1;
    } break;
    case COLLISION_DOWN: {
      hero->coordinates.y += speed_dt(hero->current_speed_gravity);
      is_collision = 1;
    } break;
    case COLLISION_RIGHT: {
      if (enemy->direction == DIRECTION_LEFT) {
        if (hero->speed * current_coefficient_jerk_hero() > enemy->speed)
          enemy->coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero() + enemy->speed); 
        else
          enemy->coordinates.x -= speed_dt(enemy->speed); 
      } else {
        if (hero->speed * current_coefficient_jerk_hero() > enemy->speed)
          enemy->coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero() + enemy->speed); 
        else
          enemy->coordinates.x -= speed_dt(enemy->speed); 
      }
      is_collision = 1;
    } break;
    case COLLISION_LEFT: {
      if (enemy->direction == DIRECTION_LEFT) {
        if (hero->speed * current_coefficient_jerk_hero() > enemy->speed)
          enemy->coordinates.x += speed_dt(hero->speed * current_coefficient_jerk_hero() + enemy->speed); 
        else
          enemy->coordinates.x += speed_dt(enemy->speed); 
      } else {
        if (hero->speed * current_coefficient_jerk_hero() > enemy->speed)
          enemy->coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero() + enemy->speed); 
        else
          enemy->coordinates.x -= speed_dt(enemy->speed); 
      }
      is_collision = 1;
    } break;
  }
  return is_collision;
}