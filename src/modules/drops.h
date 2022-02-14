#ifndef DROPS_H
#define DROPS_H

#include "game.h"

#define NUM_DROPS 10

typedef enum
{
    HEALTH,
    COIN,
} DropType;

typedef struct Drop
{
    Movable_Object object;
    void (*effect)(void);
} Drop;

int drops_create(DropType type, int x, int y);

void drops_update(void);

void drops_health_effect(void);

void drops_coin_effect(void);

void drops_reset(void);

void drops_load(void);

#endif