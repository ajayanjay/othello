#ifndef nbtrees_h
#define nbtrees_h

#include "convention.h"
#include "ai.h"

typedef AIInfo InfoNbTree;

typedef struct nbtree {
    InfoNbTree info;
    struct nbtree *fs, *nb;
} NbTree;

boolean IsEmptyTree(NbTree *t);
NbTree* CreateNode (InfoNbTree info);
void DeleteEntireTree (NbTree** root);

#endif