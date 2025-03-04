#include "../include/hero.h"

Hero* hero;
const int amount_attack_hero = 3;
Attack_type_info attack_info_hero[amount_attack_hero];

void init_hero() {
  if (!(hero = (Hero*)malloc(sizeof(Hero)))) {
    printf("Не удалось получить ресурсы для Hero *hero...\n");
    de_init_application(1);
  }
  hero->state = HERO_IDLE;
  hero->coordinates = get_coordinates_for_new_game_hero();
  init_textures_hero();
  set_current_texture_hero(hero->textures.all[hero->state]);
  hero->hitbox = {
    (int)hero->coordinates.x,
    (int)hero->coordinates.y,
    hero->textures.current->sprites[0].size.w,
    hero->textures.current->sprites[0].size.h
  };
  hero->coefficient_jerk = 3;
  hero->speed = 70;
  hero->direction = DIRECTION_RIGHT;
  hero->jump_height = 100;
  hero->current_speed_gravity = 0;
  init_attack_type_info_hero();
  hero->attack.pure_damage = 5;
  hero->attack.cause_damage = 0;
  hero->attack.type = HERO_ATTACK_NONE;
  hero->attack.hitbox = { 0, 0, 0, 0 };
}

void init_textures_hero() {
  if (!(hero->textures.all = (Texture**)malloc(sizeof(Texture*) * HERO_AMOUNT_STATE))) {
    printf("Не удалось получить ресурсы для Hero *hero...\n");
    de_init_application(1);
  }
  malloc_texture_hero(HERO_IDLE);
  init_texture(hero->textures.all[HERO_IDLE], "../game_images/hero/skin_knight/idle/");
  malloc_texture_hero(HERO_WALK);
  init_texture(hero->textures.all[HERO_WALK], "../game_images/hero/skin_knight/walk/");
  malloc_texture_hero(HERO_RUN);
  init_texture(hero->textures.all[HERO_RUN], "../game_images/hero/skin_knight/run/");
  malloc_texture_hero(HERO_JUMP);
  init_texture(hero->textures.all[HERO_JUMP], "../game_images/hero/skin_knight/jump/");
  malloc_texture_hero(HERO_FALL);
  init_texture(hero->textures.all[HERO_FALL], "../game_images/hero/skin_knight/fall/");
  malloc_texture_hero(HERO_HURT);
  init_texture(hero->textures.all[HERO_HURT], "../game_images/hero/skin_knight/hurt/");
  malloc_texture_hero(HERO_DEATH);
  init_texture(hero->textures.all[HERO_DEATH], "../game_images/hero/skin_knight/death/");
  malloc_texture_hero(HERO_ATTACK, HERO_ATTACK_AMOUNT);
  init_texture(&hero->textures.all[HERO_ATTACK][HERO_ATTACK_BASE_1], "../game_images/hero/skin_knight/attack_1/");
  init_texture(&hero->textures.all[HERO_ATTACK][HERO_ATTACK_BASE_2], "../game_images/hero/skin_knight/attack_2/");
  init_texture(&hero->textures.all[HERO_ATTACK][HERO_ATTACK_BASE_3], "../game_images/hero/skin_knight/attack_3/");
  hero->textures.sprite_time_counter = 0;
}

void malloc_texture_hero(Hero_state state, int amount_textures) {
  hero->textures.all[state] = (Texture*)malloc(sizeof(Texture) * amount_textures);
}

void init_attack_type_info_hero() {
  attack_info_hero[HERO_ATTACK_BASE_1] = {
    1.0f, 2, time_for_one_texture_iteration(&hero->textures.all[HERO_ATTACK][HERO_ATTACK_BASE_1])
  };
  attack_info_hero[HERO_ATTACK_BASE_2] = {
    1.1f, 1, time_for_one_texture_iteration(&hero->textures.all[HERO_ATTACK][HERO_ATTACK_BASE_2])
  };
  attack_info_hero[HERO_ATTACK_BASE_3] = {
    1.2f, 2, time_for_one_texture_iteration(&hero->textures.all[HERO_ATTACK][HERO_ATTACK_BASE_3])
  };
}

void load_hero(const char* load_file) {

}

void de_init_hero() {
  de_init_texture(hero->textures.all[HERO_IDLE]);
  de_init_texture(hero->textures.all[HERO_WALK]);
  de_init_texture(hero->textures.all[HERO_RUN]);
  de_init_texture(hero->textures.all[HERO_FALL]);
  de_init_texture(hero->textures.all[HERO_JUMP]);
  de_init_texture(hero->textures.all[HERO_HURT]);
  de_init_texture(hero->textures.all[HERO_DEATH]);
  de_init_texture(&hero->textures.all[HERO_ATTACK][HERO_ATTACK_BASE_1]);
  de_init_texture(&hero->textures.all[HERO_ATTACK][HERO_ATTACK_BASE_2]);
  de_init_texture(&hero->textures.all[HERO_ATTACK][HERO_ATTACK_BASE_3]);
  for (int i = 0; i < HERO_AMOUNT_STATE; ++i)
    free(hero->textures.all[i]);
  free(hero);
}

SDL_FPoint get_coordinates_for_new_game_hero() {
  for (int i = 0; i < level->amount_blocks.y; ++i) {
    for (int j = 0; j < level->amount_blocks.x; ++j) {
      Blocks spawn_block = Blocks(level->map[i][j]);
      if (spawn_block == BLOCK_SPAWN_HERO)
        return {float(level->real_size_edge_block * j), float(level->real_size_edge_block * i)};
    }
  }
  return {-1, -1};
}

void draw_hero() {
  SDL_Texture* current_sprite = hero->textures.current->sprites[hero->textures.current_number_sprite].sprite;
  if (hero->direction == DIRECTION_RIGHT)
    SDL_RenderCopy(renderer, current_sprite, NULL, &hero->hitbox);
  else
    SDL_RenderCopyEx(renderer, current_sprite, NULL, &hero->hitbox, 0, NULL, SDL_FLIP_HORIZONTAL);
}

void update_hero() {
  gravity_hero();
  move_hero();
  if (collision_with_blocks_hero() == HERO_FALL &&
      collision_platforms_with_hero() == HERO_FALL &&
      hero->state != HERO_JUMP) {
    hero->state = HERO_FALL;
  }
  jump_hero();
  attack_hero();
  determine_current_texture_hero();
  set_current_sprite_hero();
}

void gravity_hero() {
  if (hero->state == HERO_IDLE || hero->state == HERO_WALK || hero->state == HERO_RUN) {
    hero->current_speed_gravity = 0;
    return;
  } else {
    hero->current_speed_gravity += speed_dt(speed_gravity);
  }
  if (hero->state == HERO_FALL)
    hero->coordinates.y += speed_dt(hero->current_speed_gravity);
}

void jump_hero() {
  static float jump_speed_y = 0;
  if (keyboard[SDL_SCANCODE_SPACE] && (hero->state == HERO_IDLE || hero->state == HERO_WALK || hero->state == HERO_RUN)) {
    hero->state = HERO_JUMP;
    jump_speed_y = sqrt(speed_gravity * hero->jump_height);
    hero->current_speed_gravity = 0;
  } 
  if (hero->state == HERO_JUMP) {
    hero->coordinates.y -= speed_dt(jump_speed_y);
    jump_speed_y -= speed_dt(hero->current_speed_gravity);
  } 
  if (jump_speed_y <= 0 && hero->state == HERO_JUMP) {
    hero->state = HERO_FALL;
    hero->current_speed_gravity = 0;
  }
}

void move_hero() {
  if (hero->state == HERO_ATTACK)
    return; 
  if (keyboard[SDL_SCANCODE_A]) {
    hero->direction = DIRECTION_LEFT;
    if (hero->state != HERO_JUMP)
      hero->state = HERO_WALK;
    hero->coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero());
  } else if (keyboard[SDL_SCANCODE_D]) {
    hero->direction = DIRECTION_RIGHT;
    if (hero->state != HERO_JUMP)
      hero->state = HERO_WALK;
    hero->coordinates.x += speed_dt(hero->speed * current_coefficient_jerk_hero());
  } else {
    if (hero->state != HERO_JUMP)
      hero->state = HERO_IDLE;
  }
  synchronize_hitbox_with_coordinates(&hero->hitbox, hero->coordinates);
}

float current_coefficient_jerk_hero() {
  if (keyboard[SDL_SCANCODE_LSHIFT]) {
    if (hero->state != HERO_JUMP)
      hero->state = HERO_RUN;
    return hero->coefficient_jerk;
  }
  return 1;
}

//Возвращает HERO_FALL, если герой не коллизирует с блоками всерху или коллизирует снизу
//иначе возвращает текущее состояние героя
Hero_state collision_with_blocks_hero() {
  SDL_Rect position_block = { 0, 0, level->real_size_edge_block, level->real_size_edge_block };
  Hero_state state = hero->state;
  int is_collision_up = 0;
  for (int i = 0; i < level->amount_blocks.y; ++i) {
    for (int j = 0; j < level->amount_blocks.x; ++j) {
      Blocks b_type = Blocks(level->map[i][j]);
      if (b_type != BLOCK_SPACE && b_type != BLOCK_PLATFORM_BASE && b_type != BLOCK_SPAWN_HERO &&
          b_type != BLOCK_SPAWN_SLIME) {
        position_block.x = j * level->real_size_edge_block;
        position_block.y = i * level->real_size_edge_block;
        switch (collision_of_two_objects(&hero->hitbox, &position_block)) {
          case COLLISION_LEFT: {
            hero->coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero());
          } break;
          case COLLISION_RIGHT: {
            hero->coordinates.x += speed_dt(hero->speed * current_coefficient_jerk_hero());
          } break;
          case COLLISION_UP: {
            hero->current_speed_gravity = 0;
            is_collision_up = 1;
          } break;
          case COLLISION_DOWN: {
            hero->coordinates.y += speed_dt(sqrt(speed_gravity * hero->jump_height));
            state = HERO_FALL;
            hero->state = HERO_FALL;
          } break;
        }
      }
    }
  }
  if (!is_collision_up)
    state = HERO_FALL;
  synchronize_hitbox_with_coordinates(&hero->hitbox, hero->coordinates);
  return state;
}

//Возвращает HERO_FALL, если герой не коллизирует с платформой всерху или коллизирует снизу
//иначе возвращает текущее состояние героя
Hero_state collision_platform_with_hero(struct Platform* platform) {
  Hero_state state = hero->state;
  int is_collision_up = 0;
  switch (collision_of_two_objects(&hero->hitbox, &platform->hitbox)) {
    case COLLISION_UP: {
      if (platform->direction == DIRECTION_LEFT)
        hero->coordinates.x -= speed_dt(platform->speed);
      else
        hero->coordinates.x += speed_dt(platform->speed);
      is_collision_up = 1;
    } break;
    case COLLISION_LEFT: {
      if (platform->direction == hero->direction)
        hero->coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero());
      else
        hero->coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero() + platform->speed);
    } break;
    case COLLISION_DOWN: {
      hero->coordinates.y += speed_dt(sqrt(speed_gravity * hero->jump_height));
      state = HERO_FALL;
      hero->state = HERO_FALL;
    } break;
    case COLLISION_RIGHT: {
      if (platform->direction == hero->direction)
        hero->coordinates.x += speed_dt(hero->speed * current_coefficient_jerk_hero());
      else
        hero->coordinates.x += speed_dt(hero->speed * current_coefficient_jerk_hero() + platform->speed);
    } break;
  }
  if (!is_collision_up)
    state = HERO_FALL;
  return state;
}

Hero_state collision_platforms_with_hero() {
  Hero_state state = hero->state;
  for (int i = 0; i < amount_platforms; ++i) {
    Platform* platform = &platforms[i];
    collision_direction current_collision = COLLISION_NONE;
    switch (platform->type) {
      case PLATFORM_BASE: {
        state = collision_platform_with_hero(platform);
      } break;
      case PLATFORM_DISAPPEARING: {
        if (platform->special.disappearing.is_active)
          state = collision_platform_with_hero(platform);
        else
          state = HERO_FALL;
      } break;
      case PLATFORM_BREAKING: {
        state = collision_platform_with_hero(platform);
        if (hero->state == HERO_IDLE || hero->state == HERO_WALK || hero->state == HERO_RUN)
          platform->special.breaking.was_the_hero_standing_on_the_platform = 1;
      } break;
    }
  }
  synchronize_hitbox_with_coordinates(&hero->hitbox, hero->coordinates);
  return state;
}

void attack_hero() {
  if (hero->state == HERO_ATTACK) {
    attack_logic_hero();
    return;
  } else if (hero->state != HERO_IDLE && hero->state != HERO_WALK && hero->state != HERO_RUN)
    return;
  if (keyboard[SDL_SCANCODE_J]) {
    hero->state = HERO_ATTACK;
    hero->attack.type = HERO_ATTACK_BASE_1;
  } else if (keyboard[SDL_SCANCODE_K]) {
    hero->state = HERO_ATTACK;
    hero->attack.type = HERO_ATTACK_BASE_2;
  } else if (keyboard[SDL_SCANCODE_L]) {
    hero->state = HERO_ATTACK;
    hero->attack.type = HERO_ATTACK_BASE_3;
  }
}

void attack_logic_hero() {
  static double time = 0;
  time += dt;
  if (time >= attack_info_hero[hero->attack.type].total_impact_time) {
    hero->state = HERO_IDLE;
    hero->attack.type = HERO_ATTACK_NONE;
    time = 0;
    return;
  } else if (attack_info_hero[hero->attack.type].number_sprite_for_damage == hero->textures.current_number_sprite) {
    float damage = get_damage_hero(hero->attack.type);
    //Получить хитбокс удара и проверить коллиизию с противниками.
    SDL_Rect attack_hitbox = hero->hitbox;
    for (int i = 0; i < enemy_container->amount_enemies; ++i) {  
      Enemy_base* enemy = enemy_container->enemies[i];
      if (collision_of_two_objects(&attack_hitbox, &enemy->hitbox)) {
        enemy->health -= damage;
        if (enemy->health <= 0) {
          switch (enemy->type) {
            case ENEMY_SLIME: {
              Enemy_slime* slime = (Enemy_slime*)enemy->full_enemy; 
              slime->current_state = ENEMY_SLIME_DEATH;
            } break;
          } 
        }
      }
    }
  }
}

float get_damage_hero(Attack_type type) {
  return hero->attack.pure_damage * attack_info_hero[hero->attack.type].damage_multiplier;
}

void collision_attack_hero_with_enemy() {

}

void determine_current_texture_hero() {
  static Hero_state prev_state = HERO_IDLE;
  if (prev_state == hero->state)
    return;
  if (hero->state != HERO_ATTACK)
    set_current_texture_hero(hero->textures.all[hero->state]);
  else
    set_current_texture_hero(&hero->textures.all[hero->state][hero->attack.type]);
  prev_state = hero->state;
}

void set_current_texture_hero(Texture* texture) {
  hero->textures.current_number_sprite = 0;
  hero->textures.current = texture;
}

void set_current_sprite_hero() {
  static Texture* prev_texture = hero->textures.current;
  set_current_sprite(
    hero->textures.current, 
    &hero->textures.current_number_sprite, 
    &hero->hitbox,
    &hero->coordinates,
    hero->direction,
    &hero->textures.sprite_time_counter,
    prev_texture != hero->textures.current
  );
  if (prev_texture != hero->textures.current)
    prev_texture = hero->textures.current;
}