#ifndef QUEUE_INCLUDED
#define QUEUE_INCLUDED

struct Queue {

    int *arr;
    int fr,rear,sz,capacity;

};
void enqueue (struct Queue *q, int element);
int dequeue (struct Queue *q);
struct Queue* initialize (int cap);
#endif // QUEUE_INCLUDED
