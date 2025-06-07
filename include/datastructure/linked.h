#ifndef linked_H
#define linked_H

#include "../attribute/activity.h"

#define nil NULL

#define info(P) (P)->info
#define next(P) (P)->next

typedef Activity infotype;
typedef struct tElmtList *address;
typedef struct tElmtList {
    infotype info;
    address next;
} ElmtList;

int isEmpty(address p);

address createNodeSingle(infotype nilai);

void insertAwal(address *p, address PNew);
void insertAkhir(address *p, address PNew);
void insertAfter(address *pBef, address PNew);
void freeAwal(address *p, infotype *X);
void freeAkhir(address *p, infotype *X);
void freeList(address *p);
int getSize(address p);

#endif