#include "game.h"
#include "piece.h"
#include "menu.h"
#include "boolean.h"
#include "datastructures/octuple.h"
#include <stdlib.h>
#include <stdio.h>

int isGameOver(NodeOctuple * board) {
    return !hasValidMove(board, BLACK) && !hasValidMove(board, WHITE);
}

int hasValidMove(NodeOctuple * board, char player) {
    Move buffer[64]; int bufferSize = 0;
    getValidMoves(board, player, buffer, &bufferSize);
    return bufferSize > 0;
}

int undo(NodeOctuple * board, Deque * queue_undo, Stack * stack_undo, char * currentPlayer) {
    
    if (isDequeEmpty(queue_undo)) return 0;
    
    Activity lastActivity;
    popHead(queue_undo, &lastActivity);

    convertArrayToOctuple(board, lastActivity.board);
    *currentPlayer = lastActivity.currentPlayer;

    push(stack_undo, &(lastActivity.lastMove));
    return 1;
}

int redo(NodeOctuple * board, Deque * queue_undo, Stack * stack_redo, char * currentPlayer) {
    
    if (isStackEmpty(stack_redo)) return 0;

    Move lastMove;
    pop(stack_redo, &lastMove);

    // makeMove(board, &lastMove, *currentPlayer);

    *currentPlayer = (*currentPlayer == BLACK) ? WHITE : BLACK;

    pushHead(queue_undo, activity(board, lastMove, *currentPlayer));

    return 1;
}

Activity activity(NodeOctuple * board, Move lastMove, char currentPlayer) {
    Activity newActivity;
    convertOctupleToArray(board, newActivity.board);
    newActivity.lastMove = lastMove;
    newActivity.currentPlayer = currentPlayer;
    return newActivity;
}

Move inputMove (NodeOctuple *root, char player){
    Move valid_moves[64]; // to collect valid moves and later will store by address
    int num_valid_moves; // how many valid move

    // Get all valid moves for the current player
    getValidMoves (root, player, valid_moves, &num_valid_moves);

    // If no valid moves 
    if (num_valid_moves == 0){
        Move invalid = {-1, -1};
        return invalid;
    }

    int selected=0;
    
    boolean isExit = false;

    while (!isExit){
        clearScreen();
        printBoard (root, valid_moves, num_valid_moves, selected, player);
        switch (userInput()){
            case LEFT: 
            // % num valid for circular || + num valid ensure selected always positive
            //move selection left
                selected = (selected-1 + num_valid_moves) % num_valid_moves;
                break;
            case RIGHT:
                selected = (selected+1) % num_valid_moves;
                break;
            case ENTER:
                isExit = true;
                break;
            default:
                break;
        }
        // % num valid for circular || + num valid ensure selected always positive
        //move selection left
    }
    //return selected move
    return valid_moves[selected];
}