#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "process.h"

void enqueue (struct Queue *q, struct Process p) {
    if(q->sz == q-> capacity)
        return;
    q->rear = (q->rear+1)%q->capacity;
    q->arr[q->rear] =  p;
    q->sz++;
}
struct Process dequeue (struct Queue *q) {
    struct Process p = q->arr[q->fr];
    q->fr = (q->fr+1)%q->capacity;
    q->sz--;
    return p;
}
struct Process peak (struct Queue *q) {
    struct Process p = q->arr[q->fr];
    return p;
}
 struct Queue* initialize (int cap) {
    struct Queue *q = (struct Queue *) malloc(sizeof (struct Queue));
    q->capacity = cap;
    q->fr = 0;
    q->rear = q->capacity-1;
    q->arr = (struct Process *) malloc(q->capacity * sizeof(struct Process));
    q->sz = 0;
    return q;
}
int queueSZ(struct Queue *q) {
	return q->sz;
}
