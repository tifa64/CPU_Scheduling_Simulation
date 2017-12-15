#ifndef QUEUE_INCLUDED
#define QUEUE_INCLUDED
#include "process.h"
struct Queue {

    struct Process *arr;
    int fr,rear,sz,capacity;

};
void enqueue (struct Queue *q, struct Process p);
struct Process dequeue (struct Queue *q);
struct Queue* initialize (int cap);
int queueSZ(struct Queue *q);
#endif // QUEUE_INCLUDED
