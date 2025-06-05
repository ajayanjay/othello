#ifndef array_h
#define array_h

// dynamically allocated array class.

typedef unsigned int SizeData; // type for the size of the array

typedef struct {
    void * data; // pointer to the data
    SizeData elementSize; // sizeof (element)
    SizeData size; // number of elements in the array
    SizeData capacity; // number of elements allocated in the array
} Array;

void initArray(Array * array, SizeData elementSize, SizeData capacity);
void freeArray(Array * array);

// check whether size is less than capacity
// if not, increase the capacity by 2 * elementSize
void onArraySizeIncrease(Array * array);

int isArrayEmpty(Array * array);

int getElement(Array * array, int index, void * element);

#endif