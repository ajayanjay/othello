#include "game.h"
#include "piece.h"
#include "menu.h"
#include "boolean.h"
#include "datastructures/octuple.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

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
    int row_idx = 0;

    while (row != NULL && row_idx <8){
        col=row;
        int col_idx = 0;

        while (col != NULL && col_idx < 8){
            //if isvalidmove, add to list
            if (isValidMove(col, player)){ //col=pointer node
                validMoves[*numValidMoves].x = row_idx; //*numvalidmove beacuse acces score
                validMoves[*numValidMoves].y = col_idx;
                (*numValidMoves)++; //update index validmoves
            }
            col = col->right; //update collumn
            col_idx++;
        }
        row = row->down; // update row
        row_idx++;
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
void printBoard(NodeOctuple *board, Move *validMoves, int numValidMoves, int selected_idx, char player) {
    char buffer[1024];
    int offset = 0;

    // Baris atas
    offset += sprintf(buffer + offset, "  +-----------------+\n"); //border top

    NodeOctuple *current_row = board;
    int row_number = 1;

    while (current_row != NULL && row_number <= 8) {
        offset += sprintf(buffer + offset, "%d |", row_number); //print row number and border
        NodeOctuple *current_col = current_row;
        int col_idx = 0;

        while (current_col != NULL && col_idx < 8) {
            char c = current_col->info;
            int row_idx = row_number -1;
            int is_valid = 0;
            int is_selected=0;

            //check if position contain valid move or is_selected 
            int validMoves_idx = 0;
            while (validMoves_idx < numValidMoves){
                if (validMoves[validMoves_idx].x == row_idx && validMoves[validMoves_idx].y == col_idx){
                    is_valid = 1;
                    if (validMoves_idx == selected_idx){
                        is_selected = 1;
                    }
                    break;
                }

                validMoves_idx++;
            }

            if (is_selected){ // is_selected>0 with special formatting
                offset += sprintf (buffer + offset, " \033[30;47m%c\033[0m", player);
            }
            else if (is_valid){ // possible move
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
            
            current_col = current_col->right; // update current to right
            col_idx++;
        }
        
        // end row
        offset += sprintf(buffer + offset, " |\n");
        current_row = current_row->down; // update current to bottom
        row_number++; //update row number
    }

    offset += sprintf(buffer + offset, "  +-----------------+\n"); //border bottom
    offset += sprintf(buffer + offset, "    A B C D E F G H\n");

    // Print all
    printf("%s", buffer);
}

// Author: Azzar
int undo(NodeOctuple * board, Deque * queue_undo, Stack * stack_undo, char * currentPlayer) {
    
    if (isDequeEmpty(queue_undo)) return 0;
    
    Activity lastActivity;
    popHead(queue_undo, &lastActivity);

    convertArrayToOctuple(board, lastActivity.board);
    *currentPlayer = lastActivity.currentPlayer;

    push(stack_undo, &(lastActivity.lastMove));
    return 1;
}

// Author: Azzar
int redo(NodeOctuple * board, Deque * queue_undo, Stack * stack_redo, char * currentPlayer) {
    
    if (isStackEmpty(stack_redo)) return 0;

    Move lastMove;
    pop(stack_redo, &lastMove);

    makeMove(board, &lastMove, *currentPlayer);

    *currentPlayer = (*currentPlayer == BLACK) ? WHITE : BLACK;

    pushHead(queue_undo, activity(board, lastMove, *currentPlayer));

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

// Author: Ihsan
// Note: Moved to this file by Azzar
Move inputMove (NodeOctuple *root, char player){
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
    
    boolean isExit = false;
    
    while (!isExit){
        clearScreen();
        printBoard (root, validMoves, numValidMoves, selected, player);
        switch (userInput()){
            case LEFT: 
            // % num valid for circular || + num valid ensure selected always positive
            //move selection left
                selected = (selected-1 + numValidMoves) % numValidMoves;
                break;
            case RIGHT:
                selected = (selected+1) % numValidMoves;
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
    return validMoves[selected];
}

// Author: Ihsan
// Note: Moved to this file by Azzar
void makeMove(NodeOctuple *board, Move *move, char player) {
    char opponent = (player == BLACK) ? WHITE : BLACK;
    
    NodeOctuple *moveNode = getNodeAt(board, move->x, move->y);
    
    moveNode->info = player;
    
    // Check all directions and flip opponent pieces
    for (int dir = 0; dir < 8; dir++) {
        NodeOctuple *current = getNext(moveNode, dir);
        NodeOctuple *pieces_to_flip[8]; // Max pieces that can be flipped in one direction
        int flip_count = 0;
        boolean found_opponent = false;
        
        // Move in the direction while finding opponent pieces
        while (current != NULL && current->info == opponent) {
            pieces_to_flip[flip_count] = current;
            flip_count++;
            found_opponent = true;
            current = getNext(current, dir);
        }
        
        // if found opponent pieces and ended at a player piece, flip
        if (found_opponent && current != NULL && current->info == player) {
            for (int i = 0; i < flip_count; i++) {
                pieces_to_flip[i]->info = player;
            }
        }
    }
}