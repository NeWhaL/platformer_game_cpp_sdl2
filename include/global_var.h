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

extern enum what_is_running {
  NO_IS_RUNNING,
  MAIN_MENU,
  LOAD_MENU,
  GAME,
  STOP_GAME_MENU,
  UPGRADE_MENU,
  SAVE_MENU,
  CHANGE_SKIN_MENU
} is_running;

#endif