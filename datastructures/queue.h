#ifndef queue_h
#define queue_h

#include "node_sll.h"

typedef struct {
    NodeSingle * front;
    NodeSingle * rear;
    unsigned int size;
} Queue;

#define front(q) q->front
#define rear(q) q->rear

void initQueue(Queue * q, unsigned int size);

void enqueue(Queue * q, const void * data);
void dequeue(Queue * q, void * deleted_data);

void peekFront(Queue * q, void * data);
void peekRear(Queue * q, void * data);

void freeQueue(Queue * q);

int isQueueEmpty(Queue * q);

#endif