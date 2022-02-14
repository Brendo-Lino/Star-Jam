#ifndef STATS_H
#define STATS_H

#include "game.h"

/* Score and coins */
extern int score;
extern int record;
extern int coins;

void stats_load(void);

void stats_update(void);

#endif