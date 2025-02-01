#include "../include/font.h"

SDL_Texture* create_font(const char* font_path, const char* text, SDL_Color color_text, SDL_Rect* size_text, SDL_Rect size_button) {
  TTF_Font* font = TTF_OpenFont(font_path, 75);
  SDL_Surface* ttf_surface = TTF_RenderText_Blended(font, text, color_text);
  *size_text = { 
    size_button.x + size_button.w / 2- ttf_surface->w / 2, 
    size_button.y + size_button.h / 2 - ttf_surface->h / 2,
    ttf_surface->w, 
    ttf_surface->h 
  };
  SDL_Texture* ttf_texture = SDL_CreateTextureFromSurface(renderer, ttf_surface);
  TTF_CloseFont(font);
  SDL_FreeSurface(ttf_surface);
  return ttf_texture;
}