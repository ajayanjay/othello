#ifndef stack_h
#define stack_h

#include "array.h"

typedef Array Stack; // stack is a type of array

void initStack(Stack * stack, SizeData elementSize, SizeData capacity);
void freeStack(Stack * stack);

void push(Stack * stack, void * element);
void pop(Stack * stack, void * element);
void top(Stack * stack, void * element);

int isStackEmpty(Stack * stack);

void saveStack(Stack * stack, const char* filename);
Stack loadStack(const char* filename);
 
#endif