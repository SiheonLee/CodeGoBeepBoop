#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include "MyList.h" // File adapted from the scanner of the previous assignment
#include <stdlib.h>
#include <stdio.h>

List readProcess(int *n);
List *readInput(int *nr_processes);
List *copyProcesses(List *processes, int nrP);
void printOutput(List *lists, int nr_processes);
void freeProcesses(List *processes, int nr_processes);

#endif