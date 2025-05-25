#include "game.h"
#include "piece.h"
#include <stdlib.h>

int isGameOver(NodeOctuple * board) {
    return (isBoardFull(board) || (!hasValidMove(board, BLACK) && !hasValidMove(board, WHITE)));
}

int hasValidMove(NodeOctuple * board, char player) {
    return (getValidMoves(board, player) != NULL);
}