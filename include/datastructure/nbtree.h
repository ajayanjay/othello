#ifndef nbtrees_h
#define nbtrees_h

#include "../util/boolean.h"
#include "../ai/ai.h"

typedef AIInfo InfoNbTree;

typedef struct nbtree {
    InfoNbTree info;
    struct nbtree *fs, *nb;
} NbTree;

boolean isEmptyTree(NbTree *t);
NbTree* createNodeTree (InfoNbTree info);
void deleteEntireTree (NbTree** root);
void disconnectTreeExcept (NbTree ** root, NbTree * chosen);

int insertChild(NbTree * parent, NbTree * son);

#endif