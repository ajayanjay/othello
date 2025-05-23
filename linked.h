#ifndef linked_H
#define linked_H
#define nil NULL

#define info(P) (P)->info
#define next(P) (P)->next

typedef char infotype;
typedef struct tElmtList *address;
typedef struct tElmtList {
    infotype info;
    address next;
} ElmtList;

int isEmpty(address p);

address createNode(infotype nilai);

void printList(address p);
void insertAwal(address *p, address PNew);
void insertAkhir(address *p, address PNew);
address search(address p, infotype nilai);
void insertAfter(address *pBef, address PNew);
void freeAwal(address *p, infotype *X);
void freeAkhir(address *p, infotype *X);
void freeValue (address *p, infotype target, infotype *X);
void freeList(address *p);
int getSize(address p);

#endif