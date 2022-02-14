#ifndef SHOTS_H
#define SHOTS_H

#include "game.h"

typedef struct Shot
{
    Movable_Object object;
    int charged;
    int animation_countdown;
} Shot;

#define NUM_SHOTS 1
#define SHOT_SPEED 8
#define SHOT_COOL_DOWN 100

/* Resets the shots */
void shots_reset(void);

void shots_load(void);

/* Updates Shots */
void shots_update(void);

Shot *shots_get_shots(void);

#endif