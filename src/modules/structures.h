#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "game.h"

/* Enum for collisions types */
typedef enum
{
    SHIP,
    PLANET,
    ENTITY,
    DROP,
    SHOT,
    NONE
} ObType;

/* Structural Constants */
#define FPS 60;
#define SCREEN_W 1280
#define SCREEN_H 720

typedef struct Box
{
    int start_x;
    int end_x;
    int start_y;
    int end_y;
} Box;

typedef struct Movable_Object
{
    int id;
    int alive;
    Box box;
    int x;
    int y;
    int speedX;
    int speedY;
    int dirX;
    int dirY;

    ALLEGRO_BITMAP *image;
    int has_animation;
    int animation_speed;
    int state;
    int total_states;
    int sprite_length;
    int death_explosion_size;
    int death_animation_countdown;

    ObType type;

} Movable_Object;

#endif