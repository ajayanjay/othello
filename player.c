#include "game.h"
#include "menu.h"
#include <stdlib.h>

// Author: Azzar
Move playAIEasy(NodeOctuple *board, Deque * dequeUndo, Stack * stackRedo, char player) {
    Move validMoves[64]; // to collect valid moves and later will store by address
    int numValidMoves; // how many valid move

    // Get all valid moves for the current player
    getValidMoves(board, player, validMoves, &numValidMoves);

    // If no valid moves 
    if (numValidMoves == 0) {
        Move invalid = {-1, -1};
        return invalid;
    }

    int selected = rand() % numValidMoves;

    while (1) {
        clearScreen();
        printBoard (board, validMoves, numValidMoves, selected, player, true);

        boolean unnecessaryInput = true;
        while (unnecessaryInput) switch (userInput()) {
            case KEY_Z:
                if (isDequeEmpty(dequeUndo)) break;

                return (Move) {-2, -2};

            case KEY_Y:
                if (isStackEmpty(stackRedo)) break;

                return (Move) {-3, -3};
            case ENTER:
                return validMoves[selected];
                
            default:
                break;
        }
        
    }

    return validMoves[selected];
}

// Author: Ihsan
// Note: Moved to this file by Azzar
Move playHuman(NodeOctuple *root, Deque * dequeUndo, Stack * stackRedo, char player){
    Move validMoves[64]; // to collect valid moves and later will store by address
    int numValidMoves; // how many valid move

    // Get all valid moves for the current player
    getValidMoves (root, player, validMoves, &numValidMoves);

    // If no valid moves 
    if (numValidMoves == 0){
        Move invalid = {-1, -1};
        return invalid;
    }

    int selected=0;
    
    while (1){
        clearScreen();
        printBoard (root, validMoves, numValidMoves, selected, player, true);

        boolean unnecessaryInput = true;
        while (unnecessaryInput) switch (userInput()){
            case LEFT: 
            // % num valid for circular || + num valid ensure selected always positive
            //move selection left
                selected = (selected-1 + numValidMoves) % numValidMoves;
                unnecessaryInput = false;
                break;

            case RIGHT:
                selected = (selected+1) % numValidMoves;
                unnecessaryInput = false;
                break;

            case KEY_Z:
                if (isDequeEmpty(dequeUndo)) break;

                return (Move) {-2, -2};

            case KEY_Y:
                if (isStackEmpty(stackRedo)) break;

                return (Move) {-3, -3};

            case ENTER:
                return validMoves[selected];
                
            default:
                break;
        }
        // % num valid for circular || + num valid ensure selected always positive
        //move selection left
    }
    //return selected move
    return validMoves[selected];
}
// Move playAIMedium(NodeOctuple *board, Deque * dequeUndo, Stack * stackRedo, char player);
// Move playAIHard(NodeOctuple *board, Deque * dequeUndo, Stack * stackRedo, char player);
