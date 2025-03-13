#ifndef ENEMY_SHOOTER_H
#define ENEMY_SHOOTER_H

#include "enemy_base.h"

enum Enemy_shooter_state {
  ENEMY_SHOOTER_AMOUNT_STATE
};

struct Enemy_shooter {
  Enemy_base base;
  Enemy_shooter_state prev_state;
  Enemy_shooter_state current_state;
};

void update_enemy_shooter(Enemy_shooter* enemy);
void move_enemy_shooter(Enemy_shooter* enemy);
void shooter_attack_on_the_hero(Enemy_shooter* enemy);
int collision_enemy_shooter_with_hero(Enemy_shooter* enemy);
void set_current_sprite_enemy_shooter(Enemy_shooter* enemy);
void determine_current_texture_enemy_shooter(Enemy_shooter* enemy);
void death_enemy_shooter(Enemy_shooter* enemy);
void determine_current_state_enemy_shooter(Enemy_shooter* enemy);

#endif // ENEMY_SHOOTER