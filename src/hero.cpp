#include "../include/hero.h"

Hero* hero;
const int amount_attack_hero = 3;
Attack_type_info attack_info_hero[amount_attack_hero] = {
  {1.0f, 2},
  {1.1f, 1},
  {1.2f, 2},
};

void init_hero() {
  if (!(hero = (Hero*)malloc(sizeof(Hero)))) {
    printf("Не удалось получить ресурсы для Hero *hero...\n");
    de_init_application(1);
  }
  hero->coordinates = get_coordinates_for_new_game_hero();
  init_texture(&hero->textures.idle, "../game_images/hero/skin_knight/idle/");
  init_texture(&hero->textures.attack_1, "../game_images/hero/skin_knight/attack_1/");
  init_texture(&hero->textures.attack_2, "../game_images/hero/skin_knight/attack_2/");
  init_texture(&hero->textures.attack_3, "../game_images/hero/skin_knight/attack_3/");
  init_texture(&hero->textures.run, "../game_images/hero/skin_knight/run/");
  init_texture(&hero->textures.walk, "../game_images/hero/skin_knight/walk/");
  init_texture(&hero->textures.fall, "../game_images/hero/skin_knight/fall/");
  init_texture(&hero->textures.hurt, "../game_images/hero/skin_knight/hurt/");
  init_texture(&hero->textures.jump, "../game_images/hero/skin_knight/jump/");
  init_texture(&hero->textures.death, "../game_images/hero/skin_knight/death/");
  set_current_texture_hero(&hero->textures.idle);
  hero->hitbox = {
    (int)hero->coordinates.x,
    (int)hero->coordinates.y,
    hero->current_texture->sprites[0].size.w,
    hero->current_texture->sprites[0].size.h
  };
  hero->coefficient_jerk = 3;
  hero->speed = 70;
  hero->direction = DIRECTION_RIGHT;
  hero->jump_height = 100;
  hero->current_speed_gravity = 0;
  hero->attack.pure_damage = 5;
  hero->attack.cause_damage = 0;
  hero->attack.type = ATTACK_HERO_NONE;
  hero->attack.hitbox = { 0, 0, 0, 0 };
  hero->state = HERO_IDLE;
}

void load_hero(const char* load_file) {

}

void de_init_hero() {
  de_init_texture(&hero->textures.idle);
  de_init_texture(&hero->textures.attack_3);
  de_init_texture(&hero->textures.attack_2);
  de_init_texture(&hero->textures.attack_1);
  de_init_texture(&hero->textures.run);
  de_init_texture(&hero->textures.walk);
  de_init_texture(&hero->textures.fall);
  de_init_texture(&hero->textures.jump);
  de_init_texture(&hero->textures.hurt);
  de_init_texture(&hero->textures.death);
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
  SDL_Texture* current_sprite = hero->current_texture->sprites[hero->current_number_sprite].sprite;
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
  if (hero->attack.type != ATTACK_HERO_NONE)
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

void hitbox_change_due_new_sprite_hero(int number_sprite, float* height_difference, float* width_difference) {
  hero->current_number_sprite = number_sprite;
  if (height_difference)
    *height_difference = hero->hitbox.h - hero->current_texture->sprites[hero->current_number_sprite].size.h;
  if (hero->direction == DIRECTION_LEFT && width_difference != NULL)
    *width_difference = hero->current_texture->sprites[hero->current_number_sprite].size.w - hero->hitbox.w;
  hero->hitbox.w = hero->current_texture->sprites[hero->current_number_sprite].size.w;
  hero->hitbox.h = hero->current_texture->sprites[hero->current_number_sprite].size.h;
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
      } break;
      case PLATFORM_BREAKING: {
        state = collision_platform_with_hero(platform);
        if (hero->state == HERO_IDLE && hero->state == HERO_WALK && hero->state == HERO_RUN)
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
  }
  if (keyboard[SDL_SCANCODE_J]) {
    hero->attack.type = ATTACK_HERO_BASE_1;
  } else if (keyboard[SDL_SCANCODE_K]) {
    hero->attack.type = ATTACK_HERO_BASE_2;
  } else if (keyboard[SDL_SCANCODE_L]) {
    hero->attack.type = ATTACK_HERO_BASE_3;
  }
}

void attack_logic_hero() {
  switch (hero->attack.type) {
    case ATTACK_HERO_BASE_1: {
      if (attack_info_hero[hero->attack.type].number_sprite_for_damage == hero->current_number_sprite) {
        float damage = get_damage_hero(hero->attack.type);
        //Получить хитбокс удара и проверить коллиизию с противниками.
      }
    } break;
    case ATTACK_HERO_BASE_2: {

    } break;
    case ATTACK_HERO_BASE_3: {

    } break;
  }
}

float get_damage_hero(Attack_type type) {
  return hero->attack.pure_damage * attack_info_hero[hero->attack.type].damage_multiplier;
}

void determine_current_texture_hero() {
  static Hero_state prev_state = HERO_IDLE;
  if (prev_state == hero->state)
    return;
  switch (hero->state) {
    case HERO_IDLE: {
      printf("hero_idle\n");
      set_current_texture_hero(&hero->textures.idle);
    } break;
    case HERO_WALK: {
      printf("hero_walk\n");
      set_current_texture_hero(&hero->textures.walk);
    } break;
    case HERO_RUN: {
      printf("hero_run\n");
      set_current_texture_hero(&hero->textures.run);
    } break;
    case HERO_ATTACK: {
      printf("hero_attack\n");
      switch (hero->attack.type) {
        case ATTACK_HERO_BASE_1: {
          set_current_texture_hero(&hero->textures.attack_1);
        } break;
        case ATTACK_HERO_BASE_2: {
          set_current_texture_hero(&hero->textures.attack_2);
        } break;
        case ATTACK_HERO_BASE_3: {
          set_current_texture_hero(&hero->textures.attack_3);
        } break;
      }
    } break;
    case HERO_JUMP: {
      printf("hero_jump\n");
      set_current_texture_hero(&hero->textures.jump);
    } break;
    case HERO_DEATH: {
      printf("hero_death\n");
      set_current_texture_hero(&hero->textures.death);
    } break;
    case HERO_FALL: {
      printf("hero_fall\n");
      set_current_texture_hero(&hero->textures.fall);
    } break;
    case HERO_HURT: {
      printf("hero_hurt\n");
      set_current_texture_hero(&hero->textures.hurt);
    } break;
  }
  prev_state = hero->state;
}

void set_current_texture_hero(Texture* texture) {
  hero->current_number_sprite = 0;
  hero->current_texture = texture;
}

void set_current_sprite_hero() {
  static double time = 0;
  static Texture* current_texture = hero->current_texture;
  if (current_texture != hero->current_texture) {
    time = 0;
    current_texture = hero->current_texture;
  }
  time += dt;
  float height_difference = 0.f;
  float width_difference = 0.f;
  if (time > current_texture->sprites[hero->current_number_sprite].rendering_time) {
    hero->current_number_sprite++; 
    if (hero->current_number_sprite == current_texture->amount_sprite) {
      if (hero->current_texture->repeat_animation)
        hero->current_number_sprite = 0;
      else
        hero->current_number_sprite--;
    }
    hitbox_change_due_new_sprite_hero(hero->current_number_sprite, &height_difference, &width_difference);
    time = 0;
  }
  hero->coordinates.y += height_difference;
  hero->coordinates.x -= width_difference;
  synchronize_hitbox_with_coordinates(&hero->hitbox, hero->coordinates);
}

void collision_attack_hero_with_enemy() {

}