#ifndef replay_h
#define replay_h

#include "boolean.h"

int countFiles (const char *directoryPath);
boolean menuSave(char *filename);
void createDirectory (const char *path);
void printReplay(const char *fileName);
void selectReplay();

#endif