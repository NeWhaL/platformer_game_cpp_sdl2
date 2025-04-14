#ifndef FONT_H
#define FONT_H

#include "global_var.h"

extern const char* font_path;
extern const int standart_font_size;

SDL_Texture* create_font(const char* font_path, const char* text, SDL_Color color_text, SDL_Rect* size_text, SDL_Rect size_button, 
                         int standart_font_size = standart_font_size);

#endif