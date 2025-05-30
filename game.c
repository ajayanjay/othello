#include "game.h"
#include "piece.h"
#include "menu.h"
#include "boolean.h"
#include "datastructures/octuple.h"
#include "player.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

//Author: Azzar & Ihsan
int game(PlayerType player1Type, PlayerType player2Type, Stack *stackRedo, Deque *dequeUndo, char startingPlayer) {
    NodeOctuple * board;
    constructOthelloBoard(&board);
    // stackRedo dan dequeUndo sudah diinisialisasi di luar

    // Map player types to play functions
    Move (*player1Function)(NodeOctuple*, Deque*, Stack*, char) = NULL;
    Move (*player2Function)(NodeOctuple*, Deque*, Stack*, char) = NULL;

    switch(player1Type) {
        case HUMAN:
            player1Function = playHuman;
            break;
        case AI_EASY:
            player1Function = playAIEasy;
            break;
        case AI_MEDIUM:
            // player1Function = playAIMedium;
            break;
        case AI_HARD:
            // player1Function = playAIHard;
            break;
        default:
            player1Function = playHuman;
            break;
    }

    switch(player2Type) {
        case HUMAN:
            player2Function = playHuman;
            break;
        case AI_EASY:
            player2Function = playAIEasy;
            break;
        case AI_MEDIUM:
            // player2Function = playAIMedium;
            break;
        case AI_HARD:
            // player2Function = playAIHard;
            break;
        default:
            player2Function = playHuman;
            break;
    }

    Player player1 = {player1Function, BLACK};
    Player player2 = {player2Function, WHITE};

    Player * currentPlayer = (startingPlayer == BLACK) ? &player1 : &player2;
    Move lastMove = {-1, -1};

    while (1) {
        clearScreen();
        if (isGameOver(board)) {
            printBoard(board, NULL, 0, 0, EMPTY);
            printf("Game Over! No valid moves left for both players.\n");
            inputUntilChar('P');
            break;
        }
        lastMove = currentPlayer->play(board, dequeUndo, stackRedo, currentPlayer->symbol);
        // currentPlayer has no available moves.
        if (lastMove.x == -1 && lastMove.y == -1) {
            currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
            continue;
        }
        // undo
        else if (lastMove.x == -2 && lastMove.y == -2) {
            char temp = currentPlayer->symbol;
            undo(board, dequeUndo, stackRedo, &temp);
            currentPlayer = (temp == BLACK) ? &player1 : &player2;
            continue;
        }
        // redo
        else if (lastMove.x == -3 && lastMove.y == -3) {
            char temp = currentPlayer->symbol;
            redo(board, dequeUndo, stackRedo, &temp);
            currentPlayer = (temp == BLACK) ? &player1 : &player2;
            continue;
        }
        emptyStack(stackRedo); // if user make a move, empty the redo stack.
        pushHead(dequeUndo, activity(board, lastMove, currentPlayer->symbol));
        makeMove(board, &lastMove, currentPlayer->symbol);
        currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
    }
    return 0;
}

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
        printBoard (board, validMoves, numValidMoves, selected, player);

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
        printBoard (root, validMoves, numValidMoves, selected, player);

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

// Author: Azzar
int isGameOver(NodeOctuple * board) {
    return !hasValidMove(board, BLACK) && !hasValidMove(board, WHITE);
}

// Author: Azzar
int hasValidMove(NodeOctuple * board, char player) {
    Move buffer[64]; int bufferSize = 0;
    getValidMoves(board, player, buffer, &bufferSize);
    return bufferSize > 0;
}

// Author: Ihsan
// Note: Moved to this file by Azzar
void getValidMoves (NodeOctuple *root, char player, Move *validMoves, int *numValidMoves) {
    //valid moves untuk pointer ke array
    //num valid moves passing by reference because will use in another method

    *numValidMoves = 0; // (*) acces score not pointer. Init count valid move
    NodeOctuple *row= root;
    NodeOctuple *col;
    int rowIndex = 0;

    while (row != NULL && rowIndex <8){
        col=row;
        int colIndex = 0;

        while (col != NULL && colIndex < 8){
            //if isvalidmove, add to list
            if (isValidMove(col, player)){ //col=pointer node
                validMoves[*numValidMoves].x = rowIndex; //*numvalidmove beacuse acces score
                validMoves[*numValidMoves].y = colIndex;
                (*numValidMoves)++; //update index validmoves
            }
            col = col->right; //update collumn
            colIndex++;
        }
        row = row->down; // update row
        rowIndex++;
    }
}

// Author: Ihsan
// Note: Moved to this file by Azzar
int isValidMove(NodeOctuple* node, char player) {
    //Static helper function to check if a move is valid for the player

    // If the node is not empty, it's not a valid move
    if (node->info != EMPTY) return false;

    // Determine the opponent's symbol
    char opponent = (player == BLACK) ? WHITE : BLACK;
    // player x then opponent O, otherwise

    // Check all all direction
    for (int dir = 0; dir < 8; dir++) {
        NodeOctuple* current = getNext(node, dir);
        int count = 0;

        // Move in all directions when facing opponent's pieces
        while (current != NULL && current->info == opponent) {
            current = getNext(current, dir);
            count++;
        }

        // If we end up at a piece of the player and have passed over opponent pieces, it's valid
        if (current != NULL && current->info == player && count > 0) {
            return true;
        }
    }

    // If no capturing path is found, it's not valid
    return false;
}

// Author: Ihsan
// Note: Moved to this file by Azzar
void printBoard(NodeOctuple *board, Move *validMoves, int numValidMoves, int selectedIndex, char player) {
    char buffer[1024];
    int offset = 0;

    // Baris atas
    offset += sprintf(buffer + offset, "  +-----------------+\n"); //border top

    NodeOctuple *currentRow = board;
    int rowNumber = 1;

    while (currentRow != NULL && rowNumber <= 8) {
        offset += sprintf(buffer + offset, "%d |", rowNumber); //print row number and border
        NodeOctuple *currentCol = currentRow;
        int colIndex = 0;

        while (currentCol != NULL && colIndex < 8) {
            char c = currentCol->info;
            int rowIndex = rowNumber -1;
            int isValid = 0;
            int isSelected=0;

            //check if position contain valid move or isSelected 
            int validMovesIndex = 0;
            while (validMovesIndex < numValidMoves){
                if (validMoves[validMovesIndex].x == rowIndex && validMoves[validMovesIndex].y == colIndex){
                    isValid = 1;
                    if (validMovesIndex == selectedIndex){
                        isSelected = 1;
                    }
                    break;
                }

                validMovesIndex++;
            }

            if (isSelected){ // isSelected>0 with special formatting
                offset += sprintf (buffer + offset, " \033[30;47m%c\033[0m", player);
            }
            else if (isValid){ // possible move
                offset += sprintf (buffer + offset, " \033[2m%c\033[m", tolower(player));
            }
            else { // regular cell
                if (c == BLACK)
                    offset += sprintf (buffer + offset, " \033[91m%c\033[m", c);
                else if (c == WHITE)
                    offset += sprintf (buffer + offset, " \033[96m%c\033[m", c);
                else
                    offset += sprintf (buffer + offset, " %c", c);
            }
            
            currentCol = currentCol->right; // update current to right
            colIndex++;
        }
        
        // end row
        offset += sprintf(buffer + offset, " |\n");
        currentRow = currentRow->down; // update current to bottom
        rowNumber++; //update row number
    }

    offset += sprintf(buffer + offset, "  +-----------------+\n"); //border bottom
    offset += sprintf(buffer + offset, "    A B C D E F G H\n");

    // Print all
    printf("%s", buffer);
}

// Author: Azzar
int undo(NodeOctuple * board, Deque * dequeUndo, Stack * stackRedo, char * currentPlayer) {
    
    Activity lastActivity;
    popHead(dequeUndo, &lastActivity);

    convertArrayToOctuple(board, lastActivity.board);
    *currentPlayer = lastActivity.currentPlayer;

    push(stackRedo, &(lastActivity.lastMove));
    return 1;
}

// Author: Azzar
int redo(NodeOctuple * board, Deque * dequeUndo, Stack * stackRedo, char * currentPlayer) {

    Move lastMove;
    pop(stackRedo, &lastMove);

    pushHead(dequeUndo, activity(board, lastMove, *currentPlayer));
    
    makeMove(board, &lastMove, *currentPlayer);
    *currentPlayer = (*currentPlayer == BLACK) ? WHITE : BLACK;

    return 1;
}

// Author: Azzar
Activity activity(NodeOctuple * board, Move lastMove, char currentPlayer) {
    Activity newActivity;
    convertOctupleToArray(board, newActivity.board);
    newActivity.lastMove = lastMove;
    newActivity.currentPlayer = currentPlayer;
    return newActivity;
}

// Author: Idotoho
// Note: Moved to this file by Azzar
void makeMove(NodeOctuple *board, Move *move, char player) {
    char opponent = (player == BLACK) ? WHITE : BLACK;
    
    NodeOctuple *moveNode = getNodeAt(board, move->x, move->y);
    
    moveNode->info = player;
    
    // Check all directions and flip opponent pieces
    for (int dir = 0; dir < 8; dir++) {
        NodeOctuple *current = getNext(moveNode, dir);
        NodeOctuple *piecesToFlip[8]; // Max pieces that can be flipped in one direction
        int flipCount = 0;
        boolean foundOpponent = false;
        
        // Move in the direction while finding opponent pieces
        while (current != NULL && current->info == opponent) {
            piecesToFlip[flipCount] = current;
            flipCount++;
            foundOpponent = true;
            current = getNext(current, dir);
        }
        
        // if found opponent pieces and ended at a player piece, flip
        if (foundOpponent && current != NULL && current->info == player) {
            for (int i = 0; i < flipCount; i++) {
                piecesToFlip[i]->info = player;
            }
        }
    }
}