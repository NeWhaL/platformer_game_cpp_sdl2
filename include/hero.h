#ifndef HERO_H
#define HERO_H

#include "sdl.h"
#include "global_var.h"
#include "texture.h"
#include "level.h"
#include <math.h>
#include "direction.h"


enum Attack_type {
  ATTACK_HERO_BASE_1,
  ATTACK_HERO_BASE_2,
  ATTACK_HERO_BASE_3,
  ATTACK_HERO_NONE,
};

struct Attack_type_info {
  float damage_multiplier; //Множитель урона
  int number_sprite_for_damage; //Номер спрайта, с которого начинается нанесение урона.
};

//Для каждой атаки свой множитель и свой стартовый спрайт нанесения урона
extern const int amount_attack_hero;
extern Attack_type_info attack_info_hero[];

extern struct Hero {
  SDL_Rect hitbox;
  SDL_FPoint coordinates;
  struct {
    Texture idle;
    Texture attack_1;
    Texture attack_2;
    Texture attack_3;
  } textures;
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
  struct {
    float pure_damage;
    int cause_damage;
    Attack_type type;
    SDL_Rect hitbox;
 } attack;
} *hero;

void init_hero();
void load_hero(const char* load_file);
void de_init_hero();
void draw_hero();
void update_hero();
void move_hero();
void gravity_hero();
void hitbox_change_due_new_sprite_hero(int number_sprite, 
  float* height_difference = NULL, float* width_difference = NULL);
void set_current_texture_hero(Texture* texture);
void set_current_sprite_hero(double time_one_frame);
void collision_with_blocks_hero();
void collision_platforms_with_hero();
int collision_platform_with_hero(struct Platform* platform);
int current_coefficient_jerk_hero();
SDL_FPoint get_coordinates_for_new_game_hero();
void attack_hero();
float get_damage_hero(Attack_type type);
void attack_logic_hero();
void collision_attack_hero_with_enemy();

#endif