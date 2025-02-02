#include "../include/game.h"

void init_game(const char* save_file) {
  if (!save_file) { 
  }
}

void de_init_game() {

}

void game(const char* save_file) {
  init_game(save_file);
  // while (is_running_game) {
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

}

void draw_game_frame() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_RenderPresent(renderer);
}