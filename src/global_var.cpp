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

int is_running_main_menu = 1;
int is_running_load_menu = 0;
int is_running_game = 0;
int is_running_stop_game_menu = 0;
int is_running_upgrade_menu = 0;
int is_running_save_menu = 0;