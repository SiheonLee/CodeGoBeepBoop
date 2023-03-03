#include <stdio.h>
#include "LibQueue.h"

int main() {
    int memFrames;
    scanf("%d", &memFrames);
    int pageFaults = 0;
    Queue q = newQueue(memFrames);
    int page;
    while (scanf("%d", &page) != EOF) {
        // Check if the page is in the queue
        if (contains(q, page)) {
            continue;
        }
        // Make sure queue is full before dequeueing
        if (isFullQueue(q)) {
            dequeue(&q);
        }
        enqueue(page, &q);
        pageFaults++;
    }
    printf("%d\n", pageFaults);
    freeQueue(q);
    return 0;
}