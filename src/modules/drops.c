#include "game.h"

Drop drops[NUM_DROPS];

ALLEGRO_BITMAP *img_health = NULL;
ALLEGRO_BITMAP *img_coin_sheet = NULL;

Drop health;

void drops_load(void)
{
    img_health = al_load_bitmap("assets/drops/heart.png");
    img_coin_sheet = al_load_bitmap("assets/drops/coin_sheet.png");

    drops_reset();
}

void drops_reset(void)
{
    for (int i = 0; i < NUM_DROPS; i++)
    {
        drops[i].object.alive = 0;
        drops[i].object.state = 0;
        drops[i].object.speedX = 0;
        drops[i].object.speedY = 0;
        drops[i].object.death_animation_countdown = 0;
    }
}

int drops_create(DropType type, int x, int y)
{
    for (int i = 0; i < NUM_DROPS; i++)
    {
        if (drops[i].object.alive)
            continue;
        Movable_Object drop_object = {
            .x = x,
            .y = y,
            .alive = 1,
            .state = 0,
            .has_animation = 0,
            .total_states = 1,
            .death_explosion_size = 1,
            .death_animation_countdown = 0,
            .animation_speed = 1,
            .speedX = 4,
            .speedY = 0,
            .dirX = -1,
            .dirY = 0,
            .speedX = 1,
            .speedY = 1,
            .type = DROP,
        };

        Drop drop;
        drop.object = drop_object;
        switch (type)
        {
        case HEALTH:
            drop.object.image = img_health;
            drop.object.sprite_length = al_get_bitmap_width(drop.object.image);
            drop.effect = drops_health_effect;
            break;
        case COIN:
            drop.object.image = img_coin_sheet;
            drop.object.has_animation = 1;
            drop.object.sprite_length = 48;
            drop.object.total_states = 6;
            drop.effect = drops_coin_effect;
            break;
        }

        drops[i] = drop;
        break;
    }
}

void drops_update(void)
{
    for (int i = 0; i < NUM_DROPS; i++)
    {
        motion_update(&drops[i].object);

        if (drops[i].object.alive)
        {

            int hit = 0;

            /* Checks for collisions each 0.25s */
            if (al_get_timer_count(timer) % 10 == 0)
            {
                ObType collision = motion_collision(&drops[i].object);
                if (collision != NONE)
                {
                    hit = 1;
                    switch (collision)
                    {
                    /* Pick-up the drop */
                    case SHIP:
                        drops[i].effect();
                    }
                }

                if (hit)
                {
                    drops[i].object.alive = 0;
                }
            }
        }
    }
}

void drops_health_effect(void)
{
    Ship *ship;
    ship = ship_get_ship();
    if (ship->health < 5)
    {
        ship->health++;
    }
}

void drops_coin_effect(void)
{
    coins++;
}