#ifndef OTHER_FUNCTION_H
#define OTHER_FUNCTION_H

// #include "application.h"
#include "direction.h"
#include "sdl.h"
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
collision_direction collision_of_two_objects(const SDL_Rect *hitbox_anyone, const SDL_Rect* hitbox_block);
void synchronize_hitbox_with_coordinates(SDL_Rect* hitbox, SDL_FPoint coordinates);
void gravity(SDL_FPoint* coordinates);
double read_from_file_double(FILE* file);
float the_distance_between_the_centers_of_two_rect(SDL_Rect* rect_1, SDL_Rect* rect_2);

#endif