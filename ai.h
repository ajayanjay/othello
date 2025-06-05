#ifndef ai_h
#define ai_h

#include "move.h"
#include "boolean.h"

typedef struct aiinfo {
    char board[8][8];
    char currentPlayer;
    Move move;
    boolean isMax;
} AIInfo;

AIInfo createAIInfo(char board[8][8], char currentPlayer, Move move, boolean isMax);

int evaluateBoardArray(char board[8][8], char player);

int evaluateBoardMapArray(char board[8][8], char player);

int evaluateMobility(char board[8][8], char player);
int evaluatePieceDifference(char board[8][8], char player);
int evaluateCorner(char board[8][8], char player);

// Returns the total number of pieces on the board.
int getTotalPieceCountArray(char board[8][8]);

// returns the total number of pieces for a specific player.
int getPieceCountArray(char board[8][8], char player);

// returns all possible moves for a player. use count to get the number of moves.
Move * getValidMovesArray(char board[8][8], char player, int *count);

// checks if a move (Move *move) is possible for a player.
// Returns 1 if the move is possible, 0 otherwise.
int isValidMoveArray(char board[8][8], Move *move, char player);

// Makes a move on the board.
// This function modifies the board directly.
void makeMoveArray(char board[8][8], Move *move, char player);

void copyArray(char dst[8][8], char src[8][8]);

int isGameFinishedArray(char board[8][8]);

#endif