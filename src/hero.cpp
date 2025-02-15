#include "../include/hero.h"

personage* hero;

void init_hero() {
  if (!(hero = (personage*)malloc(sizeof(personage)))) {
    printf("Не удалось получить ресурсы для personage *hero...\n");
    de_init_application(1);
  }
  hero->textures.idle.texture = load_texture_from_file("../game_images/Hero Knight/Sprites/HeroKnight/Idle/hero_knight_idle.png", 
      &hero->textures.idle.size);
  hero->textures.idle.amount_sprite = hero->textures.idle.size.w / hero->textures.idle.size.h ;
  hero->current_idle = 0;
  hero->coordinates = { 100, 100 };
  hero->hitbox = { 
    (int)hero->coordinates.x, 
    (int)hero->coordinates.y, 
    hero->textures.idle.size.w / 8, 
    hero->textures.idle.size.h 
  };
  hero->speed = 70;
  hero->coefficient_jerk = 3;
  hero->current_texture = &hero->textures.idle;
  hero->direction = DIRECTION_RIGHT;
  hero->is_standing = 0;
  hero->jump_height = 100;
  hero->is_jumping = 0;
  hero->current_speed_gravity = 0;
}

void de_init_hero() {
  SDL_DestroyTexture(hero->textures.idle.texture);
  free(hero);
}

void draw_hero() {
  SDL_Rect current_frame = hero->current_texture->size;
  current_frame.x = hero->current_texture->size.x + hero->hitbox.w * hero->current_idle;
  current_frame.w = hero->hitbox.w;
  if (hero->direction == DIRECTION_RIGHT)
    SDL_RenderCopy(renderer, hero->current_texture->texture, &current_frame, &hero->hitbox);
  else
    SDL_RenderCopyEx(renderer, hero->current_texture->texture, &current_frame, &hero->hitbox, 0, NULL, SDL_FLIP_HORIZONTAL);
}

void update_hero() {
  SDL_FPoint coordinates_before_gravity_action = hero->coordinates;
  gravity_hero();
  move_hero();
  jump_hero();
  collision_with_blocks_hero();
  set_current_sprite_hero(130);
}

void move_hero() {
  float coeficient_jerk = 1;
  if (keyboard[SDL_SCANCODE_LSHIFT])
    coeficient_jerk = hero->coefficient_jerk;
  if (keyboard[SDL_SCANCODE_A]) {
    hero->direction = DIRECTION_LEFT;
    hero->coordinates.x -= speed_dt(hero->speed * coeficient_jerk);
  } else if (keyboard[SDL_SCANCODE_D]) {
    hero->direction = DIRECTION_RIGHT;
    hero->coordinates.x += speed_dt(hero->speed * coeficient_jerk);
  } else if (keyboard[SDL_SCANCODE_W]) {
    hero->coordinates.y -= speed_dt(hero->speed * coeficient_jerk);
  } else if (keyboard[SDL_SCANCODE_S]) {
    hero->coordinates.y += speed_dt(hero->speed * coeficient_jerk);
  }
  synchronize_hitbox_with_coordinates(&hero->hitbox, hero->coordinates);
}

void gravity_hero() {
  static float jump_speed_y = 0;
  // static float current_speed_gravity = 0;
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
    // current_speed_gravity = 0;
  if (jump_speed_y <= 0 && hero->is_jumping) {
    hero->is_jumping = 0;
    hero->current_speed_gravity = 0;
  }

  if (!hero->is_jumping) {
    hero->coordinates.y += speed_dt(hero->current_speed_gravity);
  }


  // static float current_speed_gravity = 0;
  // if (hero->is_standing) {
  //   current_speed_gravity = 0;
  //   return;
  // }
  // current_speed_gravity -= speed_dt(speed_gravity);
  // if (current_speed_gravity < -100)
  //   int q = 1;
  // float d = speed_dt(current_speed_gravity);
  // hero->coordinates.y += speed_dt(current_speed_gravity);
}

void jump_hero() {
  // static int is_jumping = 0;
  // static float jump_speed_y = 0;
  // if (keyboard[SDL_SCANCODE_SPACE] && hero->is_standing) {
    // is_jumping = 1;
    // jump_speed_y = sqrt(2 * speed_gravity * hero->jump_height);
    // hero->is_standing = 0;
  // }
  // if (!hero->is_standing)
  //   hero->coordinates.y -= speed_dt(jump_speed_y);
  // else
  //   jump_speed_y = 0;
  // if (!hero->is_standing) {
  //   jump_speed_y -= speed_dt(speed_gravity);
  //   if (jump_speed_y < -100)
  //     int x = 1;
  //   hero->coordinates.y -= speed_dt(jump_speed_y);
  // }
}

void set_current_sprite_hero(double time_one_frame) {
  static double time = 0;
  time += dt;
  for (int i = 0; i < hero->current_texture->amount_sprite; ++i) {
    if (time <= time_one_frame * i) {
      hero->current_idle = i;
      break;
    }
  }
  if (time > time_one_frame * hero->current_texture->amount_sprite) {
    hero->current_idle = 0;
    time = 0; 
  }
}

void collision_with_blocks_hero() {
  float coeficient_jerk = 1;
  if (keyboard[SDL_SCANCODE_LSHIFT])
    coeficient_jerk = hero->coefficient_jerk;
  SDL_Rect position_block = { 0, 0, level->real_size_edge_block, level->real_size_edge_block };
  int is_hero_standing = 0;
  for (int i = 0; i < level->amount_blocks.y; ++i) {
    for (int j = 0; j < level->amount_blocks.x; ++j) {
      if (level->map[i][j] != BLOCK_SPACE) {
        position_block.x = j * level->real_size_edge_block;
        position_block.y = i * level->real_size_edge_block;
        switch (collision_with_block(&hero->hitbox, &position_block)) {
          case COLLISION_LEFT: {
            hero->coordinates.x -= speed_dt(hero->speed * coeficient_jerk);
          } break;
          case COLLISION_RIGHT: {
            hero->coordinates.x += speed_dt(hero->speed * coeficient_jerk);
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