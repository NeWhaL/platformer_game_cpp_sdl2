#ifndef GAME_H
#define GAME_H

#include "sdl.h"
#include "global_var.h"
#include "menu.h"
#include "other_function.h"
#include "font.h"

void game(const char* save_file = NULL);
void game_menu();

#endif