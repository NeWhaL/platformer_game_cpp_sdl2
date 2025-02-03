#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "sdl.h"
#include "global_var.h"
#include "other_function.h"
#include <string.h>

enum background_type {
 BACKGROUND_NONE,
 BACKGROUND_LEVEL_1,
 BACKGROUND_LEVEL_2,
 BACKGROUND_LEVEL_3,
 BACKGROUND_MAIN_MENU,
 BACKGROUND_GAME_MENU
};

extern struct background {
  SDL_Color color;
  SDL_Texture* texture = NULL;
  SDL_Rect size = { 0, 0, 0, 0 };
  background_type type;
} *backgrounds;
extern const int amount_backgrounds;

void init_backgrounds();
void de_init_backgrounds();
void create_background(background* b, const char* filename, background_type type, SDL_Color color = { 0, 0, 0, 255 });
void destroy_background(background* b);
void draw_background(background* b);

#endif