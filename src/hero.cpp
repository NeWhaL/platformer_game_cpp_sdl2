#include "../include/hero.h"

Hero* hero;

void init_hero() {
  if (!(hero = (Hero*)malloc(sizeof(Hero)))) {
    printf("Не удалось получить ресурсы для Hero *hero...\n");
    de_init_application(1);
  }
  hero->current_number_sprite = 0;
  hero->coordinates = get_coordinates_for_new_game_hero();
  // init_texture(&hero->textures.idle, "../game_images/hero/skin_knight/idle/");
  // init_texture(&hero->textures.attack_1, "../game_images/hero/skin_knight/attack_1/");
  // init_texture(&hero->textures.attack_2, "../game_images/hero/skin_knight/attack_2/");
  // init_texture(&hero->textures.attack_3, "../game_images/hero/skin_knight/attack_3/");
  hero->current_texture = &hero->textures.attack_2;
  hero->hitbox = {
    (int)hero->coordinates.x,
    (int)hero->coordinates.y,
    hero->current_texture->sprites[0].size.w,
    hero->current_texture->sprites[0].size.h
  };
  hero->coefficient_jerk = 3;
  hero->speed = 70;
  hero->direction = DIRECTION_RIGHT;
  hero->is_standing = 0;
  hero->jump_height = 100;
  hero->is_jumping = 0;
  hero->current_speed_gravity = 0;
  hero->is_attack = ATTACK_HERO_NONE;
}

void load_hero(const char* load_file) {

}

void de_init_hero() {
  de_init_texture(&hero->textures.idle);
  de_init_texture(&hero->textures.attack_1);
  de_init_texture(&hero->textures.attack_2);
  de_init_texture(&hero->textures.attack_3);
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
  SDL_FPoint coordinates_before_gravity_action = hero->coordinates;
  gravity_hero();
  move_hero();
  collision_with_blocks_hero();
  collision_platforms_with_hero();
  set_current_sprite_hero(130);
}

void move_hero() {
  if (keyboard[SDL_SCANCODE_A]) {
    hero->direction = DIRECTION_LEFT;
    hero->coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero());
  } else if (keyboard[SDL_SCANCODE_D]) {
    hero->direction = DIRECTION_RIGHT;
    hero->coordinates.x += speed_dt(hero->speed * current_coefficient_jerk_hero());
  }
  synchronize_hitbox_with_coordinates(&hero->hitbox, hero->coordinates);
}

void gravity_hero() {
  static float jump_speed_y = 0;
  if (hero->current_speed_gravity < speed_gravity)
    hero->current_speed_gravity += speed_dt(speed_gravity);
  if (keyboard[SDL_SCANCODE_SPACE] && hero->is_standing) {
    hero->is_jumping = 1;
    jump_speed_y = sqrt(speed_gravity * hero->jump_height);
    hero->is_standing = 0;
    hero->current_speed_gravity = 0;
  }
  if (!hero->is_standing && hero->is_jumping) {
    hero->coordinates.y -= speed_dt(jump_speed_y);
    jump_speed_y -= speed_dt(hero->current_speed_gravity);
  } else 
    jump_speed_y = 0;
  if (jump_speed_y <= 0 && hero->is_jumping) {
    hero->is_jumping = 0;
    hero->current_speed_gravity = 0;
  }
  if (!hero->is_jumping) {
    hero->coordinates.y += speed_dt(hero->current_speed_gravity);
  }
}

void set_current_sprite_hero(double time_one_frame) {
  static double time = 0;
  time += dt;
  float height_difference = 0.f;
  for (int i = 0; i < hero->current_texture->amount_sprite; ++i) {
    if (time <= time_one_frame * i) {
      hero->current_number_sprite = i;
      height_difference = hero->hitbox.h - hero->current_texture->sprites[hero->current_number_sprite].size.h;
      hero->hitbox.w = hero->current_texture->sprites[hero->current_number_sprite].size.w;
      hero->hitbox.h = hero->current_texture->sprites[hero->current_number_sprite].size.h;
      break;
    }
  }
  if (time > time_one_frame * hero->current_texture->amount_sprite) {
    hero->current_number_sprite = 0;
    time = 0; 
    height_difference = hero->hitbox.h - hero->current_texture->sprites[hero->current_number_sprite].size.h;
    hero->hitbox.w = hero->current_texture->sprites[hero->current_number_sprite].size.w;
    hero->hitbox.h = hero->current_texture->sprites[hero->current_number_sprite].size.h;
  }
  hero->coordinates.y += height_difference;
  synchronize_hitbox_with_coordinates(&hero->hitbox, hero->coordinates);
}

int current_coefficient_jerk_hero() {
  return keyboard[SDL_SCANCODE_LSHIFT] ? hero->coefficient_jerk : 1;
}

void collision_with_blocks_hero() {
  SDL_Rect position_block = { 0, 0, level->real_size_edge_block, level->real_size_edge_block };
  int is_hero_standing = 0;
  for (int i = 0; i < level->amount_blocks.y; ++i) {
    for (int j = 0; j < level->amount_blocks.x; ++j) {
      Blocks b_type = Blocks(level->map[i][j]);
      if (b_type != BLOCK_SPACE && b_type != BLOCK_PLATFORM_BASE && b_type != BLOCK_SPAWN_HERO) {
        position_block.x = j * level->real_size_edge_block;
        position_block.y = i * level->real_size_edge_block;
        switch (collision_with_block(&hero->hitbox, &position_block)) {
          case COLLISION_LEFT: {
            hero->coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero());
          } break;
          case COLLISION_RIGHT: {
            hero->coordinates.x += speed_dt(hero->speed * current_coefficient_jerk_hero());
          } break;
          case COLLISION_UP: {
            hero->coordinates.y -= speed_dt(speed_gravity);
            hero->current_speed_gravity = 0;
            is_hero_standing = 1;
          } break;
          case COLLISION_DOWN: {
            hero->coordinates.y += speed_dt(sqrt(speed_gravity * hero->jump_height));
            hero->is_jumping = 0;
          } break;
        }
      }
    }
  }
  hero->is_standing = is_hero_standing;
  synchronize_hitbox_with_coordinates(&hero->hitbox, hero->coordinates);
}

int collision_platform_with_hero(struct Platform* platform) {
  int is_hero_standing = 0;
  switch (collision_with_block(&hero->hitbox, &platform->hitbox)) {
    case COLLISION_UP: {
      if (platform->direction == DIRECTION_LEFT)
        hero->coordinates.x -= speed_dt(platform->speed);
      else
        hero->coordinates.x += speed_dt(platform->speed);
      hero->coordinates.y -= speed_dt(speed_gravity);
      is_hero_standing = 1;
    } break;
    case COLLISION_LEFT: {
      if (platform->direction == hero->direction)
        hero->coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero());
      //В случае, если платформа и герой двигаются в противоположных направлениях, то коллизия в этом случае не корректно работает
      else
        hero->coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero() + platform->speed);
    } break;
    case COLLISION_DOWN: {
      hero->coordinates.y += speed_dt(sqrt(speed_gravity * hero->jump_height));
      hero->is_jumping = 0;
    } break;
    case COLLISION_RIGHT: {
      if (platform->direction == hero->direction)
        hero->coordinates.x += speed_dt(hero->speed * current_coefficient_jerk_hero());
      //В случае, если платформа и герой двигаются в противоположных направлениях, то коллизия в этом случае не корректно работает
      else
        hero->coordinates.x += speed_dt(hero->speed * current_coefficient_jerk_hero() + platform->speed);
    } break;
  }
  return is_hero_standing;
}

void collision_platforms_with_hero() {
  int is_hero_standing = 0;
  for (int i = 0; i < amount_platforms; ++i) {
    Platform* platform = &platforms[i];
    switch (platform->type) {
      case PLATFORM_BASE: {
        is_hero_standing = collision_platform_with_hero(platform);
      } break;
      case PLATFORM_DISAPPEARING: {
        if (platform->special.disappearing.is_active)
          is_hero_standing = collision_platform_with_hero(platform);
      } break;
      case PLATFORM_BREAKING: {
        if (is_hero_standing = collision_platform_with_hero(platform))
          platform->special.breaking.was_the_hero_standing_on_the_platform = 1;
      } break;
    }
  }
  if (!hero->is_standing)
    hero->is_standing = is_hero_standing;
  synchronize_hitbox_with_coordinates(&hero->hitbox, hero->coordinates);
}

void select_attack_hero() {
  // if (hero->is_attack != ATTACK_HERO_NONE)
  //   return;
  // if (keyboard[SDL_SCANCODE_J]) {
  //   hero->is_attack = ATTACK_HERO_BASE_1;
  // } else if (keyboard[SDL_SCANCODE_K]) {
  //   hero->is_attack = ATTACK_HERO_BASE_2;
  // }
}

void attack_hero() {
  // select_attack_hero();
  // if (hero->is_attack == ATTACK_HERO_NONE) {
  //   return;
  // }

}

void set_attack_texture_hero() {
  // hero->current_texture
}