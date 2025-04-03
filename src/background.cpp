#include "../include/background.h"

Background* backgrounds = NULL;
const int amount_backgrounds = 5;
const char* backgrounds_path[amount_backgrounds] {
  "level_1.png", "level_2.png", "level_3.png", "level_3.png", "level_2.png", 
};
Background_type backgrounds_type[amount_backgrounds] = { 
  BACKGROUND_LEVEL_1,
  BACKGROUND_LEVEL_2, 
  BACKGROUND_LEVEL_3, 
  BACKGROUND_MAIN_MENU, 
  BACKGROUND_GAME_MENU 
};

void init_backgrounds() {
  if (!(backgrounds = (Background*)malloc(sizeof(Background) * amount_backgrounds))) {
    printf("Не удалось выделить ресурсы под backgrounds...\n");
    // de_init_application(1);
  }
  char final_path[100];
  for (int i = 0; i < amount_backgrounds; ++i) {
    snprintf(final_path, sizeof(final_path), "../game_images/backgrounds/%s", backgrounds_path[i]);
    create_background(&backgrounds[i], final_path, backgrounds_type[i]);
  }
}

void de_init_backgrounds() {
  for (int i = 0; i < amount_backgrounds; ++i) {
    if (backgrounds[i].texture)
      SDL_DestroyTexture(backgrounds[i].texture);
  }
  free(backgrounds);
}

void create_background(Background* b, const char* filename, Background_type type, SDL_Color color) {
  b->color = color;
  SDL_Rect size;
  b->texture = load_texture_from_file(filename, &size);
  b->size = size;
  b->type = type;
}

void destroy_background(Background* b) {
  SDL_DestroyTexture(b->texture);
  free(b);
}

void draw_background(Background* b) {
  SDL_SetRenderDrawColor(renderer, b->color.r, b->color.g, b->color.b, b->color.a);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, b->texture, NULL, NULL);
}