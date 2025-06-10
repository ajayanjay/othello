#include "../../include/game.h"
#include "../../include/util/menu.h"
#include <stdlib.h>
#include <stdio.h>

extern boolean gIsAgainstHardAI;

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

                if (gIsAgainstHardAI) {
                    printf("\rCannot undo against Hard AI.");
                    break;
                } else if (isDequeEmpty(dequeUndo))
                    break;

                return (Move) {-2, -2};

            case KEY_Y:
                if (isStackEmpty(stackRedo))
                    break;

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
                if (gIsAgainstHardAI) {
                    printf("\rCannot undo against Hard AI.");
                    break;
                } else if (isDequeEmpty(dequeUndo))
                    break;

                return (Move) {-2, -2};

            case KEY_Y:
                if (isStackEmpty(stackRedo))
                    break;

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

// AI Medium difficulty using minimax with depth 1
Move playAIMedium(NodeOctuple *board, Deque * dequeUndo, Stack * stackRedo, char player) {
    Move validMoves[64];
    int numValidMoves;

    // Get all valid moves for the current player
    getValidMoves(board, player, validMoves, &numValidMoves);

    // If no valid moves 
    if (numValidMoves == 0) {
        Move invalid = {-1, -1};
        return invalid;
    }

    while (1) {
        clearScreen();
        
        // Get the best move using minimax with depth 1
        Move bestMove = getBestMove(board, player, validMoves, numValidMoves, 1);
        
        // Find the index of the best move in validMoves for display
        int selectedIndex = 0;
        int i;
        for (i = 0; i < numValidMoves; i++) {
            if (validMoves[i].x == bestMove.x && validMoves[i].y == bestMove.y) {
                selectedIndex = i;
                break;
            }
        }
        
        printBoard(board, validMoves, numValidMoves, selectedIndex, player, true);
        
        boolean unnecessaryInput = true;
        while (unnecessaryInput) switch (userInput()) {
            case KEY_Z:
                if (gIsAgainstHardAI) {
                    printf("\rCannot undo against Hard AI.");
                    break;
                } else if (isDequeEmpty(dequeUndo))
                    break;

                return (Move) {-2, -2};

            case KEY_Y:
                if (isStackEmpty(stackRedo))
                    break;
                
                return (Move) {-3, -3};
            case ENTER:
                return bestMove;
                
            default:
                break;
        }
    }
}

// AI Hard difficulty using minimax with depth 5
Move playAIHard(NodeOctuple *board, Deque * UNUSED(dequeUndo), Stack * UNUSED(stackRedo), char player) {
    Move validMoves[64];
    int numValidMoves;

    // Get all valid moves for the current player
    getValidMoves(board, player, validMoves, &numValidMoves);

    // If no valid moves 
    if (numValidMoves == 0) {
        Move invalid = {-1, -1};
        return invalid;
    }
    
    printBoard(board, validMoves, numValidMoves, -1, player, true);
    printf("\n   AI (%c) is thinking...\n", player);

    // Get the best move using minimax with depth 6
    Move bestMove = getBestMove(board, player, validMoves, numValidMoves, 6);
    
    // Find the index of the best move in validMoves for display
    int selectedIndex = 0;
    int i;
    for (i = 0; i < numValidMoves; i++) {
        if (validMoves[i].x == bestMove.x && validMoves[i].y == bestMove.y) {
            selectedIndex = i;
            break;
        }
    }
    
    clearScreen();
    printBoard(board, validMoves, numValidMoves, selectedIndex, player, true);
    inputUntilEnter();
    return bestMove;
}

Player player(PlayerType type, char symbol) {
    Player newPlayer;
    switch(type) {
        case HUMAN:
            newPlayer.play = playHuman;
            break;
            
        case AI_EASY:
            newPlayer.play = playAIEasy;
            break;

        case AI_MEDIUM:
            newPlayer.play = playAIMedium;
            break;

        case AI_HARD:
            newPlayer.play = playAIHard;
            break;

        default: 
            break;
    }

    newPlayer.symbol = symbol;
    newPlayer.type = type;

    return newPlayer;
}

int savePlayer(Player player, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }

    fwrite(&player.type, sizeof(PlayerType), 1, file);
    fwrite(&player.symbol, sizeof(char), 1, file);
    fclose(file);
    return 1;
}

int loadPlayer(Player * p, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return 0;
    }

    fread(&p->type, sizeof(PlayerType), 1, file);
    fread(&p->symbol, sizeof(char), 1, file);
    fclose(file);

    *p = player(p->type, p->symbol);

    return 1;
}