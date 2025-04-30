#ifndef GAME_H
#define GAME_H

#include "sdl.h"
#include "global_var.h"
#include "menu.h"
#include "other_function.h"
#include "font.h"
#include "hero.h"
#include "level.h"
#include "enemy_base.h"
#include "load_save.h"

void init_game(Save_number number = NEW_GAME);
void game(Save_number number = NEW_GAME);
void game_menu();
void updating_game_events();
void updating_game_logic();
void draw_game_frame();
void de_init_game();
int collision_with_next_level_block_hero();
void next_level();

extern int is_init_game;

#endif