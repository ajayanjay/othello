#ifndef replay_h
#define replay_h

#include "datastructures/deque.h"
#include "boolean.h"

boolean overwriteReplayMenu();
boolean saveReplayMenu(Deque * dequeUndo);
void printReplay(const char *filename);
void selectReplays();

#endif