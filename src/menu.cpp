#include "../include/menu.h"

button* buttons_main_menu;
button* buttons_load_menu;
button* buttons_stop_menu;
button* buttons_save_menu;
SDL_Color standart_background_color =  {180, 180, 240, 255};
SDL_Color current_color_button = {140, 140, 240, 255};
SDL_Color standart_color_button = {150, 150, 220, 255};
SDL_Color standart_color_text = {180, 180, 240, 255};
const char* font_path = "../fonts/Ubuntu-M.ttf";
const int button_width = window_width / 3;
const int button_height = 120;
const int left_margin = window_width / 3;
const int up_margin = 80;
const int amount_buttons_in_main_menu = 3;
const int amount_buttons_in_load_menu = 4;
const int amount_buttons_in_stop_menu = 5;
const int amount_buttons_in_save_menu = 4;

void create_button(button* btn, button_type type, const char* text, int x, int y, int w, int h,
                   SDL_Color std_color, SDL_Color cur_color) {
  btn->type = type;
  btn->standart_color_button = standart_color_button;
  btn->active_color_button = cur_color;
  btn->color_button = btn->standart_color_button;
  btn->size_button = { x, y, button_width, button_height };
  btn->font = create_font(font_path, text, standart_color_text, &btn->size_text, btn->size_button);
}

int button_collision(button* btn) {
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

void draw_button(button* btn) {
  SDL_SetRenderDrawColor(renderer, btn->color_button.r, btn->color_button.g, btn->color_button.b, btn->color_button.a);
  SDL_RenderFillRect(renderer, &btn->size_button);
  SDL_RenderCopy(renderer, btn->font, NULL, &btn->size_text);
}

void draw_menu(void draw_buttons_function(), SDL_Color background_color) {
  SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a);
  SDL_RenderClear(renderer);
  draw_buttons_function();
  SDL_RenderPresent(renderer);
}

void buttons_malloc(button** buttons, int size) {
  *buttons = (button*)malloc(sizeof(button) * size);
}

// void create_buttons(button* buttons, button_type* types, const int size, const char* names[size]) {
//   int i_up_margin = up_margin;
//   for (int i = 0; i < amount_buttons_in_main_menu; ++i) {
//     create_button(
//       &buttons_main_menu[i],
//       button_types_main_menu[i],
//       button_names_main_menu[i],
//       left_margin,
//       i_up_margin
//     );
//     i_up_margin = buttons_main_menu[i].size_button.y + buttons_main_menu[i].size_button.h + up_margin;
//   }
// }



void main_menu() {
  static int is_button_created = 0;
  if (!is_button_created)
    create_main_menu_buttons();
  is_button_created = 1;
  int is_mouse_button_left = 0;
  while (is_running == MAIN_MENU || is_running == LOAD_MENU) {
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
  int i_up_margin = up_margin;
  for (int i = 0; i < amount_buttons_in_main_menu; ++i) {
    create_button(
      &buttons_main_menu[i],
      button_types_main_menu[i],
      button_names_main_menu[i],
      left_margin,
      i_up_margin
    );
    i_up_margin = buttons_main_menu[i].size_button.y + buttons_main_menu[i].size_button.h + up_margin;
  }

  buttons_malloc(&buttons_load_menu, amount_buttons_in_load_menu);
  const char* button_names_load_menu[amount_buttons_in_load_menu] = { "load 1", "load 2", "load 3", "main menu"};
  button_type button_types_load_menu[amount_buttons_in_load_menu] = { BUTTON_LOAD, BUTTON_LOAD, BUTTON_LOAD, BUTTON_MAIN_MENU };
  i_up_margin = up_margin;
  for (int i = 0; i < amount_buttons_in_load_menu; ++i) {
    create_button(
      &buttons_load_menu[i],
      button_types_load_menu[i],
      button_names_load_menu[i],
      left_margin,
      i_up_margin
    );
    i_up_margin = buttons_load_menu[i].size_button.y + buttons_load_menu[i].size_button.h + up_margin;
  }
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
  static int is_button_created = 0;
  if (!is_button_created)
    create_game_menu_buttons();
  is_button_created = 1;
  int is_mouse_button_left = 0;
  while (is_running == STOP_GAME_MENU || is_running == SAVE_MENU ||
         is_running == UPGRADE_MENU || is_running == CHANGE_SKIN_MENU) {
    updating_mouse_state();
    updating_game_menu_events(&is_mouse_button_left);
    logic_game_menu(is_mouse_button_left);
    draw_menu(draw_game_menu_buttons);
    is_mouse_button_left = 0;
  }
}

void create_game_menu_buttons() {
  buttons_malloc(&buttons_stop_menu, amount_buttons_in_stop_menu);
  const char* button_names_stop_menu[amount_buttons_in_stop_menu] = { "continue", "save", "upgrade", "change skin", "main menu"};
  button_type button_types_stop_menu[amount_buttons_in_stop_menu] = { BUTTON_CONTINUE, BUTTON_SAVE_MENU, 
    BUTTON_UPGRADE, BUTTON_CHANGE_SKIN, BUTTON_MAIN_MENU };
  int i_up_margin = up_margin - 70;
  for (int i = 0; i < amount_buttons_in_stop_menu; ++i) {
    create_button(
      &buttons_stop_menu[i],
      button_types_stop_menu[i],
      button_names_stop_menu[i],
      left_margin,
      i_up_margin
    );
    i_up_margin = buttons_stop_menu[i].size_button.y + buttons_stop_menu[i].size_button.h + up_margin - 70;
  }

  buttons_malloc(&buttons_save_menu, amount_buttons_in_save_menu);
  const char* button_names_save_menu[amount_buttons_in_save_menu] = { "save 1", "save 2", "save 3", "back"};
  button_type button_types_save_menu[amount_buttons_in_save_menu] = { BUTTON_SAVE, BUTTON_SAVE, BUTTON_SAVE, BUTTON_STOP_GAME_MENU };
  i_up_margin = up_margin - 70;
  for (int i = 0; i < amount_buttons_in_save_menu; ++i) {
    create_button(
      &buttons_save_menu[i],
      button_types_save_menu[i],
      button_names_save_menu[i],
      left_margin,
      i_up_margin
    );
    i_up_margin = buttons_save_menu[i].size_button.y + buttons_save_menu[i].size_button.h + up_margin - 70;
  }
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
  }
}

void draw_game_menu_buttons() {
  if (is_running == STOP_GAME_MENU) {
    for (int i = 0; i < amount_buttons_in_stop_menu; ++i)
      draw_button(&buttons_stop_menu[i]);
  } else if (is_running == SAVE_MENU) {
    for (int i = 0; i < amount_buttons_in_save_menu; ++i)
      draw_button(&buttons_save_menu[i]);
  } else if (is_running == UPGRADE_MENU) {

  } else if (is_running == CHANGE_SKIN_MENU) {

  }
}

void de_init_game_menu() {
  free(buttons_save_menu);
  free(buttons_stop_menu);
}