#ifndef ENEMY_SKELETON_H
#define ENEMY_SKELETON_H

#include "enemy_base.h"

enum Enemy_skeleton_state {
  ENEMY_SKELETON_IDLE,
  ENEMY_SKELETON_WALK,
  ENEMY_SKELETON_ATTACK,
  ENEMY_SKELETON_HURT,
  ENEMY_SKELETON_DEATH,
  ENEMY_SKELETON_AMOUNT_STATE
};

struct Enemy_skeleton {
  Enemy_base base;
  Enemy_skeleton_state prev_state;
  Enemy_skeleton_state current_state;
  float reaction_range_walk;
  double sprites_timer;
  float prev_health; 
};

void update_enemy_skeleton(Enemy_skeleton* enemy);
void move_enemy_skeleton(Enemy_skeleton* enemy);
void skeleton_attack_on_the_hero(Enemy_skeleton* enemy);
int collision_enemy_skeleton_with_hero(Enemy_skeleton* enemy);
void set_current_sprite_enemy_skeleton(Enemy_skeleton* enemy);
void determine_current_texture_enemy_skeleton(Enemy_skeleton* enemy);
void death_enemy_skeleton(Enemy_skeleton* enemy);
void determine_current_state_enemy_skeleton(Enemy_skeleton* enemy);
float get_distance_reaction_attack_skeleton(Enemy_skeleton* enemy);
direction_movement determine_direction_movement_enemy_skeleton(Enemy_skeleton* enemy);
void check_for_damage_enemy_skeleton(Enemy_skeleton* enemy);
void draw_enemy_skeleton(Enemy_skeleton* enemy);

#endif