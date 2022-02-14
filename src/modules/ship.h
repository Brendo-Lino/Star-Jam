#ifndef SHIP_H
#define SHIP_H

#include "game.h"

typedef struct Ship
{
    Movable_Object object;

    int health;

    int shot_charge;
    int shot_charging;
    int shot_charged;

    // Stats
    int shot_in_cooldown;
    int recent_shot_countdown;
} Ship;

#define SHIP_SPEED 3

/* Fires a shot from the ship */
void ship_fire_shot(void);

/*  Updates the ship */
void ship_update(void);

/* Loads the Ship */
void ship_load(void);

/* Resets the Ship */
void ship_reset(void);

Ship *ship_get_ship(void);

#endif