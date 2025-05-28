#include "linked.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isEmpty(address p) {
    return (p == nil);
}

address createNodeSingle(infotype nilai){
    address P;
    P = (address) malloc(sizeof(ElmtList));
    if (P == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    info(P) = nilai;
    next(P) = nil;
    return P;
}

void printList(address p) {
    if (isEmpty(p)) {
        printf("Empty List\n");
        return;
    }
    address temp = p;
    while (temp != nil) {
        printf("%c", info(temp));
        if (next(temp) != nil) {
            printf(" -> ");
        }
        temp = next(temp);
    }
    printf("\n");
}

void insertAwal(address *p, address PNew) {
    if (isEmpty(*p)) {
        *p = PNew;
    } else {
        next(PNew) = *p;
        *p = PNew;
    }
}

void insertAkhir(address *p, address PNew) {
    if (isEmpty(*p)) {
        *p = PNew;
    } else {
        address temp = *p;
        while (next(temp) != nil) {
            temp = next(temp);
        }
        next(temp) = PNew;
    }
}

void insertAfter(address *pBef, address PNew) {
    if (*pBef != nil) {
        next(PNew) = next(*pBef);
        next(*pBef) = PNew;
    }
}

void freeAwal(address *p, infotype *X) {
    if (!isEmpty(*p)) {
        address temp = *p;
        *X = info(temp);
        *p = next(temp);
        free(temp);
    }
}

void freeAkhir(address *p, infotype *X) {
    if (isEmpty(*p)) return;

    if (next(*p) == nil) {
        *X = info(*p);
        free(*p);
        *p = nil;
    } else {
        address temp = *p;
        while (next(next(temp)) != nil) {
            temp = next(temp);
        }
        *X = info(next(temp));
        free(next(temp));
        next(temp) = nil;
    }
}

void freeList(address *p) {
    address temp;
    while (*p != nil) {
        temp = *p;
        *p = next(temp);
        free(temp);
    }
}

int getSize(address p) {
    if (p == nil) return 0;
    return 1 + getSize(next(p));
}