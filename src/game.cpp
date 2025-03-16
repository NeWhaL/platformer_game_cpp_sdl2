#include "../include/game.h"

int is_init_game = 1;

void init_game(const char* save_file) {
  if (!is_init_game)
    return;
  if (!save_file) { 
    init_level(LEVEL_1);
    init_hero(); 
    // init_enemies(LEVEL_1);
    init_enemies();
    init_shot_container();
    is_init_game = 0;
  } else {
    load_level(save_file);
    load_hero(save_file);
    // is_init_game = 0;
  }
  is_init_game = 0;
}

void de_init_game() {
  de_init_enemies();
  de_init_hero();
  de_init_level();
  de_init_shot_container();
}

void game(const char* save_file) {
  init_game(save_file);
  while (is_running == GAME) {
    updating_game_events();
    updating_game_logic();
    draw_game_frame();
  }
}

void updating_game_events() {
	while (SDL_PollEvent(&events)) {
		switch (events.type) {
      case SDL_QUIT: {
        is_running = NO_IS_RUNNING;
      } break;
		case SDL_KEYDOWN:
			switch (events.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE: {
          is_running = STOP_GAME_MENU;
        } break;
			}
		}
	}
}

void updating_game_logic() {
  updating_dt();
  updating_level();
  updating_enemies();
  update_shots();
  update_hero();
}

void draw_game_frame() {
  draw_level();
  draw_enemies();
  draw_hero();
  draw_shots();
  SDL_RenderPresent(renderer);
}