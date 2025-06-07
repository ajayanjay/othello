#include "../../include/ai/ai.h"
#include "../../include/attribute/piece.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

void printBoardArray (char board [8][8], char player, Move *selectedMove){
    char buffer[4096];
    int offset = 0;

    offset += sprintf(buffer + offset, "\n  +-----------------+\n");

    int row=0;
    while (row<8){
        offset += sprintf(buffer + offset, "%d |", row+1);

        int col=0;
        while (col<8){
            Move move = {col, row};
            int isSelected = 0;
            if (selectedMove != NULL &&
                selectedMove->x >= 0 && selectedMove->x < 8 &&
                selectedMove->y >= 0 && selectedMove->y < 8 &&
                move.y == selectedMove->y && move.x == selectedMove->x) {
                isSelected = 1;
            }
            if (isSelected) {
                offset += sprintf(buffer + offset, " \033[30;47m%c\033[0m", tolower(player));
            } else if (isValidMoveArray(board, &move, player)){
                offset += sprintf (buffer + offset, " \033[2;2m%c\033[0m", tolower(player)); 
            } else { // regular cell
                if (board[row][col] == BLACK)
                    offset += sprintf (buffer + offset, " \033[31m%c\033[m", board[row][col]);
                else if (board[row][col] == WHITE)
                    offset += sprintf (buffer + offset, " \033[36m%c\033[m", board[row][col]);
                else
                    offset += sprintf (buffer + offset, " %c", board[row][col]);
            }
            col++;
        } 
        offset += sprintf (buffer + offset, " |\n");
        row++;
    }
    offset += sprintf(buffer + offset, "  +-----------------+\n");
    offset += sprintf(buffer + offset, "    A B C D E F G H\n");
    printf ("%s", buffer);
}

AIInfo createAIInfo(char board[8][8], char currentPlayer, Move move) {
    AIInfo newInfo;
    copyBoard(newInfo.board, board);
    newInfo.currentPlayer = currentPlayer;
    newInfo.move = move;
    newInfo.isGameFinished = isGameFinishedArray(board);
    return newInfo;
}

int evaluateBoardArray(char board[8][8], char player) {

    int mobility = evaluateMobility(board, player);
    int pieceDifference = evaluatePieceDifference(board, player);
    int corner = evaluateCorner(board, player);

    // corners are the most valuable.
    // mobility is also valuable, but not as much as corners.
    return 2 * mobility + pieceDifference + 10 * corner;
}

int evaluatePieceDifference(char board[8][8], char player) {
    char opponent = getOppositePiece(player);

    int playerPieceCount = getPieceCountArray(board, player);
    int opponentPieceCount = getPieceCountArray(board, opponent);
    int totalPieces = playerPieceCount + opponentPieceCount;

    if (totalPieces == 0) return 0;
    return (playerPieceCount - opponentPieceCount) * 100 / totalPieces;
}

// mobility is the number of valid moves a player can make.
int evaluateMobility(char board[8][8], char player) {
    char opponent = getOppositePiece(player);

    int mySize = 0, opSize = 0;
    Move *myMoves = getValidMovesArray(board, player, &mySize);
    Move *opMoves = getValidMovesArray(board, opponent, &opSize);

    if (myMoves != NULL) 
        free(myMoves);
    
    if (opMoves != NULL) 
        free(opMoves);
    
    int totalMoves = mySize + opSize;
    if (totalMoves == 0) return 0;
    return (mySize - opSize) * 100 / (totalMoves + 1);
}

int evaluateCorner(char board[8][8], char player) {
    char opponent = getOppositePiece(player);

    int myCorners = 0, opCorners = 0;

    if (board[0][0] == player) myCorners++;
    else if (board[0][0] == opponent) opCorners++;

    if (board[0][7] == player) myCorners++;
    else if (board[0][7] == opponent) opCorners++;

    if (board[7][0] == player) myCorners++;
    else if (board[7][0] == opponent) opCorners++;

    if (board[7][7] == player) myCorners++;
    else if (board[7][7] == opponent) opCorners++;

    int totalCorners = myCorners + opCorners;
    if (totalCorners == 0) return 0;
    return (myCorners - opCorners) * 100 / (totalCorners + 1);
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

    Move *moves = malloc(sizeof(Move) * (*returnSize));

    for (int i = 0; i < *returnSize; ++i)
        moves[i] = buffer[i];

    return moves;
}

// move is VALID.
void makeMoveArray(char board[8][8], Move *move, char currentPlayer) {
    char opponent = getOppositePiece(currentPlayer);

    // set the piece
    board[(int)move->y][(int)move->x] = currentPlayer;

    // flip the opponent's pieces
    static const char directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},     // Up
        { 0, -1},          { 0, 1},     // Left/Right
        { 1, -1}, { 1, 0}, { 1, 1}      // Down
    };

    for (int i = 0; i < 8; ++i) {
        int dx = directions[i][1];
        int dy = directions[i][0];
        int x = (int)move->x + dx;
        int y = (int)move->y + dy;
        boolean foundOpponent = false;

        while (x >= 0 && x < 8 && y >= 0 && y < 8) {
            if (board[y][x] == opponent) {
                foundOpponent = true;
            } else if (board[y][x] == currentPlayer) {
                if (foundOpponent) {
                    // flip the opponent's pieces
                    int flipX = (int)move->x + dx;
                    int flipY = (int)move->y + dy;

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
    char opponent = getOppositePiece(player);

    if (board[(int)move->y][(int)move->x] != EMPTY)
        return 0;

    static const int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},     // Up
        { 0, -1},          { 0, 1},     // Left/Right
        { 1, -1}, { 1, 0}, { 1, 1}      // Down
    };

    for (int i = 0; i < 8; ++i) {
        int dx = directions[i][1];
        int dy = directions[i][0];
        int x = (int)move->x + dx;
        int y = (int)move->y + dy;
        boolean foundOpponent = false;

        while (x >= 0 && x < 8 && y >= 0 && y < 8) {
            // nemu musuh. berarti ada kemungkinan kita bisa nge capture.
            if (board[y][x] == opponent)
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

void copyBoard(char dst[8][8], char src[8][8]) {
    memcpy(dst, src, sizeof(char) * 8 * 8);
}

int isBoardEqual(char board[8][8], char other[8][8]) {
    return memcmp(board, other, sizeof(char) * 8 * 8) == 0;
}

int isGameFinishedArray(char board[8][8]) {
    
    int countBlack = 0, countWhite = 0;
    Move *movesBlack = getValidMovesArray(board, BLACK, &countBlack);
    Move *movesWhite = getValidMovesArray(board, WHITE, &countWhite);

    if ((movesBlack == NULL) && (movesWhite == NULL)) return 1;

    free(movesBlack);
    free(movesWhite);

    return 0;
}