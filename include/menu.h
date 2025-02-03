#ifndef MENU_H
#define MENU_H

#include "sdl.h"
#include "global_var.h"
#include "font.h"
#include "other_function.h"
#include "background.h"
#include "game.h"

extern SDL_Color standart_background_color;
extern SDL_Color current_color_button;
extern SDL_Color standart_color_button;
extern const char* font_path;
extern const int button_width;
extern const int button_height;
extern const int left_margin;
extern const int up_margin;
extern const int amount_buttons_in_main_menu;
extern const int amount_buttons_in_load_menu;
extern const int amount_buttons_in_stop_menu;
extern const int amount_buttons_in_save_menu;

enum button_type {
  BUTTON_START,
  BUTTON_LOAD_MENU,
  BUTTON_EXIT,
  BUTTON_LOAD,
  BUTTON_CONTINUE,
  BUTTON_SAVE_MENU,
  BUTTON_SAVE,
  BUTTON_UPGRADE,
  BUTTON_CHANGE_SKIN,
  BUTTON_MAIN_MENU,
  BUTTON_STOP_GAME_MENU
};

extern struct button {
  SDL_Color standart_color_button;
  SDL_Color active_color_button;
  SDL_Color color_button;
  SDL_Texture* font;
  SDL_Rect size_button;
  SDL_Rect size_text;
  button_type type;
} *buttons_main_menu, 
  *buttons_load_menu, 
  *buttons_stop_menu,
  *buttons_save_menu;

void init_main_menu();
void init_game_menu();

void buttons_malloc(button** buttons, int size);
void create_button(button* btn, button_type type, const char* text, int x, int y,
                   int w = button_width, int h = button_height, 
                   SDL_Color std_color = standart_color_button,
                   SDL_Color cur_color = current_color_button);
void create_buttons(button* buttons, button_type* types, const char** names, const int amount_buttons);
int button_collision(button* btn);
int button_collision_rect(SDL_Rect btn_rect);
void draw_button(button* btn);
void draw_menu(void draw_buttons_menu(), SDL_Color background_color = standart_background_color);

void main_menu();
void create_main_menu_buttons();
void updating_main_menu_events(int* is_mouse_button_left);
void logic_main_menu(int is_mouse_button_left);
void draw_main_menu_buttons();
void de_init_main_menu();

void game_menu();
void create_game_menu_buttons();
void updating_game_menu_events(int* is_mouse_button_left);
void logic_game_menu(int is_mouse_button_left);
void draw_game_menu_buttons();
void de_init_game_menu();

#endif