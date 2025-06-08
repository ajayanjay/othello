#ifndef storage_h
#define storage_h

#include "boolean.h"

int countFiles (const char *directoryPath);
boolean isFileExist (const char *path, const char *fileName);
boolean isDirectoryExist (const char *path);
void createDirectory (const char *path);
int removeDirectory (const char *path);

#endif