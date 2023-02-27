#ifndef MYLIST_H
#define MYLIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct ListNode *List;

typedef struct ListNode {
    int t;
    List next;
} ListNode;

bool isEmpty(List l);

void printList(List l);

void freeTokenList(List l);

List newNode(int n);

#endif