#ifndef LIBQUEUE_H
#define LIBQUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

typedef struct Queue {
  int *array;
  int back;
  int front;
  int size;
} Queue;

Queue newQueue(int s);
bool contains(Queue q, int page);
bool isFullQueue(Queue q);
int isEmptyQueue(Queue q);
void queueEmptyError();
void doubleQueueSize(Queue *qp);
void enqueue(int item, Queue *qp);
int dequeue(Queue *qp);
void freeQueue(Queue q);

#endif
