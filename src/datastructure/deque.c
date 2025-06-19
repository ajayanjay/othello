#include "../../include/datastructure/deque.h"

#include <stdlib.h>
#include <stdio.h>

// Add new element to the tail of deque
void pushTail(Deque *Q, infotype nilai) {
    address P = createNodeSingle(nilai);
    if (P == nil) {
        return;
    }
    if (isEmpty(Q->head)) {
        // If deque empty, set head and tail to new node
        Q->head = P;
        Q->tail = P;
    } else {
        Q->tail->next = P;
        P->prev = Q->tail;
        Q->tail = P;  // Update tail to point to new node
        
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

// Delete and get element from the head of deque
void popHead(Deque *Q, infotype *nilai){
    if (isEmpty(Q->head)) 
        return;

    freeAwal(&Q->head, nilai);
    if (Q->head == nil) {
        Q->tail = nil;  // If deque empty, reset tail
    }

}

void popTail(Deque *Q, infotype *nilai){
    if (isEmpty(Q->head)) 
        return;

    address last = Q->tail;
    *nilai = last->info;
    if (last->prev != nil) {
        // Update tail to prev node and break connection
        Q->tail = last->prev;
        Q->tail->next = nil;
    } else {
        // If only one node, reset pointers
        Q->head = nil;
        Q->tail = nil;
    }
}

// Initializes deque (head and tail is null)
void initDeque(Deque *Q1){
    Q1->head = nil;
    Q1->tail = nil;
}

// Frees all list deque and reset pointer
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

// Checks if deque is empty (head pointer is null)
int isDequeEmpty(Deque *Q) {
    return (Q == NULL || Q->head == nil);
}