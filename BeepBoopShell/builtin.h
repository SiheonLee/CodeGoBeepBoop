#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "history.h"


bool isBuiltIn(char *s);
int executeBuiltIn(char *builtin, char **args[], int *exitFlag, int exitCode, History hist);

#endif
