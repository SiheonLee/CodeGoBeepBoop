#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "LibQueue.h"
#include "ArrayList.h"

#define IS_SLEEPING -1

bool isDoneP(List process) {
    if(process->next->t == -1) {
        return true;
    }
    return false; 
}

double calculateTurnaroundAvrg(int *turnaround, List *processes, int nrP) {
    long totalTurnaround = 0;
    for(int i = 0; i < nrP; i++) {
        turnaround[i] -= processes[i]->t;
    }
    for(int i = 0; i < nrP; i++) {
        totalTurnaround += turnaround[i];
    }
    return (totalTurnaround/nrP);
}

int main(int argc, char *argv[]) {
    int nr_processes = 0;
    List *processes = readInput(&nr_processes);
    List *initialProcesses = copyProcesses(processes, nr_processes);

        // Variable definition
    int currentP = 0;

    int totalTimeUnits = 0;

    int runningP = -1;
    int blockedP = -1;
    Queue readyQ = newQueue(nr_processes);
    Queue blockedQ = newQueue(nr_processes);

    int *turnaround = malloc(nr_processes*sizeof(int));

        // Execution
    enqueue(currentP, &readyQ);
    processes[currentP] = processes[currentP]->next;
    currentP++;

        // MAIN LOOP
    while(!isEmptyQueue(readyQ) || !isEmptyQueue(blockedQ) || (runningP != IS_SLEEPING) || (blockedP != IS_SLEEPING) || currentP != nr_processes) {

        if(runningP == IS_SLEEPING) {
            runningP = safeDequeue(&readyQ); 
        }
        if(blockedP == IS_SLEEPING) {
            blockedP = safeDequeue(&blockedQ); 
        }

        // Update when new process arrived
        if(currentP < nr_processes && totalTimeUnits >= processes[currentP]->t) {
            enqueue(currentP, &readyQ);
            processes[currentP] = processes[currentP]->next;
            currentP++;

        // The whole system is idle
        } else if(blockedP == IS_SLEEPING && runningP == IS_SLEEPING) {
            enqueue(currentP, &readyQ);
            totalTimeUnits += processes[currentP]->t - totalTimeUnits;
            processes[currentP] = processes[currentP]->next;
            currentP++;

        // Nothing is in Blocked executing or in queue
        } else if(blockedP == IS_SLEEPING) {
            // printf("BLOCKED SLEEPING: %d\n", runningP);
            totalTimeUnits += processes[runningP]->t;
            if(!isDoneP(processes[runningP])) {
                processes[runningP] = processes[runningP]->next;
                enqueue(runningP, &blockedQ);
            } else {
                turnaround[runningP] = totalTimeUnits;
            }
            
            runningP = safeDequeue(&readyQ);

        // Nothing is in Running executing or in queue
        } else if(runningP == IS_SLEEPING) {
            // printf("RUNNING SLEEPING: %d\n", blockedP);
            totalTimeUnits += processes[blockedP]->t;

            if(!isDoneP(processes[blockedP])) {
                processes[blockedP] = processes[blockedP]->next;
                enqueue(blockedP, &readyQ);
            } else {
                turnaround[blockedP] = totalTimeUnits;
            }
            
            
            blockedP = safeDequeue(&blockedQ);

        // Process a runner
        } else if(processes[runningP]->t < processes[blockedP]->t) {
            // printf("Running less: %d\n", runningP);
            totalTimeUnits += processes[runningP]->t;
            processes[blockedP]->t -= processes[runningP]->t;

            if(!isDoneP(processes[runningP])) {
                processes[runningP] = processes[runningP]->next;
                enqueue(runningP, &blockedQ);
            } else {
                turnaround[runningP] = totalTimeUnits;
            }
            runningP = safeDequeue(&readyQ);

        // Process a blocked
        } else {
            // printf("Blocking less: %d\n", runningP);
            totalTimeUnits += processes[blockedP]->t;
            processes[runningP]->t -= processes[blockedP]->t;

            if(!isDoneP(processes[blockedP])) {
                processes[blockedP] = processes[blockedP]->next;
                enqueue(blockedP, &readyQ);
            } else {
                turnaround[blockedP] = totalTimeUnits;
            }
            blockedP = safeDequeue(&blockedQ);
        }
    }
    double result = calculateTurnaroundAvrg(turnaround, initialProcesses, nr_processes);
    printf("%.0lf\n", round(result));

    free(turnaround);
    freeQueue(readyQ);
    freeQueue(blockedQ);
    freeProcesses(initialProcesses, nr_processes);
    free(processes);
    return 0;
}
