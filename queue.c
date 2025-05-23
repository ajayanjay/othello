#include "queue.h"

void enqueue(Queue *Q, infotype nilai) {
    address P = SetNode(&nilai);
    if (P == nil) {
        return;
    }
    if (isEmpty(Q->head)) {
        Q->head = P;
        Q->tail = P;
    } else {
        Ins_Akhir(&(Q->tail), P);
        Q->tail = P;
    }
}

void dequeue (Queue *Q, infotype *nilai){
    if (!isEmpty(Q->head)) {
        Del_Awal(&Q->head, nilai);
        if (Q->head == nil) {
            Q->tail = nil;
        }
    }
}

void dequeueandprint(Queue *Q) {
    infotype nilai;
    if (!isEmpty(Q->head)) {
        dequeue(Q, &nilai);
        printf("Lagu yang telah selesai adalah %c\n", nilai);
    } else {
        printf("Queue kosong\n");
    }
}

void createinitQueue(Queue *Q1){
    Q1->head = nil;
    Q1->tail = nil;
}

void ExitQueue (Queue *Q1){
    DeAlokasi(&(Q1->head));
    Q1->tail = nil;
}