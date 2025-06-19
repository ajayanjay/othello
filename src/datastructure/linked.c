#include "../../include/datastructure/linked.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// All functions in this file were authored by Ihsan

// Checks if linked list is empty
int isEmpty(address p) {
    return (p == nil);
}

// Creates new node with value and next/prev to null
address createNodeSingle(infotype nilai){
    address P;
    P = (address) malloc(sizeof(ElmtList));
    if (P == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    info(P) = nilai;
    next(P) = nil;
    prev(P) = nil;
    return P;
}

// Inserts new node at the beginning of linked list and updates head pointer
void insertAwal(address *p, address PNew) {
    if (isEmpty(*p)) {
        *p = PNew;
    } else {
        next(PNew) = *p;
        prev(*p) = PNew;
        *p = PNew;
    }
}

// Inserts new node at the end of linked list by traversing to tail
void insertAkhir(address *p, address PNew) {
    if (isEmpty(*p)) {
        *p = PNew;
    } else {
        address temp = *p;
        while (next(temp) != nil) {
            temp = next(temp);
        }
        next(temp) = PNew;
        prev(PNew) = temp;
    }
}

// Inserts new node after specified node
void insertAfter(address *pBef, address PNew) {
    if (*pBef != nil) {
        next(PNew) = next(*pBef);
        prev(PNew) = *pBef;
        if (next(*pBef) != nil) {
            prev(next(*pBef)) = PNew;
        }
        next(*pBef) = PNew;
    }
}

// Removes first node from linked list and get value
void freeAwal(address *p, infotype *X) {
    if (!isEmpty(*p)) {
        address temp = *p;
        *X = info(temp);  // Get value
        *p = next(temp);  // Update head to next node
        if (*p != nil) {
            prev(*p) = nil;  // Break backward link from new head
        }
        free(temp);
    }
}

// Removes last node from linked list and return it value
void freeAkhir(address *p, infotype *X) {
    if (isEmpty(*p)) return;

    if (next(*p) == nil) {
        // Only one node in list
        *X = info(*p);
        free(*p);
        *p = nil;
    } else {
        // Traverse to find last node
        address temp = *p;
        while (next(temp) != nil) {
            temp = next(temp);
        }
        *X = info(temp);
        if (prev(temp) != nil) {
            next(prev(temp)) = nil;
        }
        free(temp);
    }
}

// Deallocates entire linked list
void freeList(address *p) {
    address temp;
    while (*p != nil) {
        temp = *p;
        *p = next(temp);
        free(temp);
    }
}

// Recursive and returns total number of nodes in linked list
int getSize(address p) {
    if (p == nil) return 0;
    return 1 + getSize(next(p));
}