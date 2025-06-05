#ifndef replay_h
#define replay_h

#include "boolean.h"

boolean menuoverwrite();
boolean menuSave(char *filename);
void printReplay(const char *fileName);
void selectReplay();

#endif