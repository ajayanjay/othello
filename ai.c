#include "ai.h"
#include "piece.h"
#include <stdlib.h>

int evaluateBoardArray(char board[8][8], char player) {

    char opPlayer = player == BLACK ? WHITE : BLACK;

    int evalBoardMap = evaluateBoardMapArray(board, player);
    int totalPieceCount = getTotalPieceCountArray(board);
    int playerPieceCount = getPieceCountArray(board, player);
    int opponentPieceCount = getPieceCountArray(board, opPlayer);

    return evalBoardMap + 
           (playerPieceCount - opponentPieceCount) * 10 + 
           (totalPieceCount - 32) * 5; // 32 is the initial number of pieces on the board
}


int evaluateBoardMapArray(char board[8][8], char player) {
    char opPlayer = player == BLACK ? WHITE : BLACK;

    // weight map for the board, corners are the most valuable.
    // the pieces next to corners are the worst.
    int weight[8][8] = {
        { 200, -100, 100,  50,  50, 100, -100,  200},
        {-100, -200, -50, -10, -10, -50, -200, -100},
        { 100, -50,    0,   0,   0,   0, -50,   100},
        {  50, -10,    0,   0,   0,   0, -10,    50},
        {  50, -10,    0,   0,   0,   0, -10,    50},
        { 100, -50,    0,   0,   0,   0, -50,   100},
        {-100, -200, -50, -10, -10, -50, -200, -100},
        { 200, -100, 100,  50,  50, 100, -100,  200}
    };

    if (board[0][0] != EMPTY) {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 4; ++j)
                weight[i][j] = 0; 
    }

    if (board[0][7] != EMPTY) {
        for (int i = 0; i < 3; ++i)
            for (int j = 4; j < 8; ++j)
                weight[i][j] = 0; 
    }

    if (board[7][0] != EMPTY) {
        for (int i = 5; i < 8; ++i)
            for (int j = 0; j < 4; ++j)
                weight[i][j] = 0; 
    }

    if (board[7][7] != EMPTY) {
        for (int i = 5; i < 8; ++i)
            for (int j = 4; j < 8; ++j)
                weight[i][j] = 0; 
    }

    int myScore = 0, opScore = 0;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] == player) 
                myScore += weight[i][j];
            else if (board[i][j] == opPlayer) 
                opScore += weight[i][j];
        }
    }

    return myScore - opScore;
}

int getTotalPieceCountArray(char board[8][8]) {
    int count = 0;
    for (int i = 0; i < 8; ++i) 
        for (int j = 0; j < 8; ++j)
            if (board[i][j] != EMPTY)
                ++count;

    return count;
}

int getPieceCountArray(char board[8][8], char player) {
    int count = 0;
    for (int i = 0; i < 8; i++) 
        for (int j = 0; j < 8; j++) 
            if (board[i][j] == player) 
                count++;
            
    return count;
}

// caller must call free.
Move * getValidMovesArray(char board[8][8], char player, int *returnSize) {
    
    Move buffer[64];
    *returnSize = 0;

    char opPlayer = player == BLACK ? WHITE : BLACK;

    // get all possible moves for the player.
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Move temp = {.x = j, .y = i};
            if (isValidMoveArray(board, &temp, player) == 1) {
                buffer[*returnSize] = temp;
                (*returnSize)++;
            }
        }
    }

    if (*returnSize == 0) return NULL;

    // Remove duplicates
    for (int i = 0; i < *returnSize; ++i) {
        for (int j = i + 1; j < *returnSize; ++j) {
            if (buffer[i].x == buffer[j].x && buffer[i].y == buffer[j].y) {

                // shift elements to the left to remove the duplicate
                for (int k = j; k < *returnSize - 1; ++k)
                    buffer[k] = buffer[k + 1];
                
                (*returnSize)--;
                j--; // since we shifted elements, we need to check the new element at index j again
            }
        }
    }

    Move *moves = malloc(sizeof(Move) * (*returnSize));

    for (int i = 0; i < *returnSize; ++i)
        moves[i] = buffer[i];

    return moves;
}

// move is VALID.
void makeMoveArray(char board[8][8], Move *move, char currentPlayer) {
    char opPlayer = currentPlayer == BLACK ? WHITE : BLACK;

    // set the piece
    board[move->y][move->x] = currentPlayer;

    // flip the opponent's pieces
    static const int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},     // Up
        { 0, -1},          { 0, 1},     // Left/Right
        { 1, -1}, { 1, 0}, { 1, 1}      // Down
    };

    for (int i = 0; i < 8; ++i) {
        int dx = directions[i][0];
        int dy = directions[i][1];
        int x = move->x + dx;
        int y = move->y + dy;
        boolean foundOpponent = false;

        while (x >= 0 && x < 8 && y >= 0 && y < 8) {
            if (board[y][x] == opPlayer) {
                foundOpponent = true;
            } else if (board[y][x] == currentPlayer) {
                if (foundOpponent) {
                    // flip the opponent's pieces
                    int flipX = move->x + dx;
                    int flipY = move->y + dy;

                    // continue flipping until we reach the current player's piece
                    while (flipX != x || flipY != y) {
                        board[flipY][flipX] = currentPlayer;
                        flipX += dx;
                        flipY += dy;
                    }
                }
                break;
            } else
                break;
            

            x += dx;
            y += dy;
        }
    }

}

int isValidMoveArray(char board[8][8], Move *move, char player) {
    char opPlayer = player == BLACK ? WHITE : BLACK;

    if (board[move->y][move->x] != EMPTY)
        return 0;

    static const int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},     // Up
        { 0, -1},          { 0, 1},     // Left/Right
        { 1, -1}, { 1, 0}, { 1, 1}      // Down
    };

    for (int i = 0; i < 8; ++i) {
        int dx = directions[i][0];
        int dy = directions[i][1];
        int x = move->x + dx;
        int y = move->y + dy;
        boolean foundOpponent = false;

        while (x >= 0 && x < 8 && y >= 0 && y < 8) {
            // nemu musuh. berarti ada kemungkinan kita bisa nge capture.
            if (board[y][x] == opPlayer)
                foundOpponent = true;

            // nemu kita
            else if (board[y][x] == player) {

                // kalo sebelumnya udah nemu musuh, berarti kita bisa taro piece di (x,y).
                if (foundOpponent) 
                    return 1;
                
                // kalo belum nemu musuh, berarti kita sama aja nabrak piece kita sendiri.
                else 
                    break;
            } 
            
            // nemu kosong
            else
                break;

            x += dx;
            y += dy;
        }
    }

    return 0;
}

/* bruteforce check
                // upleft check
                if (board->board[y - 1][x - 1] == opPlayer && y > 0 && x > 0) {
                    while (y > 0 && x > 0 && board->board[y - 1][x - 1] == opPlayer) {
                        y--;
                        x--;
                    }

                    if (y > 0 && x > 0 && board->board[y - 1][x - 1] == 0) {
                        buffer[*count].x = x - 1;
                        buffer[*count].y = y - 1;
                        (*count)++;
                    }
                }

                // up check
                y = i;
                x = j;
                if (board->board[y - 1][x] == opPlayer && y > 0) {
                    while (y > 0 && board->board[y - 1][x] == opPlayer) {
                        y--;
                    }

                    if (y > 0 && board->board[y - 1][x] == 0) {
                        buffer[*count].x = x;
                        buffer[*count].y = y - 1;
                        (*count)++;
                    }
                }

                // upright check
                y = i;
                x = j;
                if (board->board[y - 1][x + 1] == opPlayer && y > 0 && x < 7) {
                    while (y > 0 && x < 7 && board->board[y - 1][x + 1] == opPlayer) {
                        y--;
                        x++;
                    }

                    if (y > 0 && x < 7 && board->board[y - 1][x + 1] == 0) {
                        buffer[*count].x = x + 1;
                        buffer[*count].y = y - 1;
                        (*count)++;
                    }
                }

                // right check
                y = i;
                x = j;
                if (board->board[y][x + 1] == opPlayer && x < 7) {
                    while (x < 7 && board->board[y][x + 1] == opPlayer) {
                        x++;
                    }

                    if (x < 7 && board->board[y][x + 1] == 0) {
                        buffer[*count].x = x + 1;
                        buffer[*count].y = y;
                        (*count)++;
                    }
                }

                // downright check
                y = i;
                x = j;
                if (board->board[y + 1][x + 1] == opPlayer && y < 7 && x < 7) {
                    while (y < 7 && x < 7 && board->board[y + 1][x + 1] == opPlayer) {
                        y++;
                        x++;
                    }

                    if (y < 7 && x < 7 && board->board[y + 1][x + 1] == 0) {
                        buffer[*count].x = x + 1;
                        buffer[*count].y = y + 1;
                        (*count)++;
                    }
                }

                // down check
                y = i;
                x = j;
                if (board->board[y + 1][x] == opPlayer && y < 7) {
                    while (y < 7 && board->board[y + 1][x] == opPlayer) {
                        y++;
                    }

                    if (y < 7 && board->board[y + 1][x] == 0) {
                        buffer[*count].x = x;
                        buffer[*count].y = y + 1;
                        (*count)++;
                    }
                }

                // downleft check
                y = i;
                x = j;
                if (board->board[y + 1][x - 1] == opPlayer && y < 7 && x > 0) {
                    while (y < 7 && x > 0 && board->board[y + 1][x - 1] == opPlayer) {
                        y++;
                        x--;
                    }

                    if (y < 7 && x > 0 && board->board[y + 1][x - 1] == 0) {
                        buffer[*count].x = x - 1;
                        buffer[*count].y = y + 1;
                        (*count)++;
                    }
                }

                // left check
                y = i;
                x = j;
                if (board->board[y][x - 1] == opPlayer && x > 0) {
                    while (x > 0 && board->board[y][x - 1] == opPlayer) {
                        x--;
                    }

                    if (x > 0 && board->board[y][x - 1] == 0) {
                        buffer[*count].x = x - 1;
                        buffer[*count].y = y;
                        (*count)++;
                    }
                }
                    */