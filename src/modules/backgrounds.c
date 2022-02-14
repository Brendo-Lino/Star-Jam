#include "game.h"

ALLEGRO_TIMER *background_timer = NULL;

/* Backgrounds */
Background bg_default;
Background bg_planet;
Background bg_stars;

void backgrounds_load(void)
{
    backgrounds_reset();
}

void backgrounds_reset(void)
{
    background_timer = al_create_timer(1.0 / 60);
    if (!background_timer)
    {
        fprintf(stderr, "failed to create timer!\n");
    }

    Background buffer = {
        .x = 0,
        .y = 0,
        .velX = 0.5,
        .width = 1280,
        .spacing = 1,
        .image = al_load_bitmap("assets/background/background.png"),
    };

    bg_default = buffer;

    Background buffer_2 = {
        .x = 0,
        .y = 0,
        .velX = 0.5,
        .width = 1280,
        .spacing = 1,
        .image = al_load_bitmap("assets/background/stars.png"),
    };

    bg_stars = buffer_2;

    Background buffer_3 = {
        .x = 0,
        .y = 0,
        .velX = 0.35,
        .width = 1280,
        .spacing = 2,
        .image = al_load_bitmap("assets/background/planet.png"),
    };

    bg_planet = buffer_3;
}

void backgrounds_update(void)
{
    backgrounds_update_background(&bg_default);
    backgrounds_update_background(&bg_stars);
    backgrounds_update_background(&bg_planet);
}

void backgrounds_update_background(Background *bg)
{
    bg->x -= bg->velX;
    if (bg->x + (bg->width * bg->spacing) <= 0)
    {
        bg->x = 0;
    }

    al_draw_bitmap(bg->image, bg->x, bg->y, 0);

    al_draw_bitmap(bg->image, bg->x + (bg->width * bg->spacing), bg->y, 0);
}