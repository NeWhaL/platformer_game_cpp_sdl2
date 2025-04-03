#include "../include/enemy_shooter.h"

void update_enemy_shooter(Enemy_shooter* enemy) {
  move_enemy_shooter(enemy);
  collision_with_blocks_enemy(&enemy->base);
  determine_current_state_enemy_shooter(enemy);
  shooter_attack_on_the_hero(enemy);
  determine_current_texture_enemy_shooter(enemy);
  set_current_sprite_enemy_shooter(enemy); 
  synchronize_hitbox_with_coordinates(&enemy->base.hitbox, enemy->base.coordinates);
  death_enemy_shooter(enemy);
}

void move_enemy_shooter(Enemy_shooter* enemy) {
  if (enemy->current_state != ENEMY_SHOOTER_WALK)
    return;
  determine_float_direction_enemy_shooter(enemy);
  enemy->base.coordinates.x += speed_dt(enemy->base.speed) * enemy->float_direction.x;
  enemy->base.coordinates.y += speed_dt(enemy->base.speed) * enemy->float_direction.y;
}

void determine_float_direction_enemy_shooter(Enemy_shooter* enemy) {
  enemy->float_direction = {
    hero->coordinates.x - enemy->base.coordinates.x,
    hero->coordinates.y - enemy->base.coordinates.y
  };
  float distance = the_distance_between_the_centers_of_two_rect(&hero->hitbox, &enemy->base.hitbox);
  enemy->float_direction.x /= distance;
  enemy->float_direction.y /= distance;
}

void shooter_attack_on_the_hero(Enemy_shooter* enemy) {
  if (enemy->current_state != ENEMY_SHOOTER_SHOT_ATTACK)
    return;
  enemy->sprites_timer += dt;
  if (enemy->sprites_timer >= time_for_one_texture_iteration(enemy->base.texture.current)) {
    enemy->sprites_timer = 0;
    enemy->base.texture.sprite_time_counter = 0;
    enemy->current_state = ENEMY_SHOOTER_IDLE;
    enemy->was_the_shot_fired = 0;
  } else if (!enemy->was_the_shot_fired && 
              enemy->base.texture.current_number_sprite == enemy->sprite_number_for_attack) {
    enemy->was_the_shot_fired = 1;
    add_shot_in_shots_container(
      enemy->base.coordinates,
      SHOT_CREATOR_ENEMY,
      enemy->attack.shot.type,
      enemy->base.direction,
      enemy->attack.shot.range,
      enemy->base.damage,
      enemy->attack.shot.speed
    );
  }
  if ((hero->state == HERO_DEATH || !collision_enemy_shooter_with_hero(enemy)) ||
       hero->state == HERO_ATTACK && is_the_dealing_damage_now_hero()) {
    return;
  }
  if (hero->damage_timer >= hero->max_damage_timer) {
    hero->health -= enemy->base.damage;
    if (hero->health > 0)
      hero->state = HERO_HURT;
    else
      hero->state = HERO_DEATH;
  }
}

int collision_enemy_shooter_with_hero(Enemy_shooter* enemy) {
  if (enemy->current_state  == ENEMY_SHOOTER_DEATH)
    return 0;
  return collision_enemy_with_hero(&enemy->base);
}

void set_current_sprite_enemy_shooter(Enemy_shooter* enemy) {
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

void determine_current_texture_enemy_shooter(Enemy_shooter* enemy) {
  if (enemy->prev_state == enemy->current_state) {
    return;
  } else {
    enemy->base.texture.current = &enemy_container->textures[enemy->base.type][enemy->current_state]; 
    enemy->base.texture.current_number_sprite = 0;
  }
}

void death_enemy_shooter(Enemy_shooter* enemy) {
  if (enemy->base.health <= 0)
    enemy->current_state = ENEMY_SHOOTER_DEATH;
  if (enemy->current_state == ENEMY_SHOOTER_DEATH) {
    enemy->base.death_time += dt;
    if (enemy->base.death_time >= time_for_one_texture_iteration(enemy->base.texture.current)) {
      enemy->base.type = ENEMY_INACTIVE;
    }
  }
}

void determine_current_state_enemy_shooter(Enemy_shooter* enemy) {
  check_for_damage_enemy_shooter(enemy); 
  if (enemy->current_state == ENEMY_SHOOTER_DEATH || enemy->current_state == ENEMY_SHOOTER_HURT)
    return;
  float distance = the_distance_between_the_centers_of_two_rect(&hero->hitbox, &enemy->base.hitbox);
  if (distance > enemy->responce_radius) {
    enemy->current_state = ENEMY_SHOOTER_IDLE;
  } else if (distance > enemy->attack_radius) {
    if (enemy->current_state == ENEMY_SHOOTER_SHOT_ATTACK)
      return;
    enemy->current_state = ENEMY_SHOOTER_WALK;
    enemy->base.direction = determine_direction_movement_enemy_shooter(enemy);
  } else {
    enemy->current_state = ENEMY_SHOOTER_SHOT_ATTACK;
    enemy->base.direction = determine_direction_movement_enemy_shooter(enemy);
  }
}

direction_movement determine_direction_movement_enemy_shooter(Enemy_shooter* enemy) {
  if (hero->state == HERO_IDLE)
    return enemy->base.direction;
  if (hero->coordinates.x + hero->hitbox.w / 2 < enemy->base.coordinates.x + enemy->base.hitbox.w / 2)
    return DIRECTION_LEFT;
  return DIRECTION_RIGHT;
}

void draw_enemy_shooter(Enemy_shooter* enemy) {
  if (enemy->base.direction == DIRECTION_LEFT)
    render_copy_enemy(&enemy->base, SDL_FLIP_HORIZONTAL);
  else
    render_copy_enemy(&enemy->base);
}

void check_for_damage_enemy_shooter(Enemy_shooter* enemy) {
  if (enemy->prev_health > enemy->base.health) {
    enemy->current_state = ENEMY_SHOOTER_HURT;
    enemy->prev_health = enemy->base.health;
  } else if (enemy->current_state == ENEMY_SHOOTER_HURT) {
    enemy->sprites_timer += dt;
    if (enemy->sprites_timer >= time_for_one_texture_iteration(enemy->base.texture.current)) {
      enemy->sprites_timer = 0;
      enemy->current_state = ENEMY_SHOOTER_IDLE;
    }
  }
}