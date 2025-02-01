#ifndef INIT_APPLICATION_H
#define INIT_APPLICATION_H

#include <stdio.h>
#include "sdl.h"
#include "global_var.h"
#include "menu.h"
#include "game.h"

void init_application();
void de_init_application(int error = 0);
void application();

#endif