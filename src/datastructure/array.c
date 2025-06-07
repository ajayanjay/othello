#include "../../include/datastructure/array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// All functions in this file were authored by Azzar

void initArray(Array * array, SizeData elementSize, SizeData capacity) {
    array->data = malloc(capacity * elementSize);
    array->elementSize = elementSize;
    array->size = 0;
    array->capacity = capacity;
}

void freeArray(Array * array) {
    free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
}

void onArraySizeIncrease(Array * array) {
    if (array->size >= array->capacity) {
        array->capacity *= 2;
        array->data = realloc(array->data, array->capacity * array->elementSize);
    }
}

int isArrayEmpty(Array * array) {
    return array->size == 0;
}

int getElement(Array * array, int index, void * element) {
    if (index < 0 || index >= array->size) {
        return 0;
    }
    memcpy(element, (char *) array->data + (index * array->elementSize), array->elementSize);
    return 1;
}