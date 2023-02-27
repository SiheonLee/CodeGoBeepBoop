#include <stdlib.h>
#include <stdio.h>
#include "MyList.h"
#include "LibQueue.h"

List readProcess(int *n) {
    List head = NULL;
    while(*n != -1) {
        List node1 = (List) malloc(sizeof(struct ListNode));
        node1->t = *n;
        node1->next = head;
        head=node1;
        scanf("%d", n);
    }
    return head;
}

List *readInput(int *nr_processes) {
    int n;
    int size = 1;
    List *inp = malloc(size*sizeof(List));
    scanf("%d", &n);
    while (n != EOF) {
        inp[*nr_processes] = readProcess(&n);
        (*nr_processes)++;
        if(*nr_processes >= size) {
            size *= 2;
            inp = realloc(inp, size*sizeof(List));
        }
        scanf("%d", &n);
    }
    return inp;
}

void printOutput(List *lists, int nr_processes) {
    for (int i = 0; i < nr_processes; i++) {
        printList(lists[i]);
    }
}

void freeProcesses(List *processes, int nr_processes) {
    for(int i = 0; i < nr_processes; i++) {
        freeTokenList(processes[i]);
    }
    free(processes);
}

int main(int argc, char *argv[]) {
    int nr_processes = 0;
    List *processes = readInput(&nr_processes);
    printOutput(processes, nr_processes);

        // Variable definition
    int current_process = 0;

    int total_running = 0;
    int total_blocked = 0;

    int current_running = -1;
    int current_blocked = -1;
    Queue ready = newQueue(nr_processes);
    Queue blocked = newQueue(nr_processes);

        // Execution
    enqueue(current_process, &ready);
    processes[current_process] = processes[current_process]->next;
    current_process++;
    while(!isEmptyQueue(ready) || !isEmptyQueue(blocked) || (current_running != -1) || (current_blocked != -1)) {
        if(current_running == -1) {
            current_running = dequeue(&ready); 
        }

        if(total_running >= processes[current_process]->t) {
            enqueue(current_process, &ready);
            processes[current_process] = processes[current_process]->next;
            current_process++;
        }

            // Time pass
        if(processes[current_running]->t < processes[current_blocked]->t) {
            total_running += processes[current_running]->t;
            processes[current_blocked]->t -= processes[current_running]->t;
            total_blocked += processes[current_running]->t;
            processes[current_running] = processes[current_running]->next;
            enqueue(current_running, &blocked);
            current_running = dequeue(&ready);
        } else {
            total_blocked += processes[current_blocked]->t;
            processes[current_running] -= processes[current_blocked]->t;
            total_running += processes[current_blocked]->t;
            processes[current_blocked] = processes[current_blocked]->next;
            enqueue(current_blocked, &ready);
            current_blocked = dequeue(&blocked);
        }
    }

    printf("Total: %d\n", total_running);


    freeQueue(ready);
    freeQueue(blocked);
    freeProcesses(processes, nr_processes);
    return 0;
}
