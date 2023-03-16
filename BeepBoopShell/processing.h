#ifndef PROCESSING_H
#define PROCESSING_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "scanner.h"
#include "bonus.h"
#include "builtin.h"

typedef struct Redirect {
    char *inputFile;
    char *outputFile;
    bool input;
    bool output;
} Redirect;

void printRedirect(Redirect redirect);
void initRedirect(Redirect *redirect);
bool executeCommand(char **execArgs, char *executable, int skipFlag, Redirect redirect, int *exitCode);

#endif