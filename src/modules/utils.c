#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int rdm(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}