#include "game.h"
#include "piece.h"
#include "menu.h"
#include "boolean.h"
#include "datastructures/octuple.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int isGameOver(NodeOctuple * board) {
    return !hasValidMove(board, BLACK) && !hasValidMove(board, WHITE);
}

int hasValidMove(NodeOctuple * board, char player) {
    Move buffer[64]; int bufferSize = 0;
    getValidMoves(board, player, buffer, &bufferSize);
    return bufferSize > 0;
}

void getValidMoves (NodeOctuple *root, char player, Move *valid_moves, int *num_valid_moves) {
    //valid moves untuk pointer ke array
    //num valid moves passing by reference because will use in another method

    *num_valid_moves = 0; // (*) acces score not pointer. Init count valid move
    NodeOctuple *row= root;
    NodeOctuple *col;
    int row_idx = 0;

    while (row != NULL && row_idx <8){
        col=row;
        int col_idx = 0;

        while (col != NULL && col_idx < 8){
            //if isvalidmove, add to list
            if (isValidMove(col, player)){ //col=pointer node
                valid_moves[*num_valid_moves].x = row_idx; //*numvalidmove beacuse acces score
                valid_moves[*num_valid_moves].y = col_idx;
                (*num_valid_moves)++; //update index validmoves
            }
            col = col->right; //update collumn
            col_idx++;
        }
        row = row->down; // update row
        row_idx++;
    }
}

// Static helper function to check if a move is valid for the player
int isValidMove(NodeOctuple* node, char player) {
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


void printBoard(NodeOctuple *board, Move *valid_moves, int num_valid_moves, int selected_idx, char player) {
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
            int valid_moves_idx = 0;
            while (valid_moves_idx < num_valid_moves){
                if (valid_moves[valid_moves_idx].x == row_idx && valid_moves[valid_moves_idx].y == col_idx){
                    is_valid = 1;
                    if (valid_moves_idx == selected_idx){
                        is_selected = 1;
                    }
                    break;
                }

                valid_moves_idx++;
            }

            if (is_selected){ // is_selected>0 with special formatting
                offset += sprintf (buffer + offset, " \033[30;47m%c\033[0m", player);
            }
            else if (is_valid){ // possible move
                offset += sprintf (buffer + offset, " \033[2m%c\033[m", tolower(player));
            }
            else { // regular cell
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