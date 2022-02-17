#ifndef ENTITIES_H
#define ENTITIES_H

#include "game.h"

typedef struct Entity
{
    Movable_Object object;

    int health;

    /* Score */
    int max_score;
    int min_score;
} Entity;

#define NUM_ENTITIES 15
#define NUM_ENEMIES 1

/* Loads all entities */
void entities_load(void);

/* Resets the entities */
void entities_reset(void);

/* Creates an Entity */
int entities_create(Entity entity, int x, int y);

void entities_create_random_enemy(void);

/* Updates all Entities */
void entities_update(void);

Entity *entities_get_entities(void);

#endif