#ifndef nbtrees_h
#define nbtrees_h

#include <stdlib.h>
#include <stdbool.h>

typedef struct elmttree *addrtree;
typedef struct elmttree{
    void * data;
    unsigned int datasize;
    addrtree fs, nb; 
} nbtree;

bool IsEmptyTree(addrtree t);
addrtree CreateNode (const void *data, unsigned int dataSize);
void DeleteEntireTree (addrtree *root);


#endif