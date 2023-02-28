/**
 * @file ArrayList.c
 * @brief Helper functions for initializing and manipulating an array of lists.
 * @version 0.1
 * @date 2023-02-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "ArrayList.h"

/**
 * @brief Reads a process from stdin and savees it in a list.
 * A process is a list of integers, terminated by -1. 
 * The first element represents the arrival time of the process.
 * 
 * @param n the first element read outside the function.
 * @return The list representing the process.
 */
List readProcess(int *n) {
    List head = NULL;
    List tail = NULL;
    while(*n != -1) {
        List node = newNode(*n);
        if (tail == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        scanf("%d", n);
    }
    // Adding -1 at the end of the list
    List node = newNode(*n);
    if (tail == NULL) {
        head = node;
        tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    return head;
}

/**
 * @brief Reads the input from stdin and saves it in an array of lists.
 * 
 * @param nrP passes by reference the number of lists read.
 * @return the array of lists.
 */
List *readInput(int *nrP) {
    int n;
    int size = 100;
    List *inp = malloc(size*sizeof(List));
    scanf("%d", &n);
    while (n != EOF) {
        inp[*nrP] = readProcess(&n);
        (*nrP)++;
        if(*nrP >= size) {
            size *= 2;
            inp = realloc(inp, size*sizeof(List));
        }
        scanf("%d", &n);
    }
    return inp;
}

/**
 * @brief Copies the array of lists in a new array of lists.
 * 
 * @param lists the original array of lists.
 * @param nrP the number of lists.
 * @return the copy of the state of the array of lists.
 */
List *copyProcesses(List *lists, int nrP) {
    List *copyLists = malloc(nrP*sizeof(List));
    for(int i = 0; i < nrP; i++) {
        copyLists[i] = lists[i];
    }
    return copyLists;
}

/**
 * @brief Prints the array of lists.
 * 
 * @param lists the array of lists.
 * @param nrP the number of lists.
 */
void printOutput(List *lists, int nrP) {
    for (int i = 0; i < nrP; i++) {
        printList(lists[i]);
    }
    printf("---\n");
}

/**
 * @brief Frees the memory allocated for the array of lists.
 * 
 * @param lists the array of lists.
 * @param nrP the number of lists.
 */
void freeProcesses(List *lists, int nrP) {
    for(int i = 0; i < nrP; i++) {
        freeTokenList(lists[i]);
    }
    free(lists);
}