#ifndef BACKGROUNDS_H
#define BACKGROUNDS_H

#include "game.h"

typedef struct Background
{
    float x;
    float y;
    float velX;
    int spacing;
    int width;
    int dirX;
    ALLEGRO_BITMAP *image;
} Background;

void backgrounds_load(void);

void backgrounds_reset(void);

void backgrounds_update(void);

void backgrounds_update_background(Background *bg);

#endif