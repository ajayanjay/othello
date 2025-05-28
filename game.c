#include "game.h"
#include "piece.h"
#include "menu.h"
#include "boolean.h"
#include "datastructures/octuple.h"
#include <stdlib.h>
#include <stdio.h>

// int isGameOver(NodeOctuple * board) {
//     return (isBoardFull(board) || (!hasValidMove(board, BLACK) && !hasValidMove(board, WHITE)));
// }

// int hasValidMove(NodeOctuple * board, char player) {
//     return (getValidMoves(board, player) != NULL);
// }

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