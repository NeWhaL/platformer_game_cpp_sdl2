#include "../include/load_save.h"
#include <stdio.h>

const char* save_load_path[SAVE_AMOUNT][SAVE_TYPE_AMOUNT] = {
  {
    "../game_data/load_and_save/save_1/hero_save.bin",
    "../game_data/load_and_save/save_1/enemies_save.bin",
    "../game_data/load_and_save/save_1/shot_save.bin",
    "../game_data/load_and_save/save_1/level_save.bin"
  },
  {
    "../game_data/load_and_save/save_2/hero_save.bin",
    "../game_data/load_and_save/save_2/enemies_save.bin",
    "../game_data/load_and_save/save_2/shot_save.bin",
    "../game_data/load_and_save/save_2/level_save.bin"
  },
  {
    "../game_data/load_and_save/save_3/hero_save.bin",
    "../game_data/load_and_save/save_3/enemies_save.bin",
    "../game_data/load_and_save/save_3/shot_save.bin",
    "../game_data/load_and_save/save_3/level_save.bin"
  }
};

// SAVE
void save_progress(Save_number number) {
  save_hero_in_file(save_load_path[number][SAVE_TYPE_HERO]);
  save_shots_in_file(save_load_path[number][SAVE_TYPE_SHOTS]);
  save_level_in_file(save_load_path[number][SAVE_TYPE_LEVEL]);
  save_enemies_in_file(save_load_path[number][SAVE_TYPE_ENEMIES]);
}

void save_hero_in_file(const char* filename) {
  FILE* save_file;
  if (save_file = fopen(filename, "wb")) {
    fwrite(hero, sizeof(Hero), 1, save_file);
    fclose(save_file);
  } else {
    printf("Не удалось сохранить главного героя в файл: %s\n", filename);
  }
}

void save_enemies_in_file(const char* filename) {
  FILE* save_file;
  if (save_file = fopen(filename, "wb")) {
    int current_amount_enemies = enemy_container->amount_enemies; 
    for (int i = 0; i < enemy_container->amount_enemies; ++i) {
      if (enemy_container->enemies[i]->type == ENEMY_INACTIVE)
        --current_amount_enemies;
    }
    fwrite(&current_amount_enemies, sizeof(int), 1, save_file);
    for (int i = 0; i < enemy_container->amount_enemies; ++i) { 
      Enemy_base* enemy = enemy_container->enemies[i];
      if (enemy->type == ENEMY_INACTIVE)
        continue;
      fwrite(&enemy->type, sizeof(Enemy_type), 1, save_file);
      switch (enemy->type) {
        case ENEMY_SLIME: {
          Enemy_slime* slime = (Enemy_slime*)enemy->full_enemy;
          fwrite(slime, sizeof(Enemy_slime), 1, save_file);
        } break;
        case ENEMY_SKELETON: {
          Enemy_skeleton* skeleton = (Enemy_skeleton*)enemy->full_enemy;
          fwrite(skeleton, sizeof(Enemy_skeleton), 1, save_file);
        } break;
        case ENEMY_SHOOTER: {
          Enemy_shooter* shooter = (Enemy_shooter*)enemy->full_enemy;
          fwrite(shooter, sizeof(Enemy_shooter), 1, save_file);
        } break;
      }
    }
    fclose(save_file);
  } else {
    printf("Не удалось сохранить противников в файл: %s\n", filename);
  }
}

void save_shots_in_file(const char* filename) {
  //выстрелы не сохраняются
}

void save_level_in_file(const char* filename) {
  FILE* save_file;
  if (save_file = fopen(filename, "wb")) {
    fwrite(level, sizeof(Level), 1, save_file);
    fwrite(&amount_platforms, sizeof(int), 1, save_file);
    for (int i = 0; i < amount_platforms; ++i) {
      fwrite(&(platforms[i]), sizeof(Platform), 1, save_file);
    }
    fclose(save_file);
  } else {
    printf("Не удалось сохранить уровень в файл: %s\n", filename);
  }
}

// LOAD
void load_progress(Save_number number) {
  load_hero_from_file(save_load_path[number][SAVE_TYPE_HERO]);
  load_shots_from_file(save_load_path[number][SAVE_TYPE_SHOTS]);
  load_level_from_file(save_load_path[number][SAVE_TYPE_LEVEL]);
  load_enemies_from_file(save_load_path[number][SAVE_TYPE_ENEMIES]);
}

void load_hero_from_file(const char* filename) {
  FILE* load_file;
  if (load_file = fopen(filename, "rb")) {
    if (!(hero = (Hero*)malloc(sizeof(Hero)))) {
      printf("Не удалось получить ресурсы для Hero *hero...\n");
      de_init_application(1);
    } 
    fread(hero, sizeof(Hero), 1, load_file);
    hero->textures.all = NULL;
    init_textures_hero();
    set_current_texture_hero(hero->textures.all[hero->state]);
    fclose(load_file);
  } else {
    printf("Не удалось загрузить главного героя из файла: %s\n", filename);
  }
}

void load_enemies_from_file(const char* filename) {
  FILE* load_file;
  if (load_file = fopen(filename, "rb")) { 
    enemy_container = (Enemy_container*)malloc(sizeof(Enemy_container));
    fread(&enemy_container->amount_enemies, sizeof(int), 1, load_file);
    enemy_container->enemies = (Enemy_base**)malloc(sizeof(Enemy_base*) * enemy_container->amount_enemies);
    init_textures_enemies();
    for (int i = 0; i < enemy_container->amount_enemies; ++i) {
      Enemy_type type;
      fread(&type, sizeof(Enemy_type), 1, load_file);
      switch (type) {
        case ENEMY_SLIME: {
          Enemy_slime* slime = (Enemy_slime*)malloc(sizeof(Enemy_slime));
          fread(slime, sizeof(Enemy_slime), 1, load_file);
          slime->base.texture.current = &enemy_container->textures[ENEMY_SLIME][slime->current_state];
          slime->base.full_enemy = slime;
          enemy_container->enemies[i] = &(slime->base);
        } break;
        case ENEMY_SKELETON: {
          Enemy_skeleton* skeleton = (Enemy_skeleton*)malloc(sizeof(Enemy_skeleton));
          fread(skeleton, sizeof(Enemy_skeleton), 1, load_file);
          skeleton->base.texture.current = &enemy_container->textures[ENEMY_SKELETON][skeleton->current_state];
          skeleton->base.full_enemy = skeleton;
          enemy_container->enemies[i] = &(skeleton->base);
        } break;
        case ENEMY_SHOOTER: {
          Enemy_shooter* shooter = (Enemy_shooter*)malloc(sizeof(Enemy_shooter));
          fread(shooter, sizeof(Enemy_shooter), 1, load_file);
          shooter->base.texture.current = &enemy_container->textures[ENEMY_SHOOTER][shooter->current_state];
          shooter->base.full_enemy = shooter;
          enemy_container->enemies[i] = &(shooter->base);
        } break;
      }
    }
    fclose(load_file);
  } else {
    printf("Не удалось загрузить противников из файла: %s\n", filename);
  }
}

void load_shots_from_file(const char* filename) {
  init_shot_container();
}

void load_level_from_file(const char* filename) {
  FILE* load_file;
  if (load_file = fopen(filename, "rb")) {
    level = malloc_level(); 
    char** general_map = level->map;
    fread(level, sizeof(Level), 1, load_file);
    level->map = general_map;
    FILE* map = fopen(filename_maps[level->number], "r");
    if (map)
      for (int i = 0; fgets(level->map[i], level->amount_blocks.x, map) != NULL; ++i)
        fgetc(map);
    fclose(map);
    level->blocks.sprite = load_texture_from_file(filename_blocks[level->number], &level->blocks.size);
    SDL_SetTextureBlendMode(level->blocks.sprite, SDL_BLENDMODE_BLEND);
    fread(&amount_platforms, sizeof(int), 1, load_file);
    platforms = (Platform*)malloc(sizeof(Platform) * amount_platforms);
    for (int i = 0; i < amount_platforms; ++i) {
      fread(&(platforms[i]), sizeof(Platform), 1, load_file);
    }
    fclose(load_file);
  } else {
    printf("Не удалось сохранить уровень в файл: %s\n", filename);
  }
}