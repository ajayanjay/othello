#ifndef ai_h
#define ai_h

#include "move.h"
#include "convention.h"

// board duplicate for AI.
typedef struct boardarray {
    char board[8][8];
    char currentPlayer;
} BoardArray;

typedef struct aiinfo {
    BoardArray board;
    Move move;
    char depth, value;
    boolean isMax, evaluated, expanded;
} AIInfo;

int evaluateBoard(BoardArray *board);

#endif