#ifndef replay_h
#define replay_h

#include "boolean.h"

int countFiles (const char *directoryPath);
boolean isFileExist (const char *path, const char *fileName);
boolean menuoverwrite();
boolean menuSave(char *filename);
void createDirectory (const char *path);
void printReplay(const char *fileName);
void selectReplay();

#endif