#include "../include/hero.h"

personage* hero;

void init_hero() {
  if (!(hero = (personage*)malloc(sizeof(personage)))) {
    printf("Не удалось получить ресурсы для personage *hero...\n");
    de_init_application(1);
  }
  if (!(hero->textures = (personage_textures*)malloc(sizeof(personage_textures)))) {
    printf("Не удалось получить ресурсы для personage *hero...\n");
    free(hero);
    de_init_application(1);
  }
  hero->textures->idle = load_texture_from_file("../game_images/Hero Knight/Sprites/HeroKnight/Idle/HeroKnight_Idle_0.png", 
      &hero->textures->size_idle);
  hero->coordinates = { 100, 100 };
  hero->hitbox = { (int)hero->coordinates.x, (int)hero->coordinates.y, hero->textures->size_idle.w, hero->textures->size_idle.h };
  hero->speed = 50;
}

void de_init_hero() {
  free(hero->textures->idle);
  free(hero->textures);
  free(hero);
}

void draw_hero() {
  SDL_RenderCopy(renderer, hero->textures->idle, &hero->textures->size_idle, &hero->hitbox);
}

void update_hero() {
  move_hero();
}

void move_hero() {
  if (keyboard[SDL_SCANCODE_A]) {
    hero->direction = DIRECTION_LEFT;
    hero->coordinates.x -= speed_dt(hero->speed);
  } else if (keyboard[SDL_SCANCODE_D]) {
    hero->direction = DIRECTION_RIGHT;
    hero->coordinates.x += speed_dt(hero->speed);
  } else if (keyboard[SDL_SCANCODE_W]) {
    hero->coordinates.y -= speed_dt(hero->speed);
  } else if (keyboard[SDL_SCANCODE_S]) { 
    hero->coordinates.y += speed_dt(hero->speed);
  }
  hero->hitbox.x = (int)hero->coordinates.x;
  hero->hitbox.y = (int)hero->coordinates.y;
}