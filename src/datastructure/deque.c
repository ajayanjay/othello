#include "../../include/datastructure/deque.h"
#include <stdlib.h>
#include <stdio.h>

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
        if (Q->head == nil) {
            Q->tail = nil;
        } else {
            // Update tail pointer to previous node
            ElmtList *current = Q->head;
            while (current->next != nil) {
                current = current->next;
            }
            Q->tail = current;
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
    FILE *file = fopen(filename, "wb");
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

int loadDeque(Deque * Q, const char * filename) {
    initDeque(Q);
    
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open file for loading queue");
        return 0;
    }
    
    infotype nilai;
    while (fread(&nilai, sizeof(infotype), 1, file) == 1) {
        pushTail(Q, nilai);
    }
    
    fclose(file);
    return 1;
}

int isDequeEmpty(Deque *Q) {
    return (Q == NULL || Q->head == nil);
}