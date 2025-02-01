#ifndef FONT_H
#define FONT_H

#include "global_var.h"

SDL_Texture* create_font(const char* font_path, const char* text, SDL_Color color_text, SDL_Rect* size_text, SDL_Rect size_button);

#endif