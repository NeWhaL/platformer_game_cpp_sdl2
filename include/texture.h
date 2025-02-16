#ifndef TEXTURE_H
#define TEXTURE_H

#include "sdl.h"

struct Texture {
  SDL_Texture* texture;
  SDL_Rect size;
  int amount_sprite;
};

#endif