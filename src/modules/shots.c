#include "game.h"

ALLEGRO_TIMER *shot_timer = NULL;

/* Images */

ALLEGRO_BITMAP *img_shot_hit_sheet = NULL;
ALLEGRO_BITMAP *img_shot_hit_charged_sheet = NULL;

/* Sounds */

Shot shots[NUM_SHOTS];

void shots_load(void)
{

    img_shot_hit_sheet = al_load_bitmap("assets/shots/hit_sheet.png");
    img_shot_hit_charged_sheet = al_load_bitmap("assets/shots/hit_charged_sheet.png");

    shot_timer = al_create_timer(1.0 / 100);

    al_start_timer(shot_timer);
}

void shots_reset(void)
{
    for (int i = 0; i < NUM_SHOTS; i++)
    {
        shots[i].object.id = i;
        shots[i].object.speedX = SHOT_SPEED;
        shots[i].object.speedY = 0;
        shots[i].object.dirX = 1;
        shots[i].object.dirY = 0;
        shots[i].object.alive = 0;
        shots[i].charged = 0;
        shots[i].animation_countdown = 0;
        shots[i].object.type = SHOT;
    }
}

void shots_update(void)
{
    Entity *entities;
    entities = entities_get_entities();

    Planet *planets;
    planets = planets_get_planets();

    for (int i = 0; i < NUM_SHOTS; i++)
    {

        if (shots[i].object.alive)
        {
            motion_update(&shots[i].object);
            /* Checks for collisions each 0.01s */
            if (al_get_timer_count(shot_timer) % 1 == 0)
            {
                /* Destroys the shot if it hits a Planet */
                for (int j = 0; j < NUM_PLANETS; j++)
                {
                    if (planets[j].object.alive)
                    {
                        if (collisions_check_planet(&planets[j], shots[i].object.box))
                        {
                            shots[i].object.alive = 0;
                            shots[i].animation_countdown = 30;
                        }
                    }
                }
            }

            for (int j = 0; j < NUM_ENTITIES; j++)
            {
                if (entities[j].object.alive)
                {
                    if (collisions_check_box(shots[i].object.box, entities[j].object.box))
                    {
                        if (!shots[i].charged)
                        {
                            shots[i].object.alive = 0;
                            shots[i].animation_countdown = 30;
                        }
                        entities[j].health -= 1;
                        if (!shots[i].charged)
                        {
                            if (rdm(1, 5) == 1)
                            {

                                drops_create(HEALTH, entities[j].object.x, entities[j].object.y);
                            }
                            else if (rdm(1, 10) == 2)
                            {
                                drops_create(COIN, entities[j].object.x, entities[j].object.y);
                            }
                        }
                        break;
                    }
                }
            }
        }
        else
        {
            if (shots[i].animation_countdown > 0)
            {
                ALLEGRO_BITMAP *hit_img = shots[i].charged ? img_shot_hit_charged_sheet : img_shot_hit_sheet;
                int size = shots[i].charged ? 48 : 32;
                al_draw_bitmap_region(hit_img, abs((shots[i].animation_countdown / 5) - 5) * size, 0, size, size, shots[i].object.x, shots[i].object.y, 1);
                shots[i].animation_countdown--;
            }
        }
    }
}

Shot *shots_get_shots(void)
{
    return shots;
}
