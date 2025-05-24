#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

void enqueue(Queue *Q, infotype nilai) {
    address P = SetNode(nilai);
    if (P == nil) {
        return;
    }
    if (isEmpty(Q->head)) {
        Q->head = P;
        Q->tail = P;
    } else {
        insertAkhir(&(Q->tail), P);
        Q->tail = P;
    }
}

void dequeue (Queue *Q, infotype *nilai){
    if (!isEmpty(Q->head)) {
        freeAwal(&Q->head, nilai);
        if (Q->head == nil) {
            Q->tail = nil;
        }
    }
}

void initQueue(Queue *Q1){
    Q1->head = nil;
    Q1->tail = nil;
}

void freeQueue (Queue *Q1){
    freeList(&(Q1->head));
    Q1->tail = nil;
}