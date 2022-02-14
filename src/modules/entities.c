#include "game.h"

ALLEGRO_TIMER *entities_timer = NULL;

Entity entities[NUM_ENTITIES];

Entity enemies[NUM_ENEMIES];

ALLEGRO_BITMAP *img_red_guy = NULL;
ALLEGRO_BITMAP *img_explosion_normal = NULL;

void entities_load(void)
{
    entities_reset();

    img_red_guy = al_load_bitmap("assets/enemies/red_boy.png");
    img_explosion_normal = al_load_bitmap("assets/explosions/normal.png");

    Movable_Object red_guy_object = {
        .image = al_load_bitmap("assets/enemies/red_boy.png"),
        .has_animation = 1,
        .total_states = 5,
        .death_explosion_size = 1,
        .sprite_length = 46,
        .animation_speed = 1,
        .animation_speed = 1,
        .speedX = 1,
        .speedY = 1,
        .dirX = -1,
        .dirY = 0,
    };

    Entity red_guy = {
        .object = red_guy_object,
        .health = 1,
        .max_score = 10,
        .min_score = 3};

    enemies[0] = red_guy;
}

void entities_reset(void)
{

    entities_timer = al_create_timer(1.0 / 100);
    al_start_timer(entities_timer);

    for (int i = 0; i < NUM_ENTITIES; i++)
    {
        entities[i].object.alive = 0;
        entities[i].object.state = 0;
        entities[i].object.speedX = 0;
        entities[i].object.speedY = 0;
        entities[i].object.death_animation_countdown = 0;
    }
}

int entities_create(Entity entity, int x, int y)
{
    for (int i = 0; i < NUM_ENTITIES; i++)
    {
        if (entities[i].object.alive)
            continue;
        if (entities[i].object.death_animation_countdown != 0)
            continue;
        entities[i] = entity;
        entities[i].object.id = i;
        entities[i].object.x = x;
        entities[i].object.y = y;
        int factorSpeed = al_get_timer_count(entities_timer) / (100 * 15);
        entities[i].object.speedX = entities[i].object.speedX * rdm(3 + factorSpeed, 6 + factorSpeed);
        entities[i].object.image = entity.object.image;
        entities[i].object.alive = 1;
        entities[i].object.state = 0;
        entities[i].object.death_animation_countdown = 0;
        entities[i].object.animation_speed = 1;
        entities[i].object.type = ENTITY;
        return 1;
    }
}

void entities_create_random_enemy(void)
{
    int n = rdm(0, NUM_ENEMIES - 1);

    entities_create(enemies[0], (int)(SCREEN_W * 1.05), rdm(50, SCREEN_H - 120));
}

void entities_update(void)
{
    Ship *ship;
    ship = ship_get_ship();

    Planet *planets;
    planets = planets_get_planets();

    /* Attempts to create something each 0.5x seconds */
    if (al_get_timer_count(timer) % (int)(100 * 0.2) == 0)
    {
        entities_create_random_enemy();
    }

    for (int i = 0; i < NUM_ENTITIES; i++)
    {
        motion_update(&entities[i].object);

        if (al_get_timer_count(entities_timer) % 50 == 0)
        {
            if (rdm(1, 2) == 1)
            {
                entities[i].object.dirY = rdm(-1, 1);
            }
        }
        if (al_get_timer_count(entities_timer) % 100 == 0)
        {
            entities[i].object.dirY = 0;
        }

        if (entities[i].object.alive)
        {
            /* Checks for collisions each 0.01s */
            int hit = 0;
            if (al_get_timer_count(entities_timer) % 1 == 0)
            {
                ObType collision = motion_collision(&entities[i].object);
                if (collision != NONE)
                {
                    hit = 1;
                    switch (collision)
                    {
                        /* Destroys the entity if it hits the ship */
                    case SHIP:
                        ship->health -= 1;
                    }
                }
            }

            if (hit)
            {
                entities[i].object.alive = 0;
                entities[i].object.state = 0;
                entities[i].object.death_animation_countdown = 30;
            }

            /* Kills the entity if the health drops too low */
            if (entities[i].health <= 0)
            {
                entities[i].object.alive = 0;
                entities[i].object.death_animation_countdown = 30;
                score += rdm(entities[i].min_score, entities[i].max_score);
            }
        }
    }
}

Entity *entities_get_entities(void)
{
    return entities;
}
