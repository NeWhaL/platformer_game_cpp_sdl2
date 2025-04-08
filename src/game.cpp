#include "../include/game.h"

int is_init_game = 1;

void init_game(const char* save_file) {
  if (!is_init_game)
    return;
  if (!save_file) { 
    init_level(LEVEL_1);
    init_hero();
    init_enemies(level->number);
    init_shot_container();
    is_init_game = 0;
  } else {
    load_level(save_file);
    load_hero(save_file);
    // is_init_game = 0;
  }
  is_init_game = 0;
}

void de_init_game() {
  de_init_enemies();
  de_init_hero();
  de_init_level();
  de_init_shot_container();
}

void game(const char* save_file) {
  init_game(save_file);
  while (is_running == GAME) {
    updating_game_events();
    updating_game_logic();
    draw_game_frame();
    next_level(); 
  }
}

int collision_with_next_level_block_hero() {
  SDL_Rect position_block = { 0, 0, level->real_size_edge_block, level->real_size_edge_block };
  for (int i = 0; i < level->amount_blocks.y; ++i) {
    for (int j = 0; j < level->amount_blocks.x; ++j) {
      position_block.x = j * level->real_size_edge_block;
      position_block.y = i * level->real_size_edge_block;
      if (level->map[i][j] == BLOCK_NEXT_LEVEL && collision_of_two_objects(&hero->hitbox, &position_block)) {
        return 1;
      }
    }
  }
  return 0;
}

void next_level() {
  if (!collision_with_next_level_block_hero())
    return;
  switch (level->number) {
    case LEVEL_1: {
      // загрузить новую карту в level->map; - DONE
      // На остове этой карты сделать: - DONE
        // init platforms; - DONE 
        // установить новый фон - DONE
        // установить новые текстуры блоков - DONE
      // de init enemies - DONE
      // init enemies (при создании противников, данные считывать с нового файла, для их усилени); - DONE
      // Переместить координаты гг в точку BLOCK_SPAWN_HERO
      init_level(LEVEL_2);
      init_enemies(level->number);
      init_shot_container();
      hero->coordinates = get_coordinates_for_new_game_hero();
    } break;
    case LEVEL_2: {
      init_level(LEVEL_3);
      init_enemies(level->number);
      init_shot_container();
      hero->coordinates = get_coordinates_for_new_game_hero();
    } break;
    case LEVEL_3: {
      // конец игры
    } break;
  }
}

void updating_game_events() {
	while (SDL_PollEvent(&events)) {
		switch (events.type) {
      case SDL_QUIT: {
        is_running = NO_IS_RUNNING;
      } break;
		case SDL_KEYDOWN:
			switch (events.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE: {
          is_running = STOP_GAME_MENU;
        } break;
			}
		}
	}
}

void updating_game_logic() {
  updating_dt();
  updating_level();
  updating_enemies();
  update_shots();
  update_hero();
}

void draw_game_frame() {
  draw_level();
  draw_enemies();
  draw_hero();
  draw_shots();
  SDL_RenderPresent(renderer);
}