#ifndef nbtrees_h
#define nbtrees_h

#include "../boolean.h"
#include "../ai.h"

typedef AIInfo InfoNbTree;

typedef struct nbtree {
    InfoNbTree info;
    struct nbtree *fs, *nb;
} NbTree;

boolean isEmptyTree(NbTree *t);
NbTree* createNodeTree (InfoNbTree info);
void deleteEntireTree (NbTree** root);
void disconnectTreeExcept (NbTree ** root, NbTree * chosen);

int minimax(NbTree *root, int depth, char aiPlayer);

#endif