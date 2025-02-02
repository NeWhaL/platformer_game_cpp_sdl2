#ifndef GAME_H
#define GAME_H

#include "sdl.h"
#include "global_var.h"
#include "menu.h"
#include "other_function.h"
#include "font.h"

void init_game(const char* save_file);
void game(const char* save_file = NULL);
void game_menu();
void updating_game_events();
void updating_game_logic();
void draw_game_frame();
void de_init_game();

#endif