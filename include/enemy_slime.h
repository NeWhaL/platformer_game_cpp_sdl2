#ifndef ENEMY_SLIME_H
#define ENEMY_SLIME_H

#include "enemy_base.h"
#include "hero.h"

enum Enemy_slime_state {
  ENEMY_SLIME_WALK,
  ENEMY_SLIME_ATTACK,
  ENEMY_SLIME_DEATH,
  ENEMY_SLIME_AMOUNT_STATE
};

struct Enemy_slime {
  Enemy_base base;
  Enemy_slime_state prev_state; //нужно для того, чтобы была возможность изменить текущую текстуру.
  Enemy_slime_state current_state;
};

void update_enemy_slime(Enemy_slime* enemy);
void move_enemy_slime(Enemy_slime* enemy);
void slime_attack_on_the_hero(Enemy_slime* enemy);
int collision_enemy_slime_with_hero(Enemy_slime* enemy);
void set_current_sprite_enemy_slime(Enemy_slime* enemy);
void determine_current_texture_enemy_slime(Enemy_slime* enemy);
void death_enemy_slime(Enemy_slime* enemy);
void determine_current_state_enemy_slime(Enemy_slime* enemy);
void draw_enemy_slime(Enemy_slime* enemy);

#endif