#include "../include/enemy_skeleton.h"

void update_enemy_skeleton(Enemy_skeleton* enemy) {
  gravity_enemy(&enemy->base);
  move_enemy_skeleton(enemy);
  collision_with_blocks_enemy(&enemy->base);
  determine_current_state_enemy_skeleton(enemy);
  skeleton_attack_on_the_hero(enemy);
  determine_current_texture_enemy_skeleton(enemy);
  set_current_sprite_enemy(&enemy->base); 
  synchronize_hitbox_with_coordinates(&enemy->base.hitbox, enemy->base.coordinates);
  death_enemy_skeleton(enemy);
}

void move_enemy_skeleton(Enemy_skeleton* enemy) {
  if (enemy->current_state != ENEMY_SKELETON_WALK)
    return;
  if (enemy->base.direction == DIRECTION_LEFT)
    enemy->base.coordinates.x -= speed_dt(enemy->base.speed);
  else
    enemy->base.coordinates.x += speed_dt(enemy->base.speed);
}

void skeleton_attack_on_the_hero(Enemy_skeleton* enemy) {
  if (enemy->current_state != ENEMY_SKELETON_ATTACK)
    return;
  enemy->sprites_timer += dt;
  if (enemy->sprites_timer >= time_for_one_texture_iteration(enemy->base.texture.current)) {
    enemy->sprites_timer = 0;
    enemy->current_state = ENEMY_SKELETON_IDLE;
  }
  if (hero->state == HERO_DEATH || !collision_enemy_skeleton_with_hero(enemy))
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

int collision_enemy_skeleton_with_hero(Enemy_skeleton* enemy) {
  if (enemy->current_state  == ENEMY_SKELETON_DEATH)
    return 0;
  return collision_enemy_with_hero(&enemy->base);
}

void set_current_sprite_enemy_skeleton(Enemy_skeleton* enemy) {
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

void determine_current_texture_enemy_skeleton(Enemy_skeleton* enemy) {
  if (enemy->prev_state == enemy->current_state) {
    return;
  } else {
    enemy->base.texture.current = &enemy_container->textures[enemy->base.type][enemy->current_state]; 
    enemy->base.texture.current_number_sprite = 0;
  }
}

void death_enemy_skeleton(Enemy_skeleton* enemy) {
  if (enemy->base.health <= 0)
    enemy->current_state = ENEMY_SKELETON_DEATH;
  static double time = 0;
  if (enemy->current_state == ENEMY_SKELETON_DEATH) {
    time += dt;
    if (time >= time_for_one_texture_iteration(enemy->base.texture.current)) {
      enemy->base.type = ENEMY_INACTIVE;
    }
  }
}

void determine_current_state_enemy_skeleton(Enemy_skeleton* enemy) {
  check_for_damage_enemy_skeleton(enemy);
  if (enemy->current_state == ENEMY_SKELETON_DEATH || enemy->current_state == ENEMY_SKELETON_HURT)
    return;
  float distance = the_distance_between_the_centers_of_two_rect(&hero->hitbox, &enemy->base.hitbox);
  if (distance > enemy->reaction_range_walk) {
    enemy->current_state = ENEMY_SKELETON_IDLE;
  } else if (distance > get_distance_reaction_attack_skeleton(enemy)) {
    if (enemy->current_state == ENEMY_SKELETON_ATTACK)
      return;
    enemy->current_state = ENEMY_SKELETON_WALK;
    enemy->base.direction = determine_direction_movement_enemy_skeleton(enemy);
  } else {
    enemy->current_state = ENEMY_SKELETON_ATTACK;
    enemy->base.direction = determine_direction_movement_enemy_skeleton(enemy);
  }
}

void check_for_damage_enemy_skeleton(Enemy_skeleton* enemy) {
  if (enemy->prev_health > enemy->base.health) {
    enemy->current_state = ENEMY_SKELETON_HURT;
    enemy->prev_health = enemy->base.health;
  } else if (enemy->current_state == ENEMY_SKELETON_HURT) {
    enemy->sprites_timer += dt;
    if (enemy->sprites_timer >= time_for_one_texture_iteration(enemy->base.texture.current)) {
      enemy->sprites_timer = 0;
      enemy->current_state = ENEMY_SKELETON_IDLE;
    }
  }
}

float get_distance_reaction_attack_skeleton(Enemy_skeleton* enemy) {
  return enemy->reaction_range_walk * 0.4;
}

direction_movement determine_direction_movement_enemy_skeleton(Enemy_skeleton* enemy) {
  if (hero->state == HERO_IDLE)
    return enemy->base.direction;
  if (hero->coordinates.x + hero->hitbox.w / 2 < enemy->base.coordinates.x + enemy->base.hitbox.w / 2)
    return DIRECTION_LEFT;
  return DIRECTION_RIGHT;
}

void draw_enemy_skeleton(Enemy_skeleton* enemy) {
  if (enemy->base.direction == DIRECTION_LEFT)
    render_copy_enemy(&enemy->base, SDL_FLIP_HORIZONTAL);
  else
    render_copy_enemy(&enemy->base);
}