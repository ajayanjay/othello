#include <stdio.h>
#include <stdlib.h>
#include "nbtree.h"

boolean isEmptyTree(NbTree *root){
    return root == NULL;
}

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

void deleteEntireTree(NbTree** root){
    if (root== NULL || *root == NULL){
        printf ("no tree in DeleteEntireTree\n");
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