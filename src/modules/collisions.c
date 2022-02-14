#include "game.h"

int collisions_check_box(Box box1, Box box2)
{
    if (box1.start_x < box2.end_x && box1.end_x > box2.start_x && box1.start_y < box2.end_y && box1.end_y > box2.start_y)
    {
        return 1;
    }
    return 0;
}

int collisions_check_planet(Planet *planet, Box box)
{
    int planetX = planet->object.x + planet->size / 2;
    int planetY = planet->object.y + planet->size / 2;
    int radius = planet->size / 2;
    float testX = planetX;
    float testY = planetY;

    if (testX < box.start_x)
        testX = box.start_x;
    if (testX > box.end_x)
        testX = box.end_x;
    if (testY < box.start_y)
        testY = box.start_y;
    if (testY > box.end_y)
        testY = box.end_y;

    return ((planetX - testX) * (planetX - testX) + (planetY - testY) * (planetY - testY) < radius * radius * 0.9);
}