#ifndef storage_h
#define storage_h

#include "boolean.h"

int countFiles (const char *directoryPath);
boolean isFileExist (const char *path, const char *fileName);
void createDirectory (const char *path);

#endif