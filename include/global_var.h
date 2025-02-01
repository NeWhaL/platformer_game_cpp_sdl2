#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

#include "sdl.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Event events;
extern int window_width;
extern int window_heignt;
extern const Uint8* keyboard;
extern Uint32 mouse_buttons_state;
extern SDL_Point mouse_position;
extern double dt;

extern int is_running_main_menu;
extern int is_running_load_menu;
extern int is_running_game;
extern int is_running_stop_game_menu;
extern int is_running_upgrade_menu;
extern int is_running_save_menu;

#endif