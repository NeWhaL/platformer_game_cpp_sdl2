#include "../include/shot.h"
#include "../include/hero.h"

Shot_container shots;

void init_shot_container() {
  shots.head = NULL;
  shots.tail = shots.head;
  shots.textures = NULL;
  init_shot_textures();
}

void init_shot_textures() {
  shots.textures = (Texture**)malloc(sizeof(Texture*) * SHOT_TYPE_AMOUNT);
  shots.textures[SHOT_TYPE_ORDINARY] = (Texture*)malloc(sizeof(Texture) * SHOT_STATE_AMOUNT);
  init_texture(&shots.textures[SHOT_TYPE_ORDINARY][SHOT_STATE_MOVE], "../game_images/shots/ordinary_shot/fly/");
  init_texture(&shots.textures[SHOT_TYPE_ORDINARY][SHOT_STATE_DEATH], "../game_images/shots/ordinary_shot/death/");
  shots.textures[SHOT_TYPE_HOMING] = (Texture*)malloc(sizeof(Texture) * SHOT_TYPE_AMOUNT);
  init_texture(&shots.textures[SHOT_TYPE_HOMING][SHOT_STATE_MOVE], "../game_images/shots/ordinary_shot/fly/");
  init_texture(&shots.textures[SHOT_TYPE_HOMING][SHOT_STATE_DEATH], "../game_images/shots/ordinary_shot/death/");
}

void de_init_shot_container() {
  de_init_shot_textures();
  for (Shot* shot_delete = shots.head; shot_delete != NULL; shot_delete = shots.head)
    destroy_shot(shot_delete);
  shots.head = shots.tail = NULL;
}

void de_init_shot_textures() {
  for (int i = 0; i < SHOT_TYPE_AMOUNT; ++i) {
    for (int j = 0; j < SHOT_STATE_AMOUNT; ++j)
      de_init_texture(&shots.textures[i][j]);
    free(shots.textures[i]);
  }
  free(shots.textures);
}

Shot* create_shot(Shot_type type, Shot_creator creator, SDL_FPoint coordinates, float range, float damage, float speed, direction_movement direction) {
  Shot* shot = (Shot*)malloc(sizeof(Shot));
  shot->current_state = SHOT_STATE_MOVE;
  shot->prev_state = shot->current_state;
  shot->creator = creator;
  shot->type = type;
  shot->coordinates = coordinates;
  shot->range = range;
  shot->damage = damage;
  switch (type) {
    case SHOT_TYPE_ORDINARY: {
      shot->direction.ordinary = direction;
    } break;
    case SHOT_TYPE_HOMING: {
      switch (direction) {
        case DIRECTION_RIGHT: {
          shot->direction.homing = {1, 0};
        } break;
        case DIRECTION_LEFT: {
          shot->direction.homing = {-1, 0};
        } break;
      }
    };
  }
  shot->speed = 250;
  shot->texture.current_number_sprite = 0;
  shot->texture.sprite_time_counter = 0;
  shot->death_time = 0;
  switch (type) {
    case SHOT_TYPE_ORDINARY: {
      shot->texture.current = &shots.textures[SHOT_TYPE_ORDINARY][SHOT_STATE_MOVE];
    } break;
    case SHOT_TYPE_HOMING: {
      shot->texture.current = &shots.textures[SHOT_TYPE_HOMING][SHOT_STATE_MOVE];
    } break;
  }
  shot->hitbox.w = shot->texture.current->sprites[0].size.w;
  shot->hitbox.h = shot->texture.current->sprites[0].size.h;
  shot->next = shot->prev = NULL;
  synchronize_hitbox_with_coordinates(&shot->hitbox, shot->coordinates);
  return shot;
}

void destroy_shot(Shot* shot) {
  if (shot->next == NULL && shot->prev == NULL) {
    shots.head = shots.tail = NULL;
  } else if (shot->next == NULL) {
    shots.tail = shots.tail->prev;
    shots.tail->next = NULL;
  } else if (shot->prev == NULL) {
    shots.head = shots.head->next;
    shots.head->prev = NULL;
  } else {
    shot->next->prev = shot->prev;
    shot->prev->next = shot->next;
  }
  free(shot);
}

void add_shot_in_shots_container(SDL_FPoint coordinates_spawn, Shot_creator creator_type, 
                                 Shot_type shot_type, direction_movement direction, 
                                 float range, float damage, float speed) {
  Shot* new_shot = create_shot(shot_type, creator_type, coordinates_spawn, range, damage, speed, direction);
  if (!shots.head) {
    shots.head = shots.tail = new_shot;
  } else if (shots.head == shots.tail) {
    shots.tail->next = new_shot;
    shots.tail = shots.tail->next;
    shots.tail->prev = shots.head;
  } else {
    new_shot->prev = shots.tail;
    shots.tail->next = new_shot;
    shots.tail = shots.tail->next;
  }
}

void update_shots() {
  for (Shot* current = shots.head; current != NULL; current = current->next) {
    switch (current->type) {
      case SHOT_TYPE_ORDINARY: {
        move_ordinary_shot(current);
      } break;
      case SHOT_TYPE_HOMING: {
        determine_direction_homing_shot(current);
        move_homing_shot(current);
      } break;
    }
    update_range_shot(current);
    collision_shot_with_platforms(current);
    collision_shot_with_blocks(current);
    collision_shot_with_enemies(current);
    collision_shot_with_hero(current);
    determine_current_texture_shot(current);
    set_current_sprite_shot(current);
    death_shot(current);
  }
}

void update_range_shot(Shot* shot) {
  if (shot->current_state == SHOT_STATE_DEATH)
    return;
  shot->range -= speed_dt(shot->speed);
  if (shot->range <= 0)
    shot->current_state = SHOT_STATE_DEATH;
}

void move_ordinary_shot(Shot* shot) {
  if (shot->current_state == SHOT_STATE_DEATH)
    return;
  switch (shot->direction.ordinary) {
    case DIRECTION_LEFT: {
      shot->coordinates.x -= speed_dt(shot->speed);
    } break;
    case DIRECTION_RIGHT: {
      shot->coordinates.x += speed_dt(shot->speed);
    } break;
  }
  synchronize_hitbox_with_coordinates(&shot->hitbox, shot->coordinates);
}

void move_homing_shot(Shot* shot) {
  if (shot->current_state == SHOT_STATE_DEATH)
    return;
  shot->coordinates.x += speed_dt(shot->speed) * shot->direction.homing.x;
  shot->coordinates.y += speed_dt(shot->speed) * shot->direction.homing.y;
  synchronize_hitbox_with_coordinates(&shot->hitbox, shot->coordinates);
}

void determine_direction_homing_shot(Shot* current) {
  double target_distance = INT32_MAX;
  SDL_FPoint target_coordinates = { 0, 0 };
  switch (current->creator) {
    case SHOT_CREATOR_HERO: {
      for (int i = 0 ; i < enemy_container->amount_enemies; ++i) {
        if (enemy_container->enemies[i]->type == ENEMY_INACTIVE)
          continue;
        double current_distance = the_distance_between_the_centers_of_two_rect(&enemy_container->enemies[i]->hitbox, &current->hitbox);
        if (target_distance > current_distance) {
          target_distance = current_distance;
          target_coordinates = enemy_container->enemies[i]->coordinates;
        }
      }
      if (target_coordinates.x == 0 && target_coordinates.y == 0)
        return;
    } break;
    case SHOT_CREATOR_ENEMY: {
      target_coordinates = hero->coordinates;
      target_distance = the_distance_between_the_centers_of_two_rect(&hero->hitbox, &current->hitbox);
    } break;
  }
  current->direction.homing.x = (target_coordinates.x - current->coordinates.x) / target_distance;  
  current->direction.homing.y = (target_coordinates.y - current->coordinates.y) / target_distance;  
}

void draw_shots() {
  for (Shot* current = shots.head; current != NULL; current = current->next)
    render_copy_shot(current);
}

void render_copy_shot(Shot* shot) {
  SDL_RendererFlip flip = SDL_FLIP_NONE;
  switch (shot->type) {
    case SHOT_TYPE_ORDINARY: {
      if (shot->direction.ordinary == DIRECTION_LEFT)
        flip = SDL_FLIP_HORIZONTAL;
    } break;
    case SHOT_TYPE_HOMING: {
      if (shot->direction.homing.x < 0)
        flip = SDL_FLIP_HORIZONTAL;
    } break;
  }
  SDL_RenderCopyEx(
    renderer,
    shot->texture.current->sprites[shot->texture.current_number_sprite].sprite,
    NULL,
    &shot->hitbox,
    0, NULL, flip 
  );
}

void set_current_sprite_shot(Shot* shot) {
  direction_movement current_direction = DIRECTION_NONE;
  switch (shot->type) {
    case SHOT_TYPE_ORDINARY: {
      current_direction = shot->direction.ordinary;
    } break;
    case SHOT_TYPE_HOMING: {
      current_direction = DIRECTION_LEFT;
    } break;
  }
  set_current_sprite(
    shot->texture.current,
    &shot->texture.current_number_sprite,
    &shot->hitbox,
    &shot->coordinates,
    current_direction,
    &shot->texture.sprite_time_counter,
    shot->current_state != shot->prev_state
  );
  if (shot->current_state != shot->prev_state)
    shot->prev_state = shot->current_state;
}

void collision_shot_with_blocks(Shot* shot) {
  if (shot->current_state == SHOT_STATE_DEATH)
    return;
  SDL_Rect position_block = { 0, 0, level->real_size_edge_block, level->real_size_edge_block };
  for (int i = 0; i < level->amount_blocks.y; ++i) {
    for (int j = 0; j < level->amount_blocks.x; ++j) {
      if (is_it_a_block(Blocks(level->map[i][j])))
        continue;
      position_block.x = j * level->real_size_edge_block;
      position_block.y = i * level->real_size_edge_block;
      if (collision_of_two_objects(&shot->hitbox, &position_block))
        shot->current_state = SHOT_STATE_DEATH;
    }
  }
  synchronize_hitbox_with_coordinates(&hero->hitbox, hero->coordinates);
}

void determine_current_texture_shot(Shot* shot) {
  if (shot->prev_state == shot->current_state) {
    return;
  } else {
    shot->texture.current = &shots.textures[shot->type][shot->current_state]; 
    shot->texture.current_number_sprite = 0;
  }
}

void death_shot(Shot* shot) {
  if (shot->current_state == SHOT_STATE_DEATH)
    shot->death_time += dt;
  if (shot->death_time >= time_for_one_texture_iteration(shot->texture.current)) {
    destroy_shot(shot);
  }
}

void collision_shot_with_platforms(Shot* shot) {
  if (shot->current_state == SHOT_STATE_DEATH)
    return;
  for (int i = 0; i < amount_platforms; ++i) {
    if (collision_of_two_objects(&shot->hitbox, &platforms[i].hitbox)) {
      shot->current_state = SHOT_STATE_DEATH;
      return;
    }
  }
}

void collision_shot_with_enemies(Shot* shot) {
  if (shot->current_state == SHOT_STATE_DEATH || shot->creator == SHOT_CREATOR_ENEMY)
    return;
  for (int i = 0; i < enemy_container->amount_enemies; ++i) {
    Enemy_base* enemy = enemy_container->enemies[i];
    if (enemy->type == ENEMY_INACTIVE)
      continue;
    if (collision_of_two_objects(&shot->hitbox, &enemy->hitbox)) {
      shot->current_state = SHOT_STATE_DEATH;
      enemy->health -= shot->damage; 
    }
  }
}

void collision_shot_with_hero(Shot* shot) {
  if (shot->current_state == SHOT_STATE_DEATH || shot->creator == SHOT_CREATOR_HERO)
    return;
  if (collision_of_two_objects(&hero->hitbox, &shot->hitbox)) {
    shot->current_state = SHOT_STATE_DEATH;
    hero->health -= shot->damage;
  }
}