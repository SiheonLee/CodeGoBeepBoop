#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scanner.h"

#define HIST_SIZE 20

typedef struct History {
    char *arr[HIST_SIZE];
    int index;
    int top;
} History;

void addInputToHistory(History *hist, char *inputLine);

bool printHistory(History hist, char **args[]);


#endif
