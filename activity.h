#ifndef activity_h
#define activity_h

#include "move.h"

typedef struct {
    char board[8][8];
    Move lastMove;
    char currentPlayer;
} Activity;

#endif