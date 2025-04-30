#include "../include/application.h"


void init_main_cycle() {
  init_backgrounds();
  init_main_menu();
  init_game_menu();
}

void de_init_main_cycle() {
  de_init_game_menu();
  de_init_main_menu();
  de_init_backgrounds();
}

void application() {
  init_main_cycle();
  Save_number number = CONTINUE_GAME;
  while (is_running == MAIN_MENU ||
         is_running == LOAD_MENU ||
         is_running == GAME ||
         is_running == STOP_GAME_MENU ||
         is_running == UPGRADE_MENU ||
         is_running == SAVE_MENU) {
    if (is_running == MAIN_MENU || is_running == LOAD_MENU)
      number = main_menu();
    else if (is_running == GAME) {
      game(number);
      number = CONTINUE_GAME;
    } 
    else if (is_running == STOP_GAME_MENU ||
             is_running == UPGRADE_MENU ||
             is_running == SAVE_MENU ||
             is_running == CHANGE_SKIN_MENU)
      game_menu();
  }
  de_init_main_cycle();
}