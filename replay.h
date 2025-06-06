#ifndef replay_h
#define replay_h

#include "datastructures/deque.h"
#include "boolean.h"

#define REPLAY_DIR "gamedata/replays"

boolean overwriteReplayMenu();
boolean saveReplayMenu(Deque * dequeUndo);
void printReplay(const char *filename);
void selectReplays();

#endif