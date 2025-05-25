#ifndef ai_h
#define ai_h

#include "move.h"
#include "boolean.h"

// board duplicate for AI.
typedef struct boardarray {
    char board[8][8];
    char currentPlayer;
} BoardArray;

typedef struct aiinfo {
    BoardArray board;
    Move move;
    int score;
    boolean isMax, evaluated, expanded;
} AIInfo;

/*
    BLACK (First move) : 'X'
    WHITE (Second move): 'O'
    BLANK : '.'
*/

int evaluateBoard(char board[8][8]);

// Returns the total number of pieces on the board.
int getTotalPieceCount(char board[8][8]);

// returns the total number of pieces for a specific player.
int getPieceCount(char board[8][8], char player);

// returns all possible moves for a player. use count to get the number of moves.
Move * getValidMoves(char board[8][8], char player, int *count);

// checks if a move (Move *move) is possible for a player.
// Returns 1 if the move is possible, 0 otherwise.
int isValidMove(char board[8][8], Move *move, char player);

// Makes a move on the board.
// This function modifies the board directly.
void makeMove(char board[8][8], Move *move);

#endif