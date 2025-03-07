#include "../include/enemy_slime.h"

void update_enemy_slime(Enemy_slime* enemy) {
  death_enemy_slime(enemy);
  gravity_enemy(&enemy->base);
  move_enemy_slime(enemy);
  collision_with_blocks_enemy(&enemy->base);
  slime_attack_on_the_hero(enemy);
  determine_current_texture_enemy_slime(enemy);
  set_current_sprite_enemy(&enemy->base);
  synchronize_hitbox_with_coordinates(&enemy->base.hitbox, enemy->base.coordinates);
}

void move_enemy_slime(Enemy_slime* enemy) {
  if (enemy->current_state == ENEMY_SLIME_DEATH)
    return;
  if (enemy->base.direction == DIRECTION_LEFT)
    enemy->base.coordinates.x -= speed_dt(enemy->base.speed);
  else
    enemy->base.coordinates.x += speed_dt(enemy->base.speed);
}

void slime_attack_on_the_hero(Enemy_slime* enemy) {
  if (hero->state == HERO_DEATH || !collision_enemy_slime_with_hero(enemy))
    return;
  if (hero->state == HERO_ATTACK && is_the_dealing_damage_now_hero())
    return;
  if (hero->damage_timer >= hero->max_damage_timer) {
    hero->health -= enemy->base.damage;
    if (hero->health > 0)
      hero->state = HERO_HURT;
    else
      hero->state = HERO_DEATH;
  }
}

//Возвращает булево значение (была ли коллизия с героем или нет)
int collision_enemy_slime_with_hero(Enemy_slime* enemy) {
  int is_collision = 0;
  if (enemy->current_state  == ENEMY_SLIME_DEATH)
    return is_collision;
  switch (collision_of_two_objects(&hero->hitbox, &enemy->base.hitbox)) {
    case COLLISION_UP: {
      hero->coordinates.y -= speed_dt(hero->current_speed_gravity);
      is_collision = 1;
    } break;
    case COLLISION_DOWN: {
      hero->coordinates.y += speed_dt(hero->current_speed_gravity);
      is_collision = 1;
    } break;
    case COLLISION_RIGHT: {
      if (enemy->base.direction == DIRECTION_LEFT) {
        if (hero->speed * current_coefficient_jerk_hero() > enemy->base.speed)
          enemy->base.coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero() + enemy->base.speed); 
        else
          enemy->base.coordinates.x -= speed_dt(enemy->base.speed); 
      } else {
        if (hero->speed * current_coefficient_jerk_hero() > enemy->base.speed)
          enemy->base.coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero() + enemy->base.speed); 
        else
          enemy->base.coordinates.x -= speed_dt(enemy->base.speed); 
      }
      is_collision = 1;
    } break;
    case COLLISION_LEFT: {
      if (enemy->base.direction == DIRECTION_LEFT) {
        if (hero->speed * current_coefficient_jerk_hero() > enemy->base.speed)
          enemy->base.coordinates.x += speed_dt(hero->speed * current_coefficient_jerk_hero() + enemy->base.speed); 
        else
          enemy->base.coordinates.x += speed_dt(enemy->base.speed); 
      } else {
        if (hero->speed * current_coefficient_jerk_hero() > enemy->base.speed)
          enemy->base.coordinates.x -= speed_dt(hero->speed * current_coefficient_jerk_hero() + enemy->base.speed); 
        else
          enemy->base.coordinates.x -= speed_dt(enemy->base.speed); 
      }
      is_collision = 1;
    } break;
  }
  return is_collision; 
}

void set_current_sprite_enemy_slime(Enemy_slime* enemy) {
  set_current_sprite(
    enemy->base.texture.current,
    &enemy->base.texture.current_number_sprite,
    &enemy->base.hitbox,
    &enemy->base.coordinates,
    enemy->base.direction,
    &enemy->base.texture.sprite_time_counter,
    enemy->current_state != enemy->prev_state
  );
  if (enemy->current_state != enemy->prev_state)
    enemy->prev_state = enemy->current_state;
}

void determine_current_texture_enemy_slime(Enemy_slime* enemy) {
  if (enemy->prev_state == enemy->current_state) {
    return;
  } else {
    enemy->base.texture.current = &enemy_container->textures[enemy->base.type][enemy->current_state]; 
    enemy->base.texture.current_number_sprite = 0;
  }
}

void death_enemy_slime(Enemy_slime* enemy) {
  static double time = 0;
  if (enemy->current_state == ENEMY_SLIME_DEATH) {
    time += dt;
    if (time >= time_for_one_texture_iteration(enemy->base.texture.current)) {
      enemy->base.type = ENEMY_INACTIVE;
    }
  }
}