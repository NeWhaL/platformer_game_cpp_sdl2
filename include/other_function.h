#ifndef OTHER_FUNCTION_H
#define OTHER_FUNCTION_H

#include "application.h"
#include "global_var.h"

enum collision_direction {
  COLLISION_NONE,
  COLLISION_UP,
  COLLISION_RIGHT,
  COLLISION_DOWN,
  COLLISION_LEFT
};

void updating_mouse_state();
void updating_dt();
float speed_dt(float speed);
SDL_Texture* load_texture_from_file(const char* filename, SDL_Rect* size = NULL);
collision_direction collision_with_block(const SDL_Rect *hitbox_anyone, const SDL_Rect* hitbox_block);
void synchronize_hitbox_with_coordinates(SDL_Rect* hitbox, SDL_FPoint coordinates);
void gravity(SDL_FPoint* coordinates);

#endif