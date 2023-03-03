#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "LibQueue.h"


/**
 * Checks if the queue contains the page
 * @param q the queue
 * @param page the page to check
 * @return true if the queue contains the page, false otherwise
 */
bool contains(Queue q, int page) {
    for (int i = 0; i < q.size; i++) {
        if (q.array[i] == page) {
            return true;
        }
    }
    return false;
}

/**
 * Checks if the queue is full
 * @param q the queue
 * @return true if the queue is full, false otherwise
 */
bool isFullQueue(Queue q) {
    if(q.back < q.size) {
        if (isEmptyQueue(q)) {
            return false;
        }
        return ((q.back - q.front + q.size) % q.size) == (q.size-1);
    }
    return ((q.back - q.front) % q.size) == (q.size-1);
}

/**
 * Creates a new queue with the given size
 * @param s size of the queue
 * @return the queue
 */
Queue newQueue (int s) {
  Queue q;
  q.array = calloc(s,sizeof(int));
  assert(q.array != NULL);
  q.back = 0;
  q.front = 0;
  q.size = s;
  return q;
}

/**
 * Checks if the queue is empty
 * @param q the queue
 * @return true if the queue is empty, false otherwise
 */
int isEmptyQueue (Queue q) {
  return (q.back == q.front);
}

/**
 * Prints an error message if the queue is empty
 */
void queueEmptyError() {
  printf("queue empty\n");
  abort();
}

/**
 * Doubles the size of the queue
 * @param qp pointer to the queue
 */
void doubleQueueSize (Queue *qp) {
  int i;
  int oldSize = qp->size;
  qp->size = 2 * oldSize;
  qp->array = realloc(qp->array, qp->size * sizeof(int));
  assert(qp->array != NULL);
  for (i=0; i < qp->back ; i++) { /* eliminate the split configuration */
    qp->array[oldSize + i] = qp->array[i];
  }
  qp->back = qp->back + oldSize;  /* update qp->back */
}

/**
 * Adds an item to the queue
 * @param item the item to add
 * @param qp pointer to the queue
 */
void enqueue (int item, Queue *qp) {
  qp->array[qp->back] = item;
  qp->back = (qp->back + 1) % qp->size;
}

/**
 * Removes an item from the queue
 * @param qp pointer to the queue
 * @return the item removed
 */
int dequeue (Queue *qp) {
  int item;
  if (isEmptyQueue(*qp)) {
    queueEmptyError();
  }
  item = qp->array[qp->front];
  qp->front = (qp->front + 1) % qp->size;
  return item;
}

/**
 * Frees the queue
 * @param q the queue
 */
void freeQueue (Queue q) {
  free(q.array);
}
