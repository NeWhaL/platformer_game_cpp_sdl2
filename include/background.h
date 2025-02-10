#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "sdl.h"
#include "global_var.h"
#include "other_function.h"
#include <string.h>

enum Background_type {
 BACKGROUND_NONE = -1,
 BACKGROUND_LEVEL_1,
 BACKGROUND_LEVEL_2,
 BACKGROUND_LEVEL_3,
 BACKGROUND_MAIN_MENU,
 BACKGROUND_GAME_MENU
};

extern struct Background {
  SDL_Color color;
  SDL_Texture* texture = NULL;
  SDL_Rect size = { 0, 0, 0, 0 };
  Background_type type;
} *backgrounds;
extern const int amount_backgrounds;

void init_backgrounds();
void de_init_backgrounds();
void create_background(Background* b, const char* filename, Background_type type, SDL_Color color = { 0, 0, 0, 255 });
void destroy_background(Background* b);
void draw_background(Background* b);

#endif