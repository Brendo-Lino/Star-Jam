#include "game.h"

ALLEGRO_TIMER *motion_timer = NULL;

ALLEGRO_BITMAP *explosion = NULL;

void motion_update(Movable_Object *object)
{
    if (explosion == NULL)
    {
        explosion = al_load_bitmap("assets/explosions/normal.png");
    }
    if (object->alive)
    {
        if (object->type != SHIP)
        {
            object->x += object->speedX * object->dirX;
            object->y += object->speedY * object->dirY;
        }

        object->box.start_x = object->x;
        object->box.end_x = object->box.start_x + object->sprite_length;
        object->box.start_y = object->y;
        object->box.end_y = object->box.start_y + al_get_bitmap_height(object->image);
        /* Updates the state each 0.15s */
        if (object->has_animation)
        {
            if (al_get_timer_count(timer) % ((int)(15 / object->animation_speed)) == 0)
            {
                object->state = object->state >= object->total_states - 1 ? 0 : object->state + 1;
            }
        }

        /* Destoys the object if it leaves the screen */
        if (object->dirX == -1)
        {
            if (object->x + object->sprite_length <= 0)
            {
                object->alive = 0;
            }
        }

        if (object->dirX == 1)
        {
            if (object->x - object->sprite_length >= 1280)
            {
                object->alive = 0;
            }
        }
        /* Draws the entity if it stills alive */

        if (object->alive)
        {
            al_draw_bitmap_region(object->image, object->state * object->sprite_length, 0, object->sprite_length, al_get_bitmap_height(object->image), object->x, object->y, 0);
        }
    }
    else
    {
        if (object->death_animation_countdown > 0 && object->death_explosion_size != 0 && object->type != SHOT)
        {
            ALLEGRO_BITMAP *death_img;
            int size;
            switch (object->death_explosion_size)
            {
            case 1:
                death_img = explosion;
                size = 64;
            case 2:
                death_img = explosion;
                size = 64;
            }

            al_draw_bitmap_region(death_img, abs((object->death_animation_countdown / 5) - 5) * size, 0, size, size, object->x, object->y, 1);
            object->death_animation_countdown--;
        }
    }
}

ObType motion_collision(Movable_Object *object)
{
    Ship *ship;
    ship = ship_get_ship();

    Planet *planets;
    planets = planets_get_planets();

    Entity *entities;
    entities = entities_get_entities();

    /* Returns if it hit a planet */
    if (object->type != PLANET)
    {
        for (int j = 0; j < NUM_PLANETS; j++)
        {
            if (planets[j].object.alive)
            {
                if (collisions_check_planet(&planets[j], object->box))
                {
                    return PLANET;
                }
            }
        }
    }

    /* Returns if it hit a entities */
    for (int j = 0; j < NUM_ENTITIES; j++)
    {
        if (entities[j].object.alive)
        {
            if (entities[j].object.id != object->id)
            {
                if (collisions_check_box(entities[j].object.box, object->box))
                {
                    return ENTITY;
                }
            }
        }
    }

    if (object->type != SHIP)
    {
        if (collisions_check_box(ship->object.box, object->box))
        {
            return SHIP;
        }
    }

    return NONE;
}