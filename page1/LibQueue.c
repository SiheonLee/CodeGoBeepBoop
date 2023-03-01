/* based on queueTypeAndFunctions.c by Gerard Renardel, 31 January 2018 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "LibQueue.h"

bool contains(Queue q, int page) {
    for (int i = 0; i < q.size; i++) {
        if (q.array[i] == page) {
            return true;
        }
    }
    return false;
}

bool isFullQueue(Queue q) {
    if(q.back < q.size) {
        return ((q.back - q.front + q.size) % q.size) == (q.size-1);
    }
    return ((q.back - q.front) % q.size) == (q.size-1);
}

Queue newQueue (int s) {
  Queue q;
  q.array = malloc(s*sizeof(int));
  assert(q.array != NULL);
  q.back = 0;
  q.front = 0;
  q.size = s;
  return q;
}

int isEmptyQueue (Queue q) {
  return (q.back == q.front);
}

void queueEmptyError() {
  printf("queue empty\n");
  abort();
}

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

void enqueue (int item, Queue *qp) {
  qp->array[qp->back] = item;
  qp->back = (qp->back + 1) % qp->size;
  if ( qp->back == qp->front ) {
    doubleQueueSize(qp);
  }
}

int dequeue (Queue *qp) {
//    printf("TEST");
  int item;
  if (isEmptyQueue(*qp)) {
    queueEmptyError();
  }
  item = qp->array[qp->front];
  qp->front = (qp->front + 1) % qp->size;
  return item;
}

void freeQueue (Queue q) {
  free(q.array);
}
