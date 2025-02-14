#include "../include/global_var.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event events;
int window_width = 1920;
int window_heignt = 1080;
const Uint8* keyboard = SDL_GetKeyboardState(NULL);
Uint32 mouse_buttons_state;
SDL_Point mouse_position;
double dt;
const double speed_gravity = 100;
what_is_running is_running = MAIN_MENU;