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
        return;
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
void disconnectTreeExcept(NbTree **root, NbTree * chosen) {
    if (*root == NULL) return;

    if (chosen == NULL) {
        deleteEntireTree(root);
        return;
    }

    NbTree *parent = *root;
    NbTree *prev = NULL;
    NbTree *curr = parent->fs;

    while (curr != NULL && !isMoveEqual(curr->info.move, chosen->info.move)) {
        prev = curr;
        curr = curr->nb;
    }

    if (curr != chosen) return;

    if (prev == NULL) {
        parent->fs = (chosen)->nb;
    } else {
        prev->nb = (chosen)->nb;
    }

    (chosen)->nb = NULL;

    NbTree *oldRoot = *root;

    *root = chosen;

    deleteEntireTree(&oldRoot);
}


int minimax(NbTree * node, int depth, char aiPlayer, int alpha, int beta) {

    if (depth == 0 || isGameFinishedArray(node->info.board)) {
        return evaluateBoardArray(node->info.board, aiPlayer);
    }

    int bestScore = (node->info.isMax) ? INT_MIN : INT_MAX;
    if (node->fs != NULL) {
        NbTree * child = node->fs;
        while (child != NULL) {
            int score = minimax(child, depth - 1, aiPlayer, alpha, beta);
            if (node->info.isMax) {
                if (score > bestScore) bestScore = score;
                if (score > alpha) alpha = score;
                if (beta <= alpha) break;
            } else {
                if (score < bestScore) bestScore = score;
                if (score < beta) beta = score;
                if (beta <= alpha) break;
            }
            child = child->nb;
        }
        return bestScore;
    }

    int moveCount;
    Move *validMoves = getValidMovesArray(node->info.board, node->info.currentPlayer, &moveCount);

    if (moveCount == 0) {
        char opponent = (node->info.currentPlayer == BLACK) ? WHITE : BLACK;
        NbTree * passNode = createNodeTree(createAIInfo(node->info.board, opponent, (Move){-1, -1}, !node->info.isMax));
        node->fs = passNode;
        int score = minimax(passNode, depth - 1, aiPlayer, alpha, beta);
        return score;
    }

    NbTree * prevChild = NULL;
    for (int i = 0; i < moveCount; ++i) {
        char tempBoard[8][8];
        copyBoard(tempBoard, node->info.board);

        makeMoveArray(tempBoard, &validMoves[i], node->info.currentPlayer);
        char nextPlayer = (node->info.currentPlayer == BLACK) ? WHITE : BLACK;

        NbTree * child = createNodeTree(createAIInfo(tempBoard, nextPlayer, validMoves[i], !node->info.isMax));
        if (prevChild == NULL) {
            node->fs = child;
            prevChild = child;
        } else {
            prevChild->nb = child;
            prevChild = child;
        }

        int score = minimax(child, depth - 1, aiPlayer, alpha, beta);
        if (node->info.isMax) {
            if (score > bestScore) bestScore = score;
            if (score > alpha) alpha = score;
            if (beta <= alpha) break;
        } else {
            if (score < bestScore) bestScore = score;
            if (score < beta) beta = score;
            if (beta <= alpha) break;
        }
    }

    free(validMoves);
    return bestScore;
}