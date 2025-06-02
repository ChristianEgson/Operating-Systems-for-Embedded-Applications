#include <stdlib.h>
#include <stdio.h>

typedef struct _process
{
    int pid;
    int size;
    char name;
    int priority; //added for MLFQ
    struct _process *next;
} Process;

typedef struct _queue{
    Process *first;
    Process *last;
} Queue;


Queue* createQueue() {
    Queue* newQue = malloc(sizeof(Queue));
    newQue->first = NULL;
    newQue->last = NULL;

    return newQue;
} 

Process* createProcess(int process_id, int process_size, char process_name){
    Process* newProcess = malloc(sizeof(Process));
    newProcess->next = NULL;
    newProcess->pid = process_id;
    newProcess->size = process_size;
    newProcess->name = process_name;
    newProcess->priority = 0; //added for MLFQ, starts each process at q[0]

    return newProcess;
}

int addToQueueLast(Queue* q, Process* p){
    if (q == NULL){
        return -1;
    }
    if (p == NULL){
        return -1;
    }

    p->next = NULL;

    if (q->last == NULL){
        q->last = p;
        q->first = p;
        return 0;
    }

    q->last->next = p;
    q->last = p;

    return 0;
}

Process* getFirstFromQueue(Queue* q){
    if (q == NULL){
        return NULL;
    }
    if (q->first == NULL){
        return NULL;
    }
    Process* top = q->first;

    q->first = q->first->next;

    if (q->first == NULL){
        q->last = NULL;
    }

    top->next = NULL;
    return top;
}

int rr(Queue* hi, int decr, int maxPriority, Queue* queues[]) {
    if (hi == NULL)
        return -1;

    Process* p = getFirstFromQueue(hi);

    while (p != NULL) {
        printf("\n🔄Process %c is running: size = %d, priority = %d\n", p->name, p->size, p->priority);
        
        p->size -= decr;  // Reduce execution time

        Process* next = getFirstFromQueue(hi);  // Save next process before modifying p

        if (p->size > 0) {
            if (p->priority < maxPriority - 1) {  
                p->priority += 1;  // Lower priority
                printf("\n❗️Process %c Has Been Moved to a Lower Priority.\n", p->name);
                addToQueueLast(queues[p->priority], p);  // Move down priority queue
            } else {
                addToQueueLast(queues[p->priority], p);  // Stay in same queue
            }
        } else {
            printf("\n✅Process %c has completed.\n", p->name);
            free(p);  // Process completed
        }
        p = next;  // Move to the next process
    }

    return 0;
}

void moveToHighPriority(Queue* queues[], int numOfQueues){
    printf("\n⭐️All Processes Have Been Raised to the Highest Priority⭐️\n");

    for (int i = 0; i < numOfQueues; i++){
        Process* p = getFirstFromQueue(queues[i]);

        while (p != NULL){
            p->priority = 0; //moves to q0 (highest priority queue)
            addToQueueLast(queues[0], p); //moves the process to end of q0
            p = getFirstFromQueue(queues[i]);  // grabs next process from same queue
        }
    }
}

int MLFQ(Queue* queues[], int slice[], int numOfQueues){
    Process* p;
    int numQueues = 4;
    static int cycleCount = 0;

    //while loop added to ensure program does not end until all process are completed
    while (1) {
        int processesNotDone = 0;

    for (int i = 0; i < numOfQueues; i++){
        if (queues[i]->first != NULL)
            processesNotDone = 1;
    }

    if (processesNotDone == 0) //if all processes are completed, while loop breaks
        break;

    for (int i = 0; i < numQueues; i++){
        printf("\n==========================================================\n");
        printf("\nRound Robin for Queue %d Is In Progress: (Time Slice: %d)\n", i, slice[i]);
        printf("\n==========================================================\n");
        rr(queues[i], slice[i], numQueues, queues);  // pass queues to round robin
    }

    cycleCount++; //counter used for eventual reprioritization of all processes

    if (cycleCount % 3 == 0){
        moveToHighPriority(queues, 4); //every 4 cycles, all processes will move to q0
        }
    }

    return 0;
}


int main(){
    int numOfQueues = 4; 
    int slice_q0 = 4, slice_q1 = 8, slice_q2 = 12, slice_q3 = 16; //time slices / quanta of each queue

    Queue* q[numOfQueues]; //array of queues for MLFQ
    for (int i = 0; i < numOfQueues; i++){
        q[i] = createQueue();
    }

    int timeSlice[4] = {slice_q0, slice_q1, slice_q2, slice_q3};

    //Process* pA = createProcess(1, 5, 'A');
    addToQueueLast(q[0], createProcess(1, 63, 'A'));
    addToQueueLast(q[0], createProcess(2, 16, 'B'));
    addToQueueLast(q[0], createProcess(3, 31, 'C'));
    addToQueueLast(q[0], createProcess(4, 27, 'D'));


    MLFQ(q, timeSlice, 4); //call to MLFQ;

 

    return 0;
}


