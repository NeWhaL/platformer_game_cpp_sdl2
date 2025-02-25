#include "../include/other_function.h"

void updating_mouse_state() {
	mouse_buttons_state = SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
}

void updating_dt() {
	static double last_time = 0;
	double new_time = SDL_GetTicks();
	dt = new_time - last_time;
	last_time = new_time;
}

float speed_dt(float speed) {
  return speed * dt / 1000;
}

SDL_Texture* load_texture_from_file(const char* filename, SDL_Rect* size) {
	SDL_Surface* surface = IMG_Load(filename);
	if (!surface) {
		printf("Не удалось загрузить изображение с именем %s!\nОшибка: %s", filename, SDL_GetError());
		de_init_application(1);
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (size)
		*size = { 0, 0, surface->w, surface->h };
	SDL_FreeSurface(surface);
	return texture;
}

collision_direction collision_of_two_objects(const SDL_Rect *hitbox_anyone, const SDL_Rect* hitbox_block) {
  // collision_direction collision = COLLISION_NONE;
  if (hitbox_anyone->x + hitbox_anyone->w >= hitbox_block->x &&
		!(hitbox_anyone->y >= hitbox_block->y + hitbox_block->h ||
			hitbox_anyone->y + hitbox_anyone->h <= hitbox_block->y) &&
			hitbox_anyone->x < hitbox_block->x) {
		return COLLISION_LEFT;
  } 
	if (hitbox_anyone->x <= hitbox_block->x + hitbox_block->w &&
		!(hitbox_anyone->y >= hitbox_block->y + hitbox_block->h ||
			hitbox_anyone->y + hitbox_anyone->h <= hitbox_block->y) &&
			hitbox_anyone->x + hitbox_anyone->w > hitbox_block->x + hitbox_block->w) {
		return COLLISION_RIGHT;
  }
	if (hitbox_anyone->y + hitbox_anyone->h >= hitbox_block->y &&
		!(hitbox_anyone->x >= hitbox_block->x + hitbox_block->w ||
			hitbox_anyone->x + hitbox_anyone->w <= hitbox_block->x) &&
			hitbox_anyone->y < hitbox_block->y) {
		return COLLISION_UP;
  }
	if (hitbox_anyone->y <= hitbox_block->y + hitbox_block->h &&
		!(hitbox_anyone->x >= hitbox_block->x + hitbox_block->w ||
			hitbox_anyone->x + hitbox_anyone->w <= hitbox_block->x) &&
			hitbox_anyone->y +hitbox_anyone->h > hitbox_block->y + hitbox_block->h) {
		return COLLISION_DOWN;
  }
  return COLLISION_NONE;
}

void synchronize_hitbox_with_coordinates(SDL_Rect* hitbox, SDL_FPoint coordinates) {
  hitbox->x = (int)coordinates.x;
  hitbox->y = (int)coordinates.y;
}

void gravity(SDL_FPoint* coordinates) {
	coordinates->y += speed_dt(speed_gravity);
}

void init_texture(Texture* some_texture, const char* path_to_the_sprite_directory) {
  const int buffer_size = 512;
  char data[buffer_size];
  snprintf(data, sizeof(data), "%s%s", path_to_the_sprite_directory, "data.txt");
  FILE* f = fopen(data, "r");
  if (!f) {
    printf("Не удалось загрузить спрайты из %s...\n", path_to_the_sprite_directory);
    de_init_application(1);
  }
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
      de_init_application(1);
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

double read_from_file_double(FILE* file) {
	double data = 0.;
	fscanf(file, "%lf", &data);
	fgetc(file);
	return data;
}