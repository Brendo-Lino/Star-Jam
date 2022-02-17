#ifndef PLANETS_H
#define PLANETS_H

#include "game.h"

typedef struct Planet
{
    Movable_Object object;
    int size;
} Planet;

#define NUM_PLANETS 30
#define NUM_IMAGES 39

/* Loads the planets array */
void planets_load(void);

/* Resets the planets */
void planets_reset(void);

/* Creates a random planet */
void planets_create_random_planet(int x, int y, int speed);

/* Updates Planets */
void planets_update(void);

/* Gets the Planets */
Planet *planets_get_planets(void);

#endif