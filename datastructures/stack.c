#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// All functions in this file were authored by Azzar

void initStack(Stack * stack, SizeData elementSize, SizeData capacity) {
    initArray(stack, elementSize, capacity);
}

void freeStack(Stack * stack) {
    freeArray(stack);
}

void push(Stack * stack, void * element) {
    onArraySizeIncrease(stack);
    memcpy((char *)stack->data + stack->size * stack->elementSize, element, stack->elementSize); 
    stack->size++;
}

void pop(Stack * stack, void * element) {
    if (stack->size > 0) {
        stack->size--;
        memcpy(element, (char *)stack->data + stack->size * stack->elementSize, stack->elementSize);
    } else element = NULL;
}

void top(Stack * stack, void * element) {
    if (stack->size > 0) {
        memcpy(element, (char *)stack->data + (stack->size - 1) * stack->elementSize, stack->elementSize);
    } else element = NULL;
}

int isStackEmpty(Stack * stack) {
    return stack->size == 0;
}

void saveStack(Stack * stack, const char* filename) {
    FILE *f;

    if ( (f = fopen(filename, "w")) == NULL)
        return;

    fwrite(&stack->size, sizeof(SizeData), 1, f);
    fwrite(&stack->elementSize, sizeof(SizeData), 1, f);
    fwrite(stack->data, stack->elementSize, stack->size, f);
    fclose(f);
}

Stack loadStack(const char * filename) {
    FILE *f;
    Stack s;

    if ( (f = fopen(filename, "r")) == NULL) {
        s.size = 0;
        s.elementSize = 0;
        s.data = NULL;
        return s;
    }

    fread(&s.size, sizeof(SizeData), 1, f);
    fread(&s.elementSize, sizeof(SizeData), 1, f);
    s.data = malloc(s.size * s.elementSize);
    fread(s.data, s.elementSize, s.size, f);
    fclose(f);
    return s;
}