#include <stdio.h>
#include <stdlib.h>
#include "nbtree.h"

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

// Author: Ihsan
void disconnectTreeExcept (NbTree *root, NbTree *chosen){
    if (root==NULL || root->fs == NULL || chosen == NULL) return;

    NbTree *current = root->fs;

    while (current != NULL){
        NbTree *next = current->nb;

        if (current != chosen){
            deleteEntireTree (&current);
        } else {
            root->fs = current;
        }
    current = next;
    }

    if (root->fs == chosen) root->fs->nb = NULL;
}