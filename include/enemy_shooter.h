#ifndef ENEMY_SHOOTER_H
#define ENEMY_SHOOTER_H

#include "enemy_base.h"
#include "shot.h"
#include "hero.h"

enum Enemy_shooter_state {
  ENEMY_SHOOTER_IDLE,
  ENEMY_SHOOTER_WALK,
  ENEMY_SHOOTER_SHOT_ATTACK,
  ENEMY_SHOOTER_HURT,
  ENEMY_SHOOTER_DEATH,
  ENEMY_SHOOTER_AMOUNT_STATE
};

struct Enemy_shooter {
  Enemy_base base;
  Enemy_shooter_state prev_state;
  Enemy_shooter_state current_state;
  double sprites_timer;
  int was_the_shot_fired;
  int sprite_number_for_attack;
  float prev_health;
  float responce_radius;
  float attack_radius;
  struct {
    struct {
      Shot_type type;
      float speed;
      float damage;
      float range;
    } shot;
  } attack;
  SDL_FPoint float_direction;
};

void update_enemy_shooter(Enemy_shooter* enemy);
void move_enemy_shooter(Enemy_shooter* enemy);
void shooter_attack_on_the_hero(Enemy_shooter* enemy);
int collision_enemy_shooter_with_hero(Enemy_shooter* enemy);
void set_current_sprite_enemy_shooter(Enemy_shooter* enemy);
void determine_current_texture_enemy_shooter(Enemy_shooter* enemy);
void death_enemy_shooter(Enemy_shooter* enemy);
void determine_current_state_enemy_shooter(Enemy_shooter* enemy);
void check_for_damage_enemy_shooter(Enemy_shooter* enemy);
void draw_enemy_shooter(Enemy_shooter* enemy);
direction_movement determine_direction_movement_enemy_shooter(Enemy_shooter* enemy);
void determine_float_direction_enemy_shooter(Enemy_shooter* enemy);

#endif // ENEMY_SHOOTER