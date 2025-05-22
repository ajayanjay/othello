#include <stdio.h>
#include "tree.h"

bool IsEmptyTree(addrtree root){
    if (root==NULL) return;
}

addrtree CreateNode (const void *data, unsigned int dataSize){
    addrtree temp= (addrtree) malloc (sizeof(nbtree));
    if (temp != NULL){
        temp->data = malloc (dataSize);
        if (temp->data != NULL){
            memcpy(temp->Board, b, sizeof(board));
            temp->datasize = dataSize;
        } else {
            free (temp);
            printf ("allocate failed\n");
            return NULL;
        }
        temp->fs = NULL;
        temp->nb = NULL;
    } else {
        printf ("failed create node in tree\n");
    }
    return temp;
}

void DeleteEntireTree (addrtree *root){
    if (root== NULL || *root == NULL){
        printf ("no tree in DeleteEntireTree\n");
    }
    if ((*root)->fs != NULL){
        addrtree temp = (*root)->fs;
        (*root)->fs = NULL;
        DeleteEntireTree (&temp);
    }
    if ((*root)->nb != NULL){
        addrtree temp= (*root)->nb;
        (*root)->nb = NULL;
        DeleteEntireTree (&temp);
    }

    free (*root);
    *root = NULL;
}