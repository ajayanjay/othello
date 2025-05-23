#ifndef queue_H
#define queue_H

#include "linked.h"

typedef struct {
    address head;
    address tail;
} Queue;

void enqueue (Queue *Q, infotype nilai);

void dequeue (Queue *Q, infotype *nilai);

void initQueue(Queue *Q1);

void freeQueue (Queue *Q1);

#endif