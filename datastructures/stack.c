#include "stack.h"
#include <mem.h>
#include <stdlib.h>

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