#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void enqueue (struct Queue *q, int element) {
    if(q->sz == q-> capacity)
        return;
    q->rear = (q->rear+1)%q->capacity;
    q->arr[q->rear] =  element;
    q->sz++;
}
int dequeue (struct Queue *q) {
    if (q->sz == 0 )
     return -1;
    int element = q->arr[q->fr];
    q->fr = (q->fr+1)%q->capacity;
    q->sz--;
    return element;
}
 struct Queue* initialize (int cap) {
    struct Queue *q = (struct Queue *) malloc(sizeof (struct Queue));
    q->capacity = cap;
    q->fr = 0;
    q->rear = q->capacity-1;
    q->arr = (int *) malloc(q->capacity * sizeof(int));
    q->sz = 0;
    return q;
}
