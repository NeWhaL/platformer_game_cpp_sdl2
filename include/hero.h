#ifndef HERO_H
#define HERO_H

#include "sdl.h"
#include "global_var.h"
#include "application.h"
#include "texture.h"
#include "level.h"
#include <math.h>
#include "direction.h"


enum Attack_type {
  ATTACK_HERO_NONE,
  ATTACK_HERO_BASE_1,
  ATTACK_HERO_BASE_2,
};

extern struct Hero {
  SDL_Rect hitbox;
  SDL_FPoint coordinates;
  // struct {
  //   Texture idle;
  //   Texture attack_1;
  //   Texture attack_2;
  //   Texture attack_3;
  // } textures;
  Texture* current_texture;
  int current_number_sprite;
  direction_movement direction;
  float speed;
  float coefficient_jerk;
  int running;
  float health;
  int is_standing;
  float current_speed_gravity;
  float jump_height;
  int is_jumping;
  Attack_type is_attack;
} *hero;

void init_hero();
void load_hero(const char* load_file);
void de_init_hero();
void draw_hero();
void update_hero();
void move_hero();
void gravity_hero();
void set_current_sprite_hero(double time_one_frame);
void collision_with_blocks_hero();
void collision_platforms_with_hero();
int collision_platform_with_hero(struct Platform* platform);
int current_coefficient_jerk_hero();
SDL_FPoint get_coordinates_for_new_game_hero();
void select_attack_hero();

#endif