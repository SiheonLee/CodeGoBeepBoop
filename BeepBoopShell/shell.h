#ifndef SHELL_SHELL_H
#define SHELL_SHELL_H

#include <stdbool.h>
#include "history.h"

bool parseInputLine(List *lp, int *exitFlag, int *skipFlag, History hist);

#endif
