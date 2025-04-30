#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

#include "hero.h"
#include "enemy_shooter.h"
#include "enemy_skeleton.h"
#include "enemy_slime.h"
#include "shot.h"
#include "level.h"

enum Save_number {
  CONTINUE_GAME = -2,
  NEW_GAME = -1,
  SAVE_1,
  SAVE_2,
  SAVE_3,
  SAVE_AMOUNT
};

enum Save_type {
  SAVE_TYPE_HERO,
  SAVE_TYPE_ENEMIES,
  SAVE_TYPE_SHOTS,
  SAVE_TYPE_LEVEL,
  SAVE_TYPE_AMOUNT
};

extern const char* save_load_path[SAVE_AMOUNT][SAVE_TYPE_AMOUNT];

void save_progress(Save_number number);
void save_hero_in_file(const char* filename);
void save_enemies_in_file(const char* filename);
void save_shots_in_file(const char* filename);
void save_level_in_file(const char* filename);

void load_progress(Save_number number);
void load_hero_from_file(const char* filename);
void load_enemies_from_file(const char* filename);
void load_shots_from_file(const char* filename);
void load_level_from_file(const char* filename);

#endif