#ifndef SCANNER_H
#define SCANNER_H

#define INITIAL_STRING_SIZE 10

#include "bonus.h"

typedef struct ListNode *List;

typedef struct ListNode {
    char *t;
    List next;
} ListNode;

int getch();

char *readInputLine(int *exitFlag, int *printFlag, char *history[], int histIndex, int histSize, int histTop);

char *upArrowPressed(char *history[], int *printFlag, int histIndex, int histSize, int histTop, char* s);

char *downArrowPressed(char *history[], int *printFlag, int histIndex, int histSize, int histTop, char* s);

List getTokenList(char *s);

bool isEmpty(List l);

void printList(List l);

void freeTokenList(List l);

#endif
