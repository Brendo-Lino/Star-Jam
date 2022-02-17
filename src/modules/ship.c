#include "game.h"

ALLEGRO_BITMAP *img_ship_default = NULL;
ALLEGRO_BITMAP *img_ship_up = NULL;
ALLEGRO_BITMAP *img_ship_down = NULL;
ALLEGRO_BITMAP *img_shot_special = NULL;
ALLEGRO_BITMAP *img_shot_charging_sheet = NULL;
ALLEGRO_BITMAP *img_shot_charged = NULL;
ALLEGRO_BITMAP *img_shot_default = NULL;
ALLEGRO_BITMAP *img_shot_flash = NULL;

ALLEGRO_SAMPLE *shot_sound_default = NULL;
ALLEGRO_SAMPLE *ship_explosion = NULL;

Ship ship;

void ship_load(void)
{

    img_ship_default = al_load_bitmap("assets/ship/default.png");
    img_ship_up = al_load_bitmap("assets/ship/up.png");
    img_ship_down = al_load_bitmap("assets/ship/down.png");
    img_shot_special = al_load_bitmap("assets/shots/special_sheet.png");
    img_shot_charging_sheet = al_load_bitmap("assets/shots/charging_shot_sheet.png");
    img_shot_charged = al_load_bitmap("assets/shots/charged.png");
    img_shot_default = al_load_bitmap("assets/shots/default.png");
    img_shot_flash = al_load_bitmap("assets/shots/flash.png");

    shot_sound_default = al_load_sample("assets/soundtrack/shot_default.ogg");
    ship_explosion = al_load_sample("assets/soundtrack/ship_explosion.ogg");
}

void ship_reset(void)
{
    Ship buffer = {
        .health = 5,
        .shot_charge = 0,
        .shot_charging = 0,
        .shot_charged = 0,
        .shot_in_cooldown = 0,
        .recent_shot_countdown = 0,
    };

    ship = buffer;

    Movable_Object ship_object = {
        .x = SCREEN_H / 12,
        .y = SCREEN_W / 4,
        .speedX = SHIP_SPEED,
        .speedY = SHIP_SPEED,
        .dirX = 0,
        .dirY = 0,
        .has_animation = 0,
        .image = img_ship_default,
        .alive = 1,
    };

    ship.object = ship_object;
}

void ship_update(void)
{
    Shot *shots;
    shots = shots_get_shots();

    if (ship.health <= 0)
    {
        al_play_sample(ship_explosion, 0.5f, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
        if (!lost)
        {
            game_lose();
        }
        return;
    }

    /* Borders limits and image update according to the direction */
    if (!(ship.object.x <= 30 && ship.object.dirX < 0))
    {
        ship.object.x += ship.object.dirX * ship.object.speedX;
    }
    if (!((ship.object.y <= 20 + ((ship.object.box.end_y - ship.object.box.start_y) / 2) && ship.object.dirY < 0) || (ship.object.y >= 700 - (ship.object.box.end_y - ship.object.box.start_y) && ship.object.dirY > 0)))

        if (!((ship.object.y <= 20 + ((ship.object.box.end_y - ship.object.box.start_y) / 2) && ship.object.dirY < 0) || (ship.object.y >= 700 - (ship.object.box.end_y - ship.object.box.start_y) && ship.object.dirY > 0)))
        {
            ship.object.y += ship.object.dirY * ship.object.speedY;
            if (ship.object.dirY > 0)
            {
                ship.object.image = img_ship_up;
            }
            if (ship.object.dirY < 0)
            {
                ship.object.image = img_ship_down;
            }
            if (ship.object.dirY == 0)
            {
                ship.object.image = img_ship_default;
            }
        }

    ship.object.sprite_length = al_get_bitmap_width(ship.object.image);
    motion_update(&ship.object);

    /* Checks for collisions each 0.01s */
    if (al_get_timer_count(timer) % 1 == 0)
    {
        ObType collision = motion_collision(&ship.object);
        if (collision != NONE)
        {
            switch (collision)
            {
            case PLANET:
                ship.health = 0;
            }
        }
    }

    // Charging the shot animation
    if (ship.shot_charging)
    {
        for (int j = 0; j < NUM_SHOTS; j++)
        {
            if (!shots[j].object.alive)
            {
                int adapted_shot_charge = ship.shot_charge % 6 - 2;
                int sprite_size = 64;

                if (adapted_shot_charge >= 0)
                {
                    al_draw_bitmap_region(img_shot_charging_sheet, (0 + adapted_shot_charge * sprite_size), 0, sprite_size, sprite_size, ship.object.box.end_x + SCREEN_W / 200, (ship.object.box.end_y + ((ship.object.box.start_y - ship.object.box.end_y) / 2) - sprite_size / 4), 0);
                }

                // Each 1/5 second
                if (al_get_timer_count(timer) % ((int)100 / 5) == 0)
                {
                    if (ship.shot_charge == 4)
                    {
                        ship.shot_charged = 1;
                    }
                    ship.shot_charge += 1;
                }
            }
        }
    }

    if (ship.recent_shot_countdown > 0 && !ship.shot_charged)
    {
        ship.recent_shot_countdown--;
        al_draw_bitmap(img_shot_flash, ship.object.box.end_x + SCREEN_W / 400, ship.object.box.end_y - 45, 0);
    }
}

void ship_fire_shot(void)
{
    Shot *shots;
    shots = shots_get_shots();

    if (/*!ship.shot_in_cooldown*/ 1)
    {
        for (int i = 0; i < NUM_SHOTS; i++)
        {

            if (!shots[i].object.alive)
            {

                // shots[i].object.x = ship.object.box.end_x + SCREEN_W / 200;
                // shots[i].object.y = ship.object.box.end_y + ((ship.object.box.start_y - ship.object.box.end_y) / 2);

                shots[i].object.alive = 1;
                ship.shot_in_cooldown = 1;
                shots[i].charged = ship.shot_charged ? 1 : 0;
                ship.shot_charge = 0;
                ship.shot_charged = 0;
                ship.shot_charging = 0;
                shots[i].object.animation_speed = 3;

                ship.recent_shot_countdown = 5;

                int xVariation = 0;
                if (shots[i].charged)
                {
                    if (coins > 0)
                    {
                        shots[i].object.image = img_shot_special;
                        shots[i].object.total_states = 6;
                        shots[i].object.sprite_length = 166;
                        shots[i].object.has_animation = 1;
                        xVariation = shots[i].object.sprite_length / 6 * 0.5;
                        coins--;
                    }
                    else
                    {
                        shots[i].object.image = img_shot_charged;
                        shots[i].object.sprite_length = 57;
                        shots[i].object.has_animation = 0;
                        shots[i].object.total_states = 1;
                        xVariation = shots[i].object.sprite_length * 0.5;
                    }
                }
                else
                {
                    shots[i].object.image = img_shot_default;
                    shots[i].object.sprite_length = al_get_bitmap_width(shots[i].object.image);
                    shots[i].object.has_animation = 0;
                    shots[i].object.total_states = 1;
                    xVariation = shots[i].object.sprite_length * 0.5;
                }
                shots[i].object.state = 0;

                shots[i].object.x = ship.object.box.end_x + xVariation;
                shots[i].object.y = ship.object.box.end_y - ((ship.object.box.end_y - ship.object.box.start_y) / 3) - al_get_bitmap_height(shots[i].object.image) / 2;

                al_play_sample(shot_sound_default, 0.5f, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

                break;
            }
        }
    }
}

Ship *ship_get_ship(void)
{
    return &ship;
}