#include "../include/menu.h"

button button_start;
button button_load;
button button_exit;
button button_load_1;
button button_load_2;
button button_load_3;
button button_load_back_main_menu;
SDL_Color current_color_button = {140, 140, 240, 255};
SDL_Color standart_color_button = {150, 150, 220, 255};
SDL_Color standart_color_text = {180, 180, 240, 255};
const char* font_path = "../fonts/Ubuntu-M.ttf";
const int button_width = window_width / 3;
const int button_height = 150;

void menu() {
  create_buttons();
  while (is_running_main_menu || is_running_load_menu) {
    updating_mouse_state();
    updating_events();  
    logic_menu();
    draw_menu();
  }
}

void create_buttons() {
  int left_margin = window_width / 3;
  int margin_between_buttons = 100;
  create_button(
    &button_start,
    "new game",
    left_margin,
    margin_between_buttons
  );
  create_button(
    &button_load,
    "load",
    left_margin,
    button_start.size_button.y + button_start.size_button.h + margin_between_buttons
  );
  create_button(
    &button_exit,
    "exit",
    left_margin,
    button_load.size_button.y + button_load.size_button.h + margin_between_buttons
  );
  create_button(
    &button_load_1,
    "first save",
    left_margin,
    margin_between_buttons
  );
  create_button(
    &button_load_2, 
    "second save", 
    left_margin, 
    button_load_1.size_button.y + button_load_1.size_button.h + margin_between_buttons
  );
  create_button(
    &button_load_3, 
    "third save", 
    left_margin, 
    button_load_2.size_button.y + button_load_2.size_button.h + margin_between_buttons
  );
  create_button(
    &button_load_back_main_menu, 
    "main menu", 
    left_margin, 
    button_load_3.size_button.y + button_load_3.size_button.h + margin_between_buttons
  );
}

void create_button(button* btn, const char* text, int x, int y) {
  btn->color_button = standart_color_button;
  btn->size_button = { x, y, button_width, button_height };
  btn->font = create_font(font_path, text, standart_color_text, &btn->size_text, btn->size_button);
}

void updating_events() {	
	while (SDL_PollEvent(&events)) {
		switch (events.type) {
		case SDL_QUIT: {
			is_running_main_menu = 0;
      is_running_load_menu = 0;
		} break;
		case SDL_KEYDOWN:
			switch (events.key.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE: {
				is_running_main_menu = 0;
        is_running_load_menu = 0;
			} break;
			}
		} break;
	}
}

int button_collision(button* btn, SDL_Color std_color, SDL_Color cur_color) {
  SDL_Rect btn_size = btn->size_button;
  int collision = 0;
  if (collision = button_collision_rect(btn_size)) {
    btn->color_button = current_color_button;
  } else
    btn->color_button = standart_color_button;
  return collision;
}

int button_collision_rect(SDL_Rect btn_rect) {
  return mouse_position.x >= btn_rect.x && 
         mouse_position.x <= btn_rect.x + btn_rect.w && 
         mouse_position.y >= btn_rect.y && 
         mouse_position.y <= btn_rect.y + btn_rect.h;
}

void logic_menu() {
  if (is_running_main_menu) {
    if (button_collision(&button_start)) {
      if (mouse_buttons_state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        is_running_main_menu = 0;
        is_running_game = 1;
      }
    } else if (button_collision(&button_load)) {
      if (mouse_buttons_state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        is_running_main_menu = 0;
        is_running_load_menu = 1;
      }
    } else if (button_collision(&button_exit)) {
      if (mouse_buttons_state & SDL_BUTTON(SDL_BUTTON_LEFT))
        is_running_main_menu = 0;
    }
    return;
  } 
  if (button_collision(&button_load_1)) {

  } else if (button_collision(&button_load_2)) {

  } else if (button_collision(&button_load_3)) {

  } else if (button_collision(&button_load_back_main_menu)) { 
    if (mouse_buttons_state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
      is_running_main_menu = 1;
      is_running_load_menu = 0;
    }
  }
}

void draw_menu() {
  SDL_SetRenderDrawColor(renderer, 180, 180, 240, 255);
  SDL_RenderClear(renderer);
  draw_buttons();
  SDL_RenderPresent(renderer);
}

void draw_buttons() {
  if (is_running_main_menu) {
    draw_button(&button_start);
    draw_button(&button_load);
    draw_button(&button_exit);
    return;
  } 
  draw_button(&button_load_1);
  draw_button(&button_load_2);
  draw_button(&button_load_3);
  draw_button(&button_load_back_main_menu); 
}

void draw_button(button* btn) {
  SDL_SetRenderDrawColor(renderer, btn->color_button.r, btn->color_button.g, btn->color_button.b, btn->color_button.a);
  SDL_RenderFillRect(renderer, &btn->size_button);
  SDL_RenderCopy(renderer, btn->font, NULL, &btn->size_text);
}