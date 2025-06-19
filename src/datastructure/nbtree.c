#include "../../include/datastructure/nbtree.h"

#include <stdio.h>
#include <stdlib.h>

// Checks if tree is empty
// Author: Ihsan
boolean isEmptyTree(NbTree *root){
    return root == NULL;
}

// Creates new tree node with info and child pointers to null
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

// Recursive deletes entire tree structure and frees all allocated memory
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

// Keeps only chosen subtree and deletes all other branches for tree pruning
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

    // Search for chosen node among siblings
    while (curr != NULL && !isMoveEqual(curr->info.move, chosen->info.move)) {
        prev = curr;
        curr = curr->nb;
    }

    if (curr != chosen) return;  // Chosen node not found

    // Remove chosen node from sibling chain
    if (prev == NULL) {
        parent->fs = (chosen)->nb;
    } else {
        prev->nb = (chosen)->nb;
    }

    (chosen)->nb = NULL;  // Disconnect chosen from siblings

    NbTree *oldRoot = *root;

    *root = chosen;  // Make chosen node the new root

    deleteEntireTree(&oldRoot);  // Delete old tree structure
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