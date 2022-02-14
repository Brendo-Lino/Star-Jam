#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "game.h"

/* Checks a collision with a planet */
int collisions_check_planet(Planet *planet, Box box);

/* Checks a collision by Box */
int collisions_check_box(Box box1, Box box2);

#endif