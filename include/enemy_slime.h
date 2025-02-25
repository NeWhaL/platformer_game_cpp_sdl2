#ifndef ENEMY_SLIME_H
#define ENEMY_SLIME_H

#include "enemy_base.h"

struct Enemy_slime {
  Enemy_base base;
};

void move_enemy_slime(Enemy_slime* enemy);
void collision_enemy_slime_with_hero(Enemy_slime* enemy);

#endif