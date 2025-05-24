#include "game.h"
#include <stdlib.h>

int isGameOver(NodeOctuple * board) {
    return (isBoardFull(board) || !hasValidMove(board, 'O') && !hasValidMove(board, 'X'));
}

int hasValidMove(NodeOctuple * board, char player) {
    return (getValidMoves(board, player) != NULL);
}