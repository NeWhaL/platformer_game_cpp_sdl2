#ifndef OTHER_FUNCTION_H
#define OTHER_FUNCTION_H

#include "application.h"
#include "global_var.h"

void updating_mouse_state();
void updating_dt();
float speed_dt(float speed);
SDL_Texture* load_texture_from_file(const char* filename, SDL_Rect* size = NULL);

#endif