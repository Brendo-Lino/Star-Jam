#include "game.h"

ALLEGRO_TIMER *planets_timer = NULL;

Planet planets[NUM_PLANETS];

ALLEGRO_BITMAP *images[NUM_IMAGES];

void planets_load(void)
{

    planets_reset();

    for (int i = 0; i < NUM_IMAGES; i++)
    {

        char path[100];
        sprintf(path, "assets/planets/%d.png", i + 1);
        ALLEGRO_BITMAP *image = al_load_bitmap(path);
        images[i] = image;
    }
}

void planets_reset(void)
{
    planets_timer = al_create_timer(1.0 / 100);
    al_start_timer(planets_timer);

    for (int i = 0; i < NUM_PLANETS; i++)
    {
        planets[i].object.alive = 0;
    }
}

void planets_create_random_planet(void)
{
    for (int i = 0; i < NUM_PLANETS; i++)
    {
        if (!planets[i].object.alive)
        {
            planets[i].object.id = i;
            planets[i].object.alive = 1;
            planets[i].object.image = images[rdm(0, NUM_IMAGES - 1)];
            planets[i].size = al_get_bitmap_height(planets[i].object.image);
            planets[i].object.x = 1280 + 1280 * 0.1;
            planets[i].object.y = rdm(0, 720 - planets[i].size);
            planets[i].object.speedX = rdm(3, 5);
            planets[i].object.speedY = 0;
            planets[i].object.dirX = -1;
            planets[i].object.dirY = 0;
            planets[i].object.state = 1;
            planets[i].object.has_animation = 1;
            planets[i].object.animation_speed = 1;
            planets[i].object.sprite_length = planets[i].size;
            planets[i].object.total_states = 10;
            planets[i].object.type = PLANET;
            break;
        }
    }
}

void planets_update(void)
{

    for (int i = 0; i < NUM_PLANETS; i++)
    {
        if (planets[i].object.alive)
        {
            motion_update(&planets[i].object);
        }
    }
}

Planet *planets_get_planets(void)
{
    return planets;
}
