#include "../include/application.h"

void init_application() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Не удалось инициализировать SDL2: %s", SDL_GetError());
    de_init_application(1);
  }
  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
    printf("Не удалось инициализировать SDL_image: %s", IMG_GetError());
    de_init_application(1);
    exit(1);
  }
  if (TTF_Init()) {
    printf("Не удалось инициализировать SDL_ttf: %s", TTF_GetError());
    de_init_application(1);
  }
  window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, 
            SDL_WINDOW_SHOWN | 
            SDL_WINDOW_RESIZABLE | 
            SDL_WINDOW_FULLSCREEN);
  if (!window) {
    printf("Не удалось инициализировать окно: %s", SDL_GetError());
    SDL_Quit();
    exit(1);
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    printf("Не удалось инициализировать средство визуализации: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(1);
  }
}

void de_init_application(int error) {
  if (!renderer)
    SDL_DestroyRenderer(renderer);
  if (!window)
    SDL_DestroyWindow(window);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  exit(error);
}

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
  while (is_running == MAIN_MENU ||
         is_running == LOAD_MENU ||
         is_running == GAME ||
         is_running == STOP_GAME_MENU ||
         is_running == UPGRADE_MENU ||
         is_running == SAVE_MENU) {
    if (is_running == MAIN_MENU || is_running == LOAD_MENU)
      main_menu();
    else if (is_running == GAME)
      game();
    else if (is_running == STOP_GAME_MENU ||
             is_running == UPGRADE_MENU ||
             is_running == SAVE_MENU ||
             is_running == CHANGE_SKIN_MENU)
      game_menu();
  }
  de_init_main_cycle();
}