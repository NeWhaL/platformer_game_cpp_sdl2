#include "../include/enemy_slime.h"

void update_enemy_slime(Enemy_slime* enemy) {
  gravity_enemy(&enemy->base);
  move_enemy_slime(enemy);
  collision_with_blocks_enemy(&enemy->base);
  slime_attack_on_the_hero(enemy);
  determine_current_state_enemy_slime(enemy);
  determine_current_texture_enemy_slime(enemy);
  set_current_sprite_enemy_slime(enemy);
  synchronize_hitbox_with_coordinates(&enemy->base.hitbox, enemy->base.coordinates);
  death_enemy_slime(enemy);
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

void determine_current_state_enemy_slime(Enemy_slime* enemy) {
  if (enemy->current_state == ENEMY_SLIME_DEATH)
    return;
  if (hero->coordinates.y + hero->hitbox.h >= enemy->base.coordinates.y &&
      hero->coordinates.y <= enemy->base.coordinates.y + enemy->base.hitbox.h) {
    enemy->current_state = ENEMY_SLIME_ATTACK;
    if (enemy->base.coordinates.x > hero->coordinates.x + hero->hitbox.w) {
      enemy->base.direction = DIRECTION_LEFT;
    } else if (enemy->base.coordinates.x + enemy->base.hitbox.w < hero->coordinates.x) {
      enemy->base.direction = DIRECTION_RIGHT;
    }
  } else {
    enemy->current_state = ENEMY_SLIME_WALK;
  }
}

//Возвращает булево значение (была ли коллизия с героем или нет)
int collision_enemy_slime_with_hero(Enemy_slime* enemy) {
  if (enemy->current_state  == ENEMY_SLIME_DEATH)
    return 0;
  return collision_enemy_with_hero(&enemy->base);
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
  if (enemy->base.health <= 0)
    enemy->current_state = ENEMY_SLIME_DEATH;
  if (enemy->current_state == ENEMY_SLIME_DEATH) {
    enemy->base.death_time += dt;
    if (enemy->base.death_time >= time_for_one_texture_iteration(enemy->base.texture.current)) {
      enemy->base.type = ENEMY_INACTIVE;
    }
  }
}

void draw_enemy_slime(Enemy_slime* enemy) {
  if (enemy->base.direction == DIRECTION_LEFT)
    render_copy_enemy(&enemy->base);
  else
    render_copy_enemy(&enemy->base, SDL_FLIP_HORIZONTAL);
}