#include "../../include/datastructure/linked.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// All functions in this file were authored by Ihsan

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
    prev(P) = nil;
    return P;
}

void insertAwal(address *p, address PNew) {
    if (isEmpty(*p)) {
        *p = PNew;
    } else {
        next(PNew) = *p;
        prev(*p) = PNew;
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
        prev(PNew) = temp;
    }
}

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

void freeAwal(address *p, infotype *X) {
    if (!isEmpty(*p)) {
        address temp = *p;
        *X = info(temp);
        *p = next(temp);
        if (*p != nil) {
            prev(*p) = nil;
        }
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