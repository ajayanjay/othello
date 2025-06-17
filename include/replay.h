#ifndef replay_h
#define replay_h

#include "datastructure/deque.h"
#include "util/boolean.h"

#define REPLAY_DIR "gamedata/replays"

boolean overwriteReplayMenu();
boolean saveReplayMenu(Deque * dequeUndo);
void printReplay(const char *filename);
void replayMainMenu();
void selectReplays(int action);
void deleteReplay(const char *filename);
boolean confirmDeleteReplay();

#endif