#include "deque.h"
#include <stdlib.h>
#include <stdio.h>

// All functions in this file were authored by Azzar

void pushTail(Deque *Q, infotype nilai) {
    address P = createNodeSingle(nilai);
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

void pushHead(Deque *Q, infotype nilai) {
    address P = createNodeSingle(nilai);
    if (P == nil) {
        return;
    }
    if (isEmpty(Q->head)) {
        Q->head = P;
        Q->tail = P;
    } else {
        insertAwal(&(Q->head), P);
    }
}

void popHead(Deque *Q, infotype *nilai){
    if (!isEmpty(Q->head)) {
        freeAwal(&Q->head, nilai);
        if (Q->head == nil) {
            Q->tail = nil;
        }
    }
}

void popTail(Deque *Q, infotype *nilai){
    if (!isEmpty(Q->head)) {
        freeAkhir(&Q->tail, nilai);
        ElmtList * current = Q->head;
        if (current == NULL) {
            Q->head = nil;
            Q->tail = nil;
        } else {
            while (current->next != Q->tail) {
                current = current->next;
            }
            Q->tail = current;
            if (Q->tail == nil) {
                Q->head = nil; // If the tail is now nil, head should also be nil
            } else {
                Q->tail->next = nil; // Set the new tail's next to nil
            }
        }
    }
}

void initDeque(Deque *Q1){
    Q1->head = nil;
    Q1->tail = nil;
}

void freeDeque (Deque *Q1){
    freeList(&(Q1->head));
    Q1->tail = nil;
}

void saveDeque(Deque * Q, const char * filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file for saving queue");
        return;
    }
    
    address current = Q->head;
    while (current != nil) {
        fwrite(&(current->info), sizeof(infotype), 1, file);
        current = current->next;
    }
    
    fclose(file);
}

Deque loadDeque(const char * filename) {
    Deque Q;
    initDeque(&Q);
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file for loading queue");
        return Q; // Return empty queue
    }
    
    infotype nilai;
    while (fread(&nilai, sizeof(infotype), 1, file) == 1) {
        pushHead(&Q, nilai);
    }
    
    fclose(file);
    return Q;
}

int isDequeEmpty(Deque *Q) {
    return (Q->head == nil);
}