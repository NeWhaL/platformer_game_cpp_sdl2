#ifndef ENEMY_SLIME_H
#define ENEMY_SLIME_H

#include "enemy_base.h"

enum Enemy_slime_state {
  ENEMY_SLIME_WALK,
  ENEMY_SLIME_ATTACK,
  ENEMY_SLIME_DEATH,
  ENEMY_SLIME_AMOUNT_STATE
};

struct Enemy_slime {
  Enemy_base base;
  Enemy_slime_state state;
};

void move_enemy_slime(Enemy_slime* enemy);
void collision_enemy_slime_with_hero(Enemy_slime* enemy);

#endif