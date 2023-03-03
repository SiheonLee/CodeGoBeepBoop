#ifndef CLOCK_H
#define CLOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "assert.h"

typedef struct element {
    int page;
    bool checked;
} Element;

typedef struct Clock {
    Element *array;
    int hand;
    int size;
} Clock;

Clock newClock (int s);

bool checkClock (Clock *clock, int page);

void addToClock (Clock *clock, int page);

void freeClock(Clock clock);

#endif
