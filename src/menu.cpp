#include "../include/menu.h"

Button* buttons_main_menu;
Button* buttons_load_menu;
Button* buttons_stop_menu;
Button* buttons_save_menu;
Button* buttons_change_skin_menu;
Button* buttons_upgrade_menu;
SDL_Color standart_background_color =  {180, 180, 240, 255};
SDL_Color current_color_button = {140, 140, 240, 255};
SDL_Color standart_color_button = {150, 150, 220, 255};
SDL_Color standart_color_text = {180, 180, 240, 255};
const int button_width = window_width / 3;
const int button_height = 120;
const int left_margin = window_width / 3;
const int up_margin = 80;
const int amount_buttons_in_main_menu = 3;
const int amount_buttons_in_load_menu = 4;
const int amount_buttons_in_stop_menu = 5;
const int amount_buttons_in_save_menu = 4;
const int amount_buttons_in_change_skin_menu = 3;
const int amount_buttons_in_upgrade_menu = 7;
const int cost_of_improvement = 2;

void init_main_menu() {
  create_main_menu_buttons();
}

void init_game_menu() {
  create_game_menu_buttons();
}


void create_button(Button* btn, button_type type, const char* text, int x, int y, int w, int h, int font_size,
                   SDL_Color std_color, SDL_Color cur_color) {
  btn->type = type;
  btn->standart_color_button = standart_color_button;
  btn->active_color_button = cur_color;
  btn->color_button = btn->standart_color_button;
  btn->size_button = { x, y, w, h };
  btn->font = create_font(font_path, text, standart_color_text, &btn->size_text, btn->size_button, font_size); 
}

int button_collision(Button* btn) {
  SDL_Rect btn_size = btn->size_button;
  int collision = 0;
  if (collision = button_collision_rect(btn_size)) {
    btn->color_button = btn->active_color_button;
  } else
    btn->color_button = btn->standart_color_button;
  return collision;
}

int button_collision_rect(SDL_Rect btn_rect) {
  return mouse_position.x >= btn_rect.x && 
         mouse_position.x <= btn_rect.x + btn_rect.w && 
         mouse_position.y >= btn_rect.y && 
         mouse_position.y <= btn_rect.y + btn_rect.h;
}

void draw_buttons(Button* buttons, int amount_button) {
  for (int i = 0; i < amount_button; ++i)
    draw_button(&buttons[i]);
}

void draw_button(Button* btn) {
  SDL_SetRenderDrawColor(renderer, btn->color_button.r, btn->color_button.g, btn->color_button.b, btn->color_button.a);
  SDL_RenderFillRect(renderer, &btn->size_button);
  SDL_RenderCopy(renderer, btn->font, NULL, &btn->size_text);
}

void draw_menu(void draw_buttons_function(), SDL_Color background_color) {
  SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a);
  SDL_RenderClear(renderer);
  // draw_background(&backgrounds[MAIN_MENU]);
  draw_buttons_function();
  SDL_RenderPresent(renderer);
}

void buttons_malloc(Button** buttons, int size) {
  *buttons = (Button*)malloc(sizeof(Button) * size);
}

void create_buttons(Button* buttons, button_type* types, const char** names, const int amount_buttons,
                    int up_margin_between_buttons, int left_margin, int button_width, int button_height,
                    int font_size) {
  int i_up_margin = up_margin_between_buttons;
  for (int i = 0; i < amount_buttons; ++i) {
    create_button(
      &buttons[i],
      types[i],
      names[i],
      left_margin,
      i_up_margin,
      button_width,
      button_height,
      font_size 
    );
    i_up_margin = buttons[i].size_button.y + buttons[i].size_button.h + up_margin_between_buttons;
  }
}

void main_menu() {
  if (!is_init_game) {
    de_init_game();
    is_init_game = 1;
  }
  int is_mouse_button_left = 0;
  while (is_running == MAIN_MENU || is_running == LOAD_MENU) {
    updating_dt();
    updating_mouse_state();
    updating_main_menu_events(&is_mouse_button_left);  
    logic_main_menu(is_mouse_button_left);
    draw_menu(draw_main_menu_buttons);
    is_mouse_button_left = 0;
  }
}

void create_main_menu_buttons() {
  buttons_malloc(&buttons_main_menu, amount_buttons_in_main_menu);
  const char* button_names_main_menu[amount_buttons_in_main_menu] = { "new game", "load", "exit" };
  button_type button_types_main_menu[amount_buttons_in_main_menu] = { BUTTON_START, BUTTON_LOAD_MENU, BUTTON_EXIT };
  create_buttons(buttons_main_menu, button_types_main_menu, button_names_main_menu, amount_buttons_in_main_menu,
                 up_margin, left_margin);

  buttons_malloc(&buttons_load_menu, amount_buttons_in_load_menu);
  const char* button_names_load_menu[amount_buttons_in_load_menu] = { "load 1", "load 2", "load 3", "main menu"};
  button_type button_types_load_menu[amount_buttons_in_load_menu] = { BUTTON_LOAD, BUTTON_LOAD, BUTTON_LOAD, BUTTON_MAIN_MENU };
  create_buttons(buttons_load_menu, button_types_load_menu, button_names_load_menu, amount_buttons_in_load_menu,
                 up_margin, left_margin);
}


void updating_main_menu_events(int* is_mouse_button_left) {	
	while (SDL_PollEvent(&events)) {
		switch (events.type) {
      case SDL_QUIT: {
        is_running = NO_IS_RUNNING;
      } break;
      case SDL_KEYDOWN: {
        switch (events.key.keysym.scancode) {
          case SDL_SCANCODE_ESCAPE: {
            if (is_running == MAIN_MENU)
              is_running = NO_IS_RUNNING;
            else
              is_running = MAIN_MENU;
          } break;
        }
      } break;
      case SDL_MOUSEBUTTONUP: {
        if (events.button.button == SDL_BUTTON_LEFT)
          *is_mouse_button_left = 1;
      } break;
		} 
	}
}

void logic_main_menu(int is_mouse_button_left) {
  if (is_running == MAIN_MENU) {
    for (int i = 0; i < amount_buttons_in_main_menu; ++i)
      if (button_collision(&buttons_main_menu[i]))
        switch (buttons_main_menu[i].type) {
          case BUTTON_START: {
            if (is_mouse_button_left) {
              is_running = GAME;
              return;
            }
          } break;
          case BUTTON_LOAD_MENU: {
            if (is_mouse_button_left) {
              is_running = LOAD_MENU;
              return;
            }
          } break;
          case BUTTON_EXIT: {
            if (is_mouse_button_left) {
              is_running = NO_IS_RUNNING;
              return;
            }
          } break;
        }
    return;
  } 
  for (int i = 0; i < amount_buttons_in_load_menu; ++i)
    if (button_collision(&buttons_load_menu[i]))
      switch (buttons_load_menu[i].type) {
        case BUTTON_LOAD: {
          if (is_mouse_button_left) {
            //достать путь до файла с сохранением и отдать в init_game();
            return;
          }
        } break;
        case BUTTON_MAIN_MENU: {
          if (is_mouse_button_left)
            is_running = MAIN_MENU;
        } break;
      }
}

void draw_main_menu_buttons() {
  if (is_running == MAIN_MENU) {
    for (int i = 0; i < amount_buttons_in_main_menu; ++i)
      draw_button(&buttons_main_menu[i]);
    return;
  } 
  for (int i = 0; i < amount_buttons_in_load_menu; ++i)
    draw_button(&buttons_load_menu[i]);
}

void de_init_main_menu() {
  free(buttons_load_menu);
  free(buttons_main_menu);
}


void game_menu() {
  int is_mouse_button_left = 0;
  while (is_running == STOP_GAME_MENU || is_running == SAVE_MENU ||
         is_running == UPGRADE_MENU || is_running == CHANGE_SKIN_MENU) {
    updating_dt();
    updating_mouse_state();
    updating_game_menu_events(&is_mouse_button_left);
    logic_game_menu(is_mouse_button_left);
    draw_menu(draw_game_menu_buttons);
    is_mouse_button_left = 0;
  }
}

void create_game_menu_buttons() {
  // stop menu
  buttons_malloc(&buttons_stop_menu, amount_buttons_in_stop_menu);
  const char* button_names_stop_menu[amount_buttons_in_stop_menu] = { "continue", "save", "upgrade", "change skin", "main menu"};
  button_type button_types_stop_menu[amount_buttons_in_stop_menu] = { BUTTON_CONTINUE, BUTTON_SAVE_MENU, 
    BUTTON_UPGRADE, BUTTON_CHANGE_SKIN, BUTTON_MAIN_MENU };
  create_buttons(buttons_stop_menu, button_types_stop_menu, button_names_stop_menu, amount_buttons_in_stop_menu);
  // save menu
  buttons_malloc(&buttons_save_menu, amount_buttons_in_save_menu);
  const char* button_names_save_menu[amount_buttons_in_save_menu] = { "save 1", "save 2", "save 3", "back"};
  button_type button_types_save_menu[amount_buttons_in_save_menu] = { BUTTON_SAVE, BUTTON_SAVE, BUTTON_SAVE, BUTTON_STOP_GAME_MENU };
  create_buttons(buttons_save_menu, button_types_save_menu, button_names_save_menu, amount_buttons_in_save_menu);
  // change skin menu
  buttons_malloc(&buttons_change_skin_menu, amount_buttons_in_change_skin_menu);
  const char* button_names_change_skin_menu[amount_buttons_in_change_skin_menu] = { "red knight", "green knight", "back"};
  button_type button_types_change_skin_menu[amount_buttons_in_change_skin_menu] = { BUTTON_SKIN_RED_KNIGHT, BUTTON_SKIN_GREEN_KNIGHT, BUTTON_STOP_GAME_MENU };
  create_buttons(buttons_change_skin_menu, button_types_change_skin_menu, button_names_change_skin_menu, amount_buttons_in_change_skin_menu);
  // upgrade menu
  buttons_malloc(&buttons_upgrade_menu, amount_buttons_in_upgrade_menu);
  const char* button_names_upgrade_menu[amount_buttons_in_upgrade_menu] = 
  {"+health", "+pure damage", "+speed", "+jump height", "+shot ordinary", "+shot homing", "back" };
  button_type button_types_upgrade_menu[amount_buttons_in_upgrade_menu] = { BUTTON_UPGRADE_HEALTH, BUTTON_UPGRADE_PURE_DAMAGE, 
    BUTTON_UPGRADE_SPEED, BUTTON_UPGRADE_JUMP_HEIGHT, BUTTON_UPGRADE_SHOT_ORDINARY, BUTTON_UPGRADE_SHOT_HOMING, BUTTON_STOP_GAME_MENU };
  create_buttons(buttons_upgrade_menu, button_types_upgrade_menu, button_names_upgrade_menu, amount_buttons_in_upgrade_menu, 20, 20, 250, 80, 30);
}

void updating_game_menu_events(int* is_mouse_button_left) {
	while (SDL_PollEvent(&events)) {
		switch (events.type) {
      case SDL_QUIT: {
        is_running = NO_IS_RUNNING;
      } break;
      case SDL_KEYDOWN:
        switch (events.key.keysym.scancode) {
          case SDL_SCANCODE_ESCAPE: {
            switch (is_running) {
              case GAME: case SAVE_MENU: case CHANGE_SKIN_MENU: case UPGRADE_MENU: {
                is_running = STOP_GAME_MENU;
              } break;
              case STOP_GAME_MENU: {
                is_running = GAME;
              } break;
            }
          } break;
      } break;
      case SDL_MOUSEBUTTONUP: {
        if (events.button.button == SDL_BUTTON_LEFT)
          *is_mouse_button_left = 1;
      } break;
    }
	}
}

void logic_game_menu(int is_mouse_button_left) {
  switch (is_running) {
    case STOP_GAME_MENU: {
      for (int i = 0; i < amount_buttons_in_stop_menu; ++i) {
        if (!button_collision(&buttons_stop_menu[i]))
          continue;
        switch (buttons_stop_menu[i].type) {
          case BUTTON_CONTINUE: {
            if (is_mouse_button_left) {
              is_running = GAME;
              return;
            }
          } break;
          case BUTTON_UPGRADE: {
            if (is_mouse_button_left) {
              is_running = UPGRADE_MENU;
              return;
            }
          } break;
          case BUTTON_CHANGE_SKIN: {
            if (is_mouse_button_left) {
              is_running = CHANGE_SKIN_MENU;
              return;
            }
          } break;
          case BUTTON_SAVE_MENU: { 
            if (is_mouse_button_left) {
              is_running = SAVE_MENU;
              return;
            }
          } break;
          case BUTTON_MAIN_MENU: {
            if (is_mouse_button_left) {
              is_running = MAIN_MENU;
              is_init_game = 1;
              return;
            }
          } break;
        }
      }
    } break;
    case SAVE_MENU: {
      for (int i = 0; i < amount_buttons_in_save_menu; ++i) {
        if (!button_collision(&buttons_save_menu[i]))
          continue;
        switch (buttons_save_menu[i].type) {
          case BUTTON_SAVE: {
            if (is_mouse_button_left) {
              //логика сохранения текущего прогресса в файл (отсюда достать только путь к файлу куда сохранять и отдать в функцию).
              return;
            }
          } break;
          case BUTTON_STOP_GAME_MENU: {
            if (is_mouse_button_left) {
              is_running = STOP_GAME_MENU;
              return;
            }
          } break;
        } 
      }
    } break;
    case CHANGE_SKIN_MENU: {
      for (int i = 0; i < amount_buttons_in_change_skin_menu; ++i) {
        if (!button_collision(&buttons_change_skin_menu[i]))
          continue;
        switch (buttons_change_skin_menu[i].type) {
          case BUTTON_SKIN_RED_KNIGHT: {
            if (is_mouse_button_left) {
              set_skin_hero(HERO_SKIN_RED_KNIGHT);
              is_running = STOP_GAME_MENU;
              return;
            }
          } break;
          case BUTTON_SKIN_GREEN_KNIGHT: {
            if (is_mouse_button_left) {
              set_skin_hero(HERO_SKIN_GREEN_KNIGHT);
              is_running = STOP_GAME_MENU;
              return;
            }
          } break;
          case BUTTON_STOP_GAME_MENU: {
            if (is_mouse_button_left) {
              is_running = STOP_GAME_MENU;
              return;
            }
          } break;
        }
      }
    } break;
    case UPGRADE_MENU: {
      for (int i = 0; i < amount_buttons_in_upgrade_menu; ++i) {
        if (!button_collision(&buttons_upgrade_menu[i]))
          continue;
        switch (buttons_upgrade_menu[i].type) {
          case BUTTON_UPGRADE_HEALTH: {
            if (is_mouse_button_left) {
              if (cost_of_improvement <= hero->amount_experience) {
                hero->health += 3;
                subtract_experience_hero();
              }
            }
          } break;
          case BUTTON_UPGRADE_SPEED: {
            if (is_mouse_button_left) {
              if (cost_of_improvement <= hero->amount_experience) {
                hero->speed += 10;
                subtract_experience_hero();
              }
            }
          } break;
          case BUTTON_UPGRADE_JUMP_HEIGHT: {
            if (is_mouse_button_left) {
              if (cost_of_improvement <= hero->amount_experience) {
                hero->jump_height += 15;
                subtract_experience_hero();
              }
            }
          } break;
          case BUTTON_UPGRADE_PURE_DAMAGE: {
            if (is_mouse_button_left) {
              if (cost_of_improvement <= hero->amount_experience) {
                hero->attack.pure_damage += 2;
                subtract_experience_hero();
              }
            }
          } break;
          case BUTTON_UPGRADE_SHOT_ORDINARY: {
            if (is_mouse_button_left) {
              if (cost_of_improvement <= hero->amount_experience &&
                  !hero->attack.shot.available_types_of_shots[SHOT_TYPE_ORDINARY]) {
                hero->attack.shot.available_types_of_shots[SHOT_TYPE_ORDINARY] = 1;
                subtract_experience_hero();
              }
            }
          } break;
          case BUTTON_UPGRADE_SHOT_HOMING: {
            if (is_mouse_button_left) {
              if (cost_of_improvement <= hero->amount_experience &&
                  !hero->attack.shot.available_types_of_shots[SHOT_TYPE_HOMING]) {
                hero->attack.shot.available_types_of_shots[SHOT_TYPE_HOMING] = 1;
                subtract_experience_hero();
              }
            }
          } break;
          case BUTTON_STOP_GAME_MENU: {
            if (is_mouse_button_left) {
              is_running = STOP_GAME_MENU;
              return;
            }
          } break;
        }
      }
    } break;
  }
}

void subtract_experience_hero() {
  hero->amount_experience -= cost_of_improvement;
}

void draw_game_menu_buttons() {
  switch (is_running) {
    case STOP_GAME_MENU: {
      draw_buttons(buttons_stop_menu, amount_buttons_in_stop_menu); 
    } break;
    case SAVE_MENU: {
      draw_buttons(buttons_save_menu, amount_buttons_in_save_menu);
    } break;
    case UPGRADE_MENU: {
      draw_buttons(buttons_upgrade_menu, amount_buttons_in_upgrade_menu);
      draw_characteristics_hero();
    } break;
    case CHANGE_SKIN_MENU: {
      draw_buttons(buttons_change_skin_menu, amount_buttons_in_change_skin_menu);
    } break;
  }
}

void draw_characteristics_hero() {
  draw_something_characteristics_hero("amount experience", hero->amount_experience, 350, 50); 
  draw_something_characteristics_hero("health", hero->health, 350, 100); 
  draw_something_characteristics_hero("speed", hero->speed, 350, 150); 
  draw_something_characteristics_hero("pure damage", hero->attack.pure_damage, 350, 200); 
  draw_something_characteristics_hero("jump height", hero->jump_height, 350, 250); 
}

void draw_something_characteristics_hero(const char* characteristics_name, double value, int x, int y) {
  const int buf_size = 256;
  char buf[buf_size] = "";
  snprintf(buf, buf_size, "%s: %.1lf", characteristics_name, value);
  SDL_Rect size_text;
  SDL_Texture* text_health = create_font(font_path, buf, {255, 255, 255, 255}, &size_text, {0, 0, 0, 0}, 45);
  size_text.x = 0;
  size_text.y = 0;
  SDL_Rect dst_text_health = {x, y, size_text.w, size_text.h};
  SDL_RenderCopy(renderer, text_health, &size_text, &dst_text_health);
  SDL_DestroyTexture(text_health);
}

void de_init_game_menu() {
  free(buttons_save_menu);
  free(buttons_change_skin_menu);
  free(buttons_stop_menu);
}