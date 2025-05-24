#ifndef queue_H
#define queue_H

#include "linked.h"

typedef struct {
    address head;
    address tail;
} Deque;

void pushHead(Deque *Q, infotype nilai);
void pushTail(Deque *Q, infotype nilai);

void popHead(Deque *Q, infotype *nilai);
void popTail(Deque *Q, infotype *nilai);

void initDeque(Deque *Q1);

void freeDeque (Deque *Q1);

void saveDeque(Deque * Q, const char * filename);
Deque loadDeque(const char * filename);

int isDequeEmpty(Deque *Q);

#endif