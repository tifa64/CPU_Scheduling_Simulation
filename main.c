#include <stdio.h>
#include <stdlib.h>
#include "queue.h";

int main()
{
    struct Queue *q = initialize(10);
    enqueue(q,1);
    enqueue(q,2);
    printf("%d\n", dequeue(q));
    printf("%d", dequeue(q));
    return 0;
}
