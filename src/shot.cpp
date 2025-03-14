#include "../include/shot.h"

Shot_container shots;

void init_shot_container() {
  shots.all = NULL;
  shots.textures = NULL;
  shots.amount_shots = 0;
  shots.capacity = 0;
  shots.coefficient_capacity = 2;
  init_shot_textures();
}

void de_init_shot_container() {
  de_init_shot_textures();
  for (int i = 0; i < shots.amount_shots; ++i)
    free(shots.all[i]);
  free(shots.all);
}

void init_shot_textures() {
  shots.textures = (Texture**)malloc(sizeof(Texture*) * SHOT_TYPE_AMOUNT);
  shots.textures[SHOT_TYPE_ORDINARY] = (Texture*)malloc(sizeof(Texture) * SHOT_STATE_AMOUNT);
  init_texture(&shots.textures[SHOT_TYPE_ORDINARY][SHOT_STATE_MOVE], "../game_images/shot/ordinary_shot/fly/");
  init_texture(&shots.textures[SHOT_TYPE_ORDINARY][SHOT_STATE_DEATH], "../game_images/shot/ordinary_shot/death/");
  shots.textures[SHOT_TYPE_HOMING] = (Texture*)malloc(sizeof(Texture) * SHOT_TYPE_AMOUNT);
  init_texture(&shots.textures[SHOT_TYPE_HOMING][SHOT_STATE_MOVE], "../game_images/shot/ordinary_shot/fly/");
  init_texture(&shots.textures[SHOT_TYPE_HOMING][SHOT_STATE_DEATH], "../game_images/shot/ordinary_shot/death/");
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
  shot->creator = creator;
  shot->type = type;
  shot->coordinates = coordinates;
  shot->range = range;
  shot->direction = direction;
  shot->texture.current_number_sprite = 0;
  shot->texture.sprite_time_counter = 0;
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
  return shot;
}

void destroy_shot(Shot* shot) {
  free(shot);
}

void add_shot_in_shots_container(SDL_FPoint coordinates_spawn, Shot_creator creator_type, 
                                 Shot_type shot_type, direction_movement direction, float range) {
  if (shots.all == NULL) {
    shots.capacity = shots.coefficient_capacity;
    shots.all = (Shot**)malloc(sizeof(Shot*) * shots.capacity);
  }
  else if (shots.capacity == shots.amount_shots) {
    shots.capacity *= shots.coefficient_capacity;
    shots.all = (Shot**)realloc(shots.all, sizeof(Shot*) * shots.capacity);
  }
  Shot* shot = create_shot(shot_type, creator_type, coordinates_spawn, range, direction);
  shots.all[shots.amount_shots++] = shot;
}

void move_ordinary_shot(Shot* shot) {
  switch (shot->direction) {
    case DIRECTION_LEFT: {
      shot->coordinates.x -= speed_dt(shot->speed);
    } break;
    case DIRECTION_RIGHT: {
      shot->coordinates.x += speed_dt(shot->speed);
    } break;
  }
}

void move_homing_shot(Shot* shot) {

}

void draw_shots() {
  for (int i = 0; i < shots.amount_shots; ++i)
    render_copy_shot(shots.all[i]);
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