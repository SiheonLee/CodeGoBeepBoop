/* based on queueTypeAndFunctions.c by Gerard Renardel, 31 January 2018 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "LibQueue.h"

bool contains(Queue q, int page) {
    for (int i = 0; i < q.size; i++) {
        if (q.array[i].page == page) {
            return true;
        }
    }
    return false;
}

bool isFullQueue(Queue q) {
    if(q.back < q.size) {
        if (isEmptyQueue(q)) {
            return false;
        }
        return ((q.back - q.front + q.size) % q.size) == (q.size-1);
    }
    return ((q.back - q.front) % q.size) == (q.size-1);
}

Queue newQueue (int s) {
  Queue q;
  q.array = malloc(s*sizeof(Element));
  for (int i = 0; i < s; i++) {
    q.array[i].page = -1;
    q.array[i].checked = true;
  }
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
  qp->array[qp->back].page = item;
  qp->back = (qp->back + 1) % qp->size;
}

int dequeue (Queue *qp) {
  int item;
  if (isEmptyQueue(*qp)) {
    queueEmptyError();
  }
  item = qp->array[qp->front].page;
  qp->front = (qp->front + 1) % qp->size;
  return item;
}

void freeQueue (Queue q) {
  free(q.array);
}
