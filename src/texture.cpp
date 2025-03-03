#include "../include/texture.h"

void init_texture(Texture* some_texture, const char* path_to_the_sprite_directory) {
  const int buffer_size = 512;
  char data[buffer_size];
  snprintf(data, sizeof(data), "%s%s", path_to_the_sprite_directory, "data.txt");
  FILE* f = fopen(data, "r");
  if (!f) {
    printf("Не удалось загрузить спрайты из %s...\n", path_to_the_sprite_directory);
    exit(1);
  }
	//переменная, отвечающая за цикличность анимации.
	fscanf(f, "%d", &some_texture->repeat_animation);
	fgetc(f);
	//переменная, отвечающая за количество спрайтов в текстуре.
  fscanf(f, "%d", &some_texture->amount_sprite);
	fgetc(f);
  some_texture->sprites = (Sprite*)malloc(sizeof(Sprite) * some_texture->amount_sprite);
  for (int i = 0; i < some_texture->amount_sprite; ++i) {
		Sprite* current_sprite = &some_texture->sprites[i];
		fscanf(f, "%lf", &current_sprite->rendering_time);
		fgetc(f);
    char sprite_name[buffer_size / 2];
    if (fgets(sprite_name, sizeof(sprite_name), f) == NULL) {
      printf("Ошибка: фактическое число спрайтов оказалось меньше, чем указано в файле %s в первой строке...", data);
      fclose(f);
      exit(1);
    }
		int size_sprite_name = strlen(sprite_name);
		if (size_sprite_name > 0 && sprite_name[size_sprite_name - 1] == '\n') {
			sprite_name[size_sprite_name - 1] = '\0';
		}
		char sprite_path[buffer_size];
		snprintf(sprite_path, buffer_size, "%s%s", path_to_the_sprite_directory, sprite_name);
    current_sprite->sprite = load_texture_from_file(sprite_path, &current_sprite->size);
  }
  fclose(f);
}

void de_init_texture(Texture* texture) {
  for (int i = 0; i < texture->amount_sprite; ++i)
    SDL_DestroyTexture(texture->sprites[i].sprite);
  free(texture->sprites);
}

SDL_Texture* load_texture_from_file(const char* filename, SDL_Rect* size) {
	SDL_Surface* surface = IMG_Load(filename);
	if (!surface) {
		printf("Не удалось загрузить изображение с именем %s!\nОшибка: %s", filename, SDL_GetError());
    exit(1);
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (size)
		*size = { 0, 0, surface->w, surface->h };
	SDL_FreeSurface(surface);
	return texture;
}

double time_for_one_texture_iteration(Texture* texture) {
	double total_time = 0;
	for (int i = 0; i < texture->amount_sprite; ++i)
		total_time += texture->sprites[i].rendering_time;
	return total_time;
}

void set_current_sprite(Texture *texture, int *current_number_sprite, SDL_Rect *hitbox, SDL_FPoint *coordinates,
		direction_movement direction, double *sprite_time_counter, int texture_changed) {
  *sprite_time_counter += dt;
  float height_difference = 0.f;
  float width_difference = 0.f;
  if (texture_changed) {
    hitbox_change_due_new_sprite(hitbox, direction, texture, *current_number_sprite, &height_difference, &width_difference);
    *sprite_time_counter = 0;
  }
  if ((*sprite_time_counter) > texture->sprites[*current_number_sprite].rendering_time) {
    (*current_number_sprite)++; 
    if (*current_number_sprite == texture->amount_sprite) {
      if (texture->repeat_animation)
        *current_number_sprite = 0;
      else
        (*current_number_sprite)--;
    }
    hitbox_change_due_new_sprite(hitbox, direction, texture, *current_number_sprite, &height_difference, &width_difference);
    *sprite_time_counter = 0;
  }
  coordinates->y += height_difference;
  coordinates->x -= width_difference;
  synchronize_hitbox_with_coordinates(hitbox, *coordinates);
}

void hitbox_change_due_new_sprite(SDL_Rect* hitbox, direction_movement direction, Texture* current_texture, int current_number_sprite,
		float* height_difference, float* width_difference) {
  if (height_difference)
    *height_difference = hitbox->h - current_texture->sprites[current_number_sprite].size.h;
  if (direction == DIRECTION_LEFT && width_difference != NULL)
    *width_difference = current_texture->sprites[current_number_sprite].size.w - hitbox->w;
  hitbox->w = current_texture->sprites[current_number_sprite].size.w;
  hitbox->h = current_texture->sprites[current_number_sprite].size.h;
} 