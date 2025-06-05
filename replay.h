#ifndef replay_h
#define replay_h

#include "datastructures/deque.h"
#include "boolean.h"

boolean menuoverwrite();
boolean menuSave(Deque *dequeundo);
void printReplay(const char *fileName);
void selectReplay();

#endif