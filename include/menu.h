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
extern const int button_width;
extern const int button_height;
extern const int left_margin;
extern const int up_margin;
extern const int amount_buttons_in_main_menu;
extern const int amount_buttons_in_load_menu;
extern const int amount_buttons_in_stop_menu;
extern const int amount_buttons_in_save_menu;
extern const int amount_buttons_in_change_skin_menu;
extern const int amount_buttons_in_upgrade_menu;
extern const int cost_of_improvement;

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
  BUTTON_STOP_GAME_MENU,
  BUTTON_SKIN_RED_KNIGHT,
  BUTTON_SKIN_GREEN_KNIGHT,
  BUTTON_UPGRADE_HEALTH,
  BUTTON_UPGRADE_SPEED,
  BUTTON_UPGRADE_PURE_DAMAGE,
  BUTTON_UPGRADE_JUMP_HEIGHT,
  BUTTON_UPGRADE_SHOT_ORDINARY,
  BUTTON_UPGRADE_SHOT_HOMING
};

extern struct Button {
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
  *buttons_save_menu,
  *buttons_change_skin_menu,
  *buttons_upgrade_menu;

void init_main_menu();
void init_game_menu();

void buttons_malloc(Button** buttons, int size);
void create_button(Button* btn, button_type type, const char* text, int x, int y,
                   int w = button_width, int h = button_height, int font_size = standart_font_size, 
                   SDL_Color std_color = standart_color_button,
                   SDL_Color cur_color = current_color_button);
void create_buttons(Button* buttons, button_type* types, const char** names, const int amount_buttons,
                    int up_margin_between_buttons = up_margin, int left_margin = left_margin, int button_width = button_width, 
                    int button_height = button_height, int font_size = standart_font_size);
int button_collision(Button* btn);
int button_collision_rect(SDL_Rect btn_rect);
void draw_button(Button* btn);
void draw_buttons(Button* buttons, int amount_button);
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
void draw_characteristics_hero();
void draw_something_characteristics_hero(const char* characteristics_name, double value, int x, int y);
void de_init_game_menu();

void subtract_experience_hero();

#endif