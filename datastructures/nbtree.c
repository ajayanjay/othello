#include <stdio.h>
#include <stdlib.h>
#include "nbtree.h"
#include "../piece.h"

// Author: Ihsan
boolean isEmptyTree(NbTree *root){
    return root == NULL;
}

// Author: Ihsan
NbTree* createNodeTree (InfoNbTree b){
    NbTree* temp= (NbTree*) malloc (sizeof(NbTree));
    if (temp != NULL){
        temp->info = b;
        temp->fs = NULL;
        temp->nb = NULL;
    } else {
        printf ("failed create node in tree\n");
    }
    return temp;
}

// Author: Ihsan
void deleteEntireTree(NbTree** root){
    if (root== NULL || *root == NULL){
        printf ("no tree in DeleteEntireTree\n");
    }
    if ((*root)->fs != NULL){
        NbTree* temp = (*root)->fs;
        (*root)->fs = NULL;
        deleteEntireTree (&temp);
    }
    if ((*root)->nb != NULL){
        NbTree* temp= (*root)->nb;
        (*root)->nb = NULL;
        deleteEntireTree (&temp);
    }

    free (*root);
    *root = NULL;
}

// Author: Ihsan
void disconnectTreeExcept (NbTree *root, NbTree *chosen){
    if (root==NULL || root->fs == NULL || chosen == NULL) return;

    NbTree *current = root->fs;

    while (current != NULL){
        NbTree *next = current->nb;

        if (current != chosen){
            deleteEntireTree (&current);
        } else {
            root->fs = current;
        }
    current = next;
    }

    if (root->fs == chosen) root->fs->nb = NULL;
}

AIInfo minimax(char board[8][8], char currentPlayer, int depth, boolean isMaximizing, char aiPlayer) {
    AIInfo result;
    result.board.currentPlayer = currentPlayer;
    result.isMax = isMaximizing;
    result.evaluated = false;
    result.expanded = false;
    
    // Copy board state
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            result.board.board[i][j] = board[i][j];
        }
    }
    
    // Base case: depth 0 or game finished
    if (depth == 0 || isGameFinishedArray(board)) {
        result.score = evaluateBoardArray(board, aiPlayer);
        result.evaluated = true;
        result.move = (Move){-1, -1}; // No move at leaf nodes
        return result;
    }
    
    int moveCount;
    Move *validMoves = getValidMovesArray(board, currentPlayer, &moveCount);
    
    // If no valid moves, pass turn to opponent
    if (moveCount == 0 || validMoves == NULL) {
        char opponent = (currentPlayer == BLACK) ? WHITE : BLACK;
        AIInfo passResult = minimax(board, opponent, depth - 1, !isMaximizing, aiPlayer);
        passResult.move = (Move){-1, -1}; // Pass move
        if (validMoves != NULL) free(validMoves);
        return passResult;
    }
    
    result.expanded = true;
    
    // Determine if current player is the AI (maximizing) or opponent (minimizing)
    boolean isAITurn = (currentPlayer == aiPlayer);
    
    if (isAITurn) {
        // AI turn - maximize score
        result.score = -10000; // Negative infinity
        result.move = validMoves[0]; // Default to first move
        
        for (int i = 0; i < moveCount; i++) {
            // Create a copy of the board for simulation
            char tempBoard[8][8];
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    tempBoard[row][col] = board[row][col];
                }
            }
            
            // Make the move on the temporary board
            makeMoveArray(tempBoard, &validMoves[i], currentPlayer);
            
            // Recursive call for opponent
            char opponent = (currentPlayer == BLACK) ? WHITE : BLACK;
            AIInfo childResult = minimax(tempBoard, opponent, depth - 1, false, aiPlayer);
            
            // Update best move if this is better
            if (childResult.score > result.score) {
                result.score = childResult.score;
                result.move = validMoves[i];
            }
        }
    } else {
        // Opponent turn - minimize score
        result.score = 10000; // Positive infinity
        result.move = validMoves[0]; // Default to first move
        
        for (int i = 0; i < moveCount; i++) {
            // Create a copy of the board for simulation
            char tempBoard[8][8];
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    tempBoard[row][col] = board[row][col];
                }
            }
            
            // Make the move on the temporary board
            makeMoveArray(tempBoard, &validMoves[i], currentPlayer);
            
            // Recursive call for AI
            char opponent = (currentPlayer == BLACK) ? WHITE : BLACK;
            AIInfo childResult = minimax(tempBoard, opponent, depth - 1, true, aiPlayer);
            
            // Update best move if this is better (minimizing)
            if (childResult.score < result.score) {
                result.score = childResult.score;
                result.move = validMoves[i];
            }
        }
    }
    
    result.evaluated = true;
    free(validMoves);
    return result;
}