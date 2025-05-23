#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

boolean IsEmptyTree(NbTree *root){
    return root == NULL;
}

NbTree* CreateNode (InfoNbTree b){
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

void DeleteEntireTree(NbTree** root){
    if (root== NULL || *root == NULL){
        printf ("no tree in DeleteEntireTree\n");
    }
    if ((*root)->fs != NULL){
        NbTree* temp = (*root)->fs;
        (*root)->fs = NULL;
        DeleteEntireTree (&temp);
    }
    if ((*root)->nb != NULL){
        NbTree* temp= (*root)->nb;
        (*root)->nb = NULL;
        DeleteEntireTree (&temp);
    }

    free (*root);
    *root = NULL;
}