#include "ai.h"

int evaluateBoard(BoardArray *board) {
    int score = 0;
    // Example evaluation logic: count pieces for each player
    int playerOCount = 0, playerXCount = 0;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board->board[i][j] == 'O') {
                playerOCount++;
            } else if (board->board[i][j] == 'X') {
                playerXCount++;
            }
        }
    }

    // Simple scoring: difference in counts
    score = playerOCount - playerXCount;

    return score;
}