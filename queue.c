#include "queue.h"
#include <stdlib.h>
#include <string.h>

void initQueue(Queue * q, unsigned int size)
{
    front(q) = NULL;
    rear(q) = NULL;
    size(q) = size;
}

void enqueue(Queue * q, const void * data)
{
    if (rear(q) == NULL)
    {
        insertLast(&rear(q), data, size(q));
        front(q) = rear(q);    
    }
    else
    {
        insertLast(&rear(q), data, size(q));
        rear(q) = next(rear(q));
    }
}

void dequeue(Queue * q, void * deleted_data)
{
    deleteFirst(&front(q), deleted_data, size(q));
}

void peekFront(Queue * q, void * data)
{
    memcpy(data, item(front(q)), size(q));
}
void peekRear(Queue * q, void * data)
{
    memcpy(data, item(rear(q)), size(q));
}

void freeQueue(Queue * q)
{
    freeSingleList(&front(q), size(q));
}

int isQueueEmpty(Queue * q)
{
    return front(q) == NULL;
}