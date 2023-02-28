/**
 * @file scheduler.c
 * @brief The main file of the scheduler.
 * The process scheduler is a FIFO scheduler that uses a ready queue and a blocked queue.
 * It is designed after the state machine presented in the lecutre with: BLOCKED, READY and RUNNING.
 * 
 * @version 0.1
 * @date 2023-02-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdlib.h>
#include <stdio.h>

#include "LibQueue.h"
#include "ArrayList.h"

// CPU or IO is sleeping
#define IS_SLEEPING -1

/**
 * @brief Checks if a process has any more CPU or IO time to execute.
 * 
 * @param process the process.
 * @return true if the process is done.
 * @return false if the process is not done.
 */
bool isDoneP(List process) {
    if(process->next->t == -1) {
        return true;
    }
    return false; 
}

/**
 * @brief Calculates the average turnaround time.
 * @attention It also does the substraction of the arrival time.
 * 
 * @param turnaround the turnaround of each process without the arrival time substracted.
 * @param processes the list of all the process in the initial stage.
 *                  All processes must point to their initial element (the arrival time).
 * @param nrP the number of processes.
 * @return the average turnaround time.
 */
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

/**
 * @brief Checks if the execution is completed.
 * 
 * @param readyQ if nothing in ready queue.
 * @param blockedQ if nothing in blocked queue.
 * @param runningP if CPU process is sleeping.
 * @param blockedP if IO process is sleeping.
 * @param currentP if current process is the last one.
 * @param nrP the number of processes.
 * @return true 
 * @return false 
 */
bool isExecutionCompleted(Queue readyQ, Queue blockedQ, int runningP, int blockedP, int currentP, int nrP) {
    return !(!isEmptyQueue(readyQ) || !isEmptyQueue(blockedQ) || (runningP != IS_SLEEPING) || (blockedP != IS_SLEEPING) || currentP != nrP);
}

/**
 * @brief Attempts to dequeue from each queue and put them in the CPU or IO executing process.
 * 
 * @param readyQ the ready queue.
 * @param blockedQ the blocked queue.
 * @param runningP the running CPU process.
 * @param blockedP the running IO process.
 */
void attemptDequeues(Queue *readyQ, Queue *blockedQ, int *runningP, int *blockedP) {
    if(*runningP == IS_SLEEPING) {
        *runningP = safeDequeue(readyQ);
    }
    if(*blockedP == IS_SLEEPING) {
        *blockedP = safeDequeue(blockedQ);
    }
}

/**
 * @brief Add the new process which just arrived to the ready queue.
 * 
 * @param readyQ the ready queue.
 * @param processes the array of all processes from which we take the next one.
 * @param currentP the index of the process which just arrived.
 */
void beginNewProcess(Queue *readyQ, List *processes, int *currentP) {
    enqueue(*currentP, readyQ);
    processes[*currentP] = processes[*currentP]->next;
    (*currentP)++;
}

/**
 * @brief Move a process to its corresponding queue from the CPU or IO.
 * @attention Use this function when one of the CPU or IO is sleeping
 * 
 * @param totalTimeUnits the total time units passed.
 * @param turnaround adds the finish time in case of process execution to the array of turnarounds.
 * @param processes the array of all processes.
 * @param P the current process that finished.
 * @param currentQ the queue from which we put insert in the CPU or IO.
 * @param moveToQ where we move the current process to.
 */
void moveProcessOtherSleeping(int *totalTimeUnits, int *turnaround, List *processes, int *P, Queue *currentQ, Queue *moveToQ) {
    *totalTimeUnits += processes[*P]->t;
    if(!isDoneP(processes[*P])) {
        processes[*P] = processes[*P]->next;
        enqueue(*P, moveToQ);
    } else {
        turnaround[*P] = *totalTimeUnits;
    }
    *P = safeDequeue(currentQ);
}


/**
 * @brief Move a process to its corresponding queue from the CPU or IO.
 * 
 * @param totalTimeUnits the total time units passed.
 * @param turnaround adds the finish time in case of process execution to the array of turnarounds.
 * @param processes the array of all processes.
 * @param currentP the current process that finished.
 * @param otherP the other process that needs time reduction.
 * @param currentQ the queue from which we put insert in the CPU or IO.
 * @param moveToQ where we move the current process to.
 */
void moveProcess(int *totalTimeUnits, int *turnaround, List *processes, int *currentP, int *otherP, Queue *currentQ, Queue *moveToQ) {
    *totalTimeUnits += processes[*currentP]->t;
    processes[*otherP]->t -= processes[*currentP]->t;

    if(!isDoneP(processes[*currentP])) {
        processes[*currentP] = processes[*currentP]->next;
        enqueue(*currentP, moveToQ);
    } else {
        turnaround[*currentP] = *totalTimeUnits;
    }
    *currentP = safeDequeue(currentQ);
}

/**
 * @brief Main function.
 */
int main(int argc, char *argv[]) {
    int nrP = 0;
    List *processes = readInput(&nrP);
    List *initialProcesses = copyProcesses(processes, nrP);

    int totalTimeUnits = 0;
    int currentP = 0, runningP = -1, blockedP = -1;
    Queue readyQ = newQueue(nrP);
    Queue blockedQ = newQueue(nrP);
    int *turnaround = malloc(nrP*sizeof(int));

    // MAIN LOOP
    while(!isExecutionCompleted(readyQ, blockedQ, runningP, blockedP, currentP, nrP)) {

        attemptDequeues(&readyQ, &blockedQ, &runningP, &blockedP);

        // New process arrived
        if(currentP < nrP && totalTimeUnits >= processes[currentP]->t) {
            beginNewProcess(&readyQ, processes, &currentP);

        // STATE: The whole system is empty
        } else if(blockedP == IS_SLEEPING && runningP == IS_SLEEPING) {
            totalTimeUnits += processes[currentP]->t - totalTimeUnits;
            beginNewProcess(&readyQ, processes, &currentP);

        // STATE: IO is idle && Blocked Queue is empty
        } else if(blockedP == IS_SLEEPING) {
            moveProcessOtherSleeping(&totalTimeUnits, turnaround, processes, &runningP, &readyQ, &blockedQ);

        // STATE: CPU is idle && Ready Queue is empty
        } else if(runningP == IS_SLEEPING) {
            moveProcessOtherSleeping(&totalTimeUnits, turnaround, processes, &blockedP, &blockedQ, &readyQ);

        // STATE: CPU and IO are busy -> see which one finishes first
        } else if(processes[runningP]->t < processes[blockedP]->t) {
            moveProcess(&totalTimeUnits, turnaround, processes, &runningP, &blockedP, &readyQ, &blockedQ);
        } else {
            moveProcess(&totalTimeUnits, turnaround, processes, &blockedP, &runningP, &blockedQ, &readyQ);
        }
    }

    printf("%.0lf\n", calculateTurnaroundAvrg(turnaround, initialProcesses, nrP));

    // I WANT TO BREAK FREE !!!
    free(turnaround);
    freeQueue(readyQ);
    freeQueue(blockedQ);
    freeProcesses(initialProcesses, nrP);
    free(processes);
    return 0;
}
