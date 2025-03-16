#include "../include/shot.h"

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
  for (Shot* shot_delete = shots.head; shot_delete != NULL; shot_delete = shots.head) {
    shots.head = shots.head->next;
    destroy_shot(shot_delete);
  }
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

Shot* create_shot(Shot_type type, Shot_creator creator, SDL_FPoint coordinates, float range, direction_movement direction) {
  Shot* shot = (Shot*)malloc(sizeof(Shot));
  shot->current_state = SHOT_STATE_MOVE;
  shot->prev_state = shot->current_state;
  shot->creator = creator;
  shot->type = type;
  shot->coordinates = coordinates;
  shot->range = range;
  shot->direction = direction;
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
                                 Shot_type shot_type, direction_movement direction, float range) {
  Shot* new_shot = create_shot(shot_type, creator_type, coordinates_spawn, range, direction);
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
        collision_shot_with_blocks(current);
        determine_current_texture_shot(current);
        set_current_sprite_shot(current);
        death_shot(current);
      } break;
      case SHOT_TYPE_HOMING: {
        // move_homing_shot(current);
      } break;
    }
  }
}

void move_ordinary_shot(Shot* shot) {
  if (shot->current_state == SHOT_STATE_DEATH)
    return;
  switch (shot->direction) {
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
}

void draw_shots() {
  for (Shot* current = shots.head; current != NULL; current = current->next)
    render_copy_shot(current);
}

void render_copy_shot(Shot* shot) {
  SDL_RendererFlip flip = SDL_FLIP_NONE;
  SDL_RenderCopyEx(
    renderer,
    shot->texture.current->sprites[shot->texture.current_number_sprite].sprite,
    NULL,
    &shot->hitbox,
    0, NULL, flip 
  );
}

void set_current_sprite_shot(Shot* shot) {
  set_current_sprite(
    shot->texture.current,
    &shot->texture.current_number_sprite,
    &shot->hitbox,
    &shot->coordinates,
    shot->direction,
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