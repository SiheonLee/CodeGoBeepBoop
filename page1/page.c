#include <stdlib.h>
#include <stdio.h>
#include "LibQueue.h"

int main() {
    int memFrames;
    scanf("%d", &memFrames);
    int pageFaults = 0;
    Queue q = newQueue(memFrames);
//    printf("MOD %d\n", (-1)%4);
    int page;
    while (scanf("%d", &page) != EOF) {
//        printf("front: %d   back: %d   size: %d\n", q.front, q.back, q.size);
        if (contains(q, page)) {
            continue;
        }
        if (isFullQueue(q)) {
//            printf("TESTTSTST\n");
            dequeue(&q);
        }
        // print queue
//        for (int i = q.front; i < q.back; i++) {
//            printf("%d ", q.array[i]);
//        }
//        printf("\n");
        enqueue(page, &q);
        pageFaults++;
    }
    printf("%d\n", pageFaults);
    freeQueue(q);
    return 0;
}