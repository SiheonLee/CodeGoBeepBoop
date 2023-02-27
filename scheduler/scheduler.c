#include <stdlib.h>
#include <stdio.h>
#include "LibQueue.h"

int main(int argc, char *argv[]) {
    // read input
    int n;
    Queue *inp[100];
    int cpuTime = 0;
    int ioTime = 0;
    scanf("%d", &n);
    int cnt = 0;
    while (n != EOF) {
        Queue q = newQueue(100);
        inp[cnt] = &q;
        cnt++;
        while (n != -1) {
            // do something
            enqueue(n, &q);
            scanf("%d", &n);
        }
        scanf("%d", &n);
    }
    printf("%d ", inp[0]->array[0]);
    printf("%d ", inp[1]->array[0]);
    // print output
//    for (int i = 0; i < cnt; i++) {
//        while (!isEmptyQueue(*inp[i])) {
//            printf("%d ", dequeue(inp[i]));
//        }
//    }
    return 0;
}
