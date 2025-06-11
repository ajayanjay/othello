#include "../../include/datastructure/stack.h"

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

void emptyStack(Stack * stack) {
    stack->size = 0;
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

    if ( (f = fopen(filename, "wb")) == NULL)
        return;

    fwrite(&stack->capacity, sizeof(SizeData), 1, f);
    fwrite(&stack->elementSize, sizeof(SizeData), 1, f);

    fwrite(&stack->size, sizeof(SizeData), 1, f);
    fwrite(stack->data, stack->elementSize, stack->size, f);
    fclose(f);
}

int loadStack(Stack * s, const char* filename) {
    FILE *f;

    if ( (f = fopen(filename, "rb")) == NULL) {
        return 0;
    }

    fread(&s->capacity, sizeof(SizeData), 1, f);
    fread(&s->elementSize, sizeof(SizeData), 1, f);
    initStack(s, s->elementSize, s->capacity);

    fread(&s->size, sizeof(SizeData), 1, f);
    fread(s->data, s->elementSize, s->size, f);
    fclose(f);
    return 1;
}