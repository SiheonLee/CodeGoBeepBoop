#include <stdlib.h>
#include <stdio.h>
#include "LibQueue.h"

int main() {
    int memFrames;
    scanf("%d", &memFrames);
    int pageFaults = 0;
    Queue q = newQueue(memFrames);
    int page;
    while (scanf("%d", &page) != EOF) {
        if (contains(q, page)) {
            continue;
        }
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