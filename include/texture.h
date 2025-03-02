#ifndef TEXTURE_H
#define TEXTURE_H

#include "sdl.h"

struct Sprite {
  SDL_Texture* sprite;
  SDL_Rect size;
  double rendering_time;
};

struct Texture {
  Sprite* sprites;
  int amount_sprite;
  int repeat_animation;
};

#endif