#include <stdio.h>
#include <stdlib.h>
#include "nbtree.h"
#include "../piece.h"

// Author: Ihsan
boolean isEmptyTree(NbTree *root){
    return root == NULL;
}

// Author: Ihsan
NbTree* createNodeTree (InfoNbTree b){
    NbTree* temp= (NbTree*) malloc (sizeof(NbTree));
    if (temp != NULL){
        temp->info = b;
        temp->fs = NULL;
        temp->nb = NULL;
    } else {
        printf ("failed create node in tree\n");
    }
    return temp;
}

// Author: Ihsan
void deleteEntireTree(NbTree** root){
    if (root== NULL || *root == NULL){
        return;
    }
    if ((*root)->fs != NULL){
        NbTree* temp = (*root)->fs;
        (*root)->fs = NULL;
        deleteEntireTree (&temp);
    }
    if ((*root)->nb != NULL){
        NbTree* temp= (*root)->nb;
        (*root)->nb = NULL;
        deleteEntireTree (&temp);
    }

    free (*root);
    *root = NULL;
}

// Author: Ihsan
void disconnectTreeExcept(NbTree **root, NbTree * chosen) {
    if (*root == NULL) return;

    if (chosen == NULL) {
        deleteEntireTree(root);
        return;
    }

    NbTree *parent = *root;
    NbTree *prev = NULL;
    NbTree *curr = parent->fs;

    while (curr != NULL && !isMoveEqual(curr->info.move, chosen->info.move)) {
        prev = curr;
        curr = curr->nb;
    }

    if (curr != chosen) return;

    if (prev == NULL) {
        parent->fs = (chosen)->nb;
    } else {
        prev->nb = (chosen)->nb;
    }

    (chosen)->nb = NULL;

    NbTree *oldRoot = *root;

    *root = chosen;

    deleteEntireTree(&oldRoot);
}

int insertChild(NbTree *parent, NbTree * son) {
    if (parent == NULL) return 0;

    NbTree *current = parent->fs;
    if (current == NULL) {
        parent->fs = son;
    } else {
        son->nb = current;
        parent->fs = son;
    }
    
    return 1;
}