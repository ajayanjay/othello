#ifndef nbtrees_h
#define nbtrees_h

#include "convention.h"
typedef struct boardarray {
    char board[8][8];
    char currentPlayer;
} BoardArray;
typedef struct nodetree {
    BoardArray board;
    char depth, value;
    boolean isMax, evaluated, expanded;
} NodeInfo;

typedef NodeInfo InfoNbTree;
typedef struct nbtree {
    InfoNbTree info;
    struct nbtree *fs, *nb;
} NbTree;

boolean IsEmptyTree(NbTree *t);
NbTree* CreateNode (InfoNbTree info);
void DeleteEntireTree (NbTree** root);

#endif