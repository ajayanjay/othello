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

// Helper function to create AIInfo for a given board state and move
AIInfo createAIInfo(char board[8][8], char currentPlayer, Move move, boolean isMax) {
    AIInfo info;
    info.board.currentPlayer = currentPlayer;
    info.isMax = isMax;
    info.evaluated = false;
    info.expanded = false;
    info.move = move;
    info.score = 0;
    
    // Copy board state
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            info.board.board[i][j] = board[i][j];
        }
    }
    
    return info;
}

// Helper function to build the game tree to specified depth
void buildGameTree(NbTree *node, int depth, char aiPlayer) {
    if (depth <= 0 || isGameFinishedArray(node->info.board.board)) {
        // Leaf node - evaluate the position
        node->info.score = evaluateBoardArray(node->info.board.board, aiPlayer);
        node->info.evaluated = true;
        return;
    }
    
    char currentPlayer = node->info.board.currentPlayer;
    int moveCount;
    Move *validMoves = getValidMovesArray(node->info.board.board, currentPlayer, &moveCount);
    
    if (moveCount == 0 || validMoves == NULL) {
        // No valid moves - pass turn to opponent
        char opponent = (currentPlayer == BLACK) ? WHITE : BLACK;
        AIInfo passInfo = createAIInfo(node->info.board.board, opponent, (Move){-1, -1}, !node->info.isMax);
        
        NbTree *passNode = createNodeTree(passInfo);
        if (passNode != NULL) {
            node->fs = passNode;
            buildGameTree(passNode, depth - 1, aiPlayer);
            node->info.expanded = true;
        }
        
        if (validMoves != NULL) free(validMoves);
        return;
    }
    
    // Create child nodes for each valid move
    NbTree *firstChild = NULL;
    NbTree *prevSibling = NULL;
    
    for (int i = 0; i < moveCount; i++) {
        // Create temporary board with the move applied
        char tempBoard[8][8];
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                tempBoard[row][col] = node->info.board.board[row][col];
            }
        }
        
        // Make the move
        makeMoveArray(tempBoard, &validMoves[i], currentPlayer);
        
        // Determine next player
        char nextPlayer = (currentPlayer == BLACK) ? WHITE : BLACK;
        boolean nextIsMax = (nextPlayer == aiPlayer);
        
        // Create child node
        AIInfo childInfo = createAIInfo(tempBoard, nextPlayer, validMoves[i], nextIsMax);
        NbTree *childNode = createNodeTree(childInfo);
        
        if (childNode != NULL) {
            if (firstChild == NULL) {
                // First child
                firstChild = childNode;
                node->fs = firstChild;
            } else {
                // Link as next sibling
                prevSibling->nb = childNode;
            }
            prevSibling = childNode;
            
            // Recursively build subtree
            buildGameTree(childNode, depth - 1, aiPlayer);
        }
    }
    
    node->info.expanded = true;
    free(validMoves);
}

// Helper function to evaluate the tree using minimax algorithm
int evaluateTreeMinimax(NbTree *node, char aiPlayer) {
    if (node == NULL) {
        return 0;
    }
    
    // If already evaluated (leaf node), return the score
    if (node->info.evaluated && node->fs == NULL) {
        return node->info.score;
    }
    
    // If no children, this shouldn't happen but handle gracefully
    if (node->fs == NULL) {
        node->info.score = evaluateBoardArray(node->info.board.board, aiPlayer);
        node->info.evaluated = true;
        return node->info.score;
    }
    
    char currentPlayer = node->info.board.currentPlayer;
    boolean isMaximizing = (currentPlayer == aiPlayer);
    
    if (isMaximizing) {
        // Maximizing player - choose highest score
        int maxScore = -10000;
        Move bestMove = {-1, -1};
        
        NbTree *child = node->fs;
        while (child != NULL) {
            int childScore = evaluateTreeMinimax(child, aiPlayer);
            if (childScore > maxScore) {
                maxScore = childScore;
                bestMove = child->info.move;
            }
            child = child->nb;
        }
        
        node->info.score = maxScore;
        node->info.move = bestMove;
    } else {
        // Minimizing player - choose lowest score
        int minScore = 10000;
        Move bestMove = {-1, -1};
        
        NbTree *child = node->fs;
        while (child != NULL) {
            int childScore = evaluateTreeMinimax(child, aiPlayer);
            if (childScore < minScore) {
                minScore = childScore;
                bestMove = child->info.move;
            }
            child = child->nb;
        }
        
        node->info.score = minScore;
        node->info.move = bestMove;
    }
    
    node->info.evaluated = true;
    return node->info.score;
}

AIInfo minimax(char board[8][8], char currentPlayer, int depth, boolean isMaximizing, char aiPlayer) {
    // Create root node with current game state
    AIInfo rootInfo = createAIInfo(board, currentPlayer, (Move){-1, -1}, isMaximizing);
    NbTree *root = createNodeTree(rootInfo);
    
    AIInfo result = rootInfo; // Default result
    
    if (root == NULL) {
        // Failed to create root node
        result.score = evaluateBoardArray(board, aiPlayer);
        result.evaluated = true;
        return result;
    }

    buildGameTree(root, depth, aiPlayer);
    evaluateTreeMinimax(root, aiPlayer);
    result = root->info;
    deleteEntireTree(&root);
    
    return result;
}