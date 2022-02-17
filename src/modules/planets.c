#include "game.h"

Planet planets[NUM_PLANETS];

ALLEGRO_BITMAP *images[NUM_IMAGES];

int aux = 0;

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

    for (int i = 0; i < NUM_PLANETS; i++)
    {
        planets[i].object.alive = 0;
    }
}

void planets_create_random_planet(int x, int y, int speed)
{
    for (int i = 0; i < NUM_PLANETS; i++)
    {
        if (!planets[i].object.alive)
        {
            int tooBig = 0;
            if (aux >= 1280)
            {
                aux = 0;
                return;
            }
            planets[i].object.id = i;
            planets[i].object.alive = 1;
            if (rdm(1, 3) == 1 && y == 0 && boss > 0)
            {
                planets[i].object.image = images[38];
            }
            else
            {
                planets[i].object.image = images[rdm(0, NUM_IMAGES - 2)];
            }
            planets[i].size = al_get_bitmap_height(planets[i].object.image);
            if (planets[i].size >= 700)
            {
                tooBig = 1;
            }
            if (x == 0)
                x = 1280 + 1280 * 0.1;
            if (y == 0)
                if (!tooBig)
                    y = rdm(0, 720 - planets[i].size);
                else
                    y = 300;
            planets[i].object.x = x;
            planets[i].object.y = y;
            if (speed == 0)
                speed = rdm(3, 5);
            planets[i].object.speedX = speed;
            planets[i].object.speedY = 0;
            planets[i].object.dirX = -1;
            planets[i].object.dirY = 0;
            planets[i].object.state = 1;
            planets[i].object.has_animation = 1;
            planets[i].object.animation_speed = 1;
            planets[i].object.sprite_length = planets[i].size;
            planets[i].object.total_states = al_get_bitmap_width(planets[i].object.image) / planets[i].size;
            planets[i].object.type = PLANET;
            aux += planets[i].size;
            if (!tooBig)
                planets_create_random_planet(x + planets[i].size * 1.2, y, speed);
            break;
        }
    }
}

void planets_update(void)
{

    /* Attemps to create a something else each 3 seconds */
    if (al_get_timer_count(timer) % (100 * 3) == 0)
    {
        int one_alive = 0;
        for (int i = 0; i < NUM_PLANETS; i++)
        {
            if (planets[i].object.alive)
            {
                one_alive = 1;
                break;
            }
        }
        if (!one_alive)
            planets_create_random_planet(0, 0, 0);
    }

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
