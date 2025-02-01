#ifndef MENU_H
#define MENU_H

#include "sdl.h"
#include "global_var.h"
#include "font.h"
#include "other_function.h"

extern struct button {
  SDL_Color color_button;
  SDL_Texture* font;
  SDL_Rect size_button;
  SDL_Rect size_text;
} button_start, button_load, button_exit,
  button_load_1, button_load_2, button_load_3, button_load_back_main_menu;

extern SDL_Color current_color_button;
extern SDL_Color standart_color_button;
extern const char* font_path;
extern const int button_width;
extern const int button_height;

void menu();
void create_buttons();
void create_button(button* btn, const char* text, int x, int y);
void updating_events();
void logic_menu();
int button_collision(button* btn, SDL_Color std_color = standart_color_button, SDL_Color cur_color = current_color_button);
int button_collision_rect(SDL_Rect btn_rect);

void draw_menu();
void draw_buttons();
void draw_button(button* btn);

#endif