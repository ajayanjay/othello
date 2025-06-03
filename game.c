#include "game.h"
#include "piece.h"
#include "menu.h"
#include "score.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

//Author: Azzar & Ihsan
int game(Player player1, Player player2, NodeOctuple * board, Stack *stackRedo, Deque *dequeUndo, char startingPlayer) {

    char temp;
    Player * currentPlayer = (startingPlayer == BLACK) ? &player1 : &player2;
    Move lastMove = {-1, -1};

    while (1) {
        clearScreen();
        if (isGameOver(board)) {
            printBoard(board, NULL, 0, 0, EMPTY, false);
            gameOverScreen(board, player1, player2);
            inputUntilEnter();
            break;
        }
        
        lastMove = currentPlayer->play(board, dequeUndo, stackRedo, currentPlayer->symbol);

        switch (lastMove.x) {
            case -1: // no valid moves
                printBoard(board, NULL, 0, 0, currentPlayer->symbol, true);
                printf("No valid moves for %c player.\n", currentPlayer->symbol);
                nonBlockingInput();
                currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
                break;

            case -2: // undo
                temp = currentPlayer->symbol;
                undo(board, dequeUndo, stackRedo, &temp);
                currentPlayer = (temp == BLACK) ? &player1 : &player2;
                break;

            case -3: // redo
                temp = currentPlayer->symbol;
                redo(board, dequeUndo, stackRedo, &temp);
                currentPlayer = (temp == BLACK) ? &player1 : &player2;
                break;

            default: // valid move
                emptyStack(stackRedo); // if user make a move, empty the redo stack.
                pushHead(dequeUndo, activity(board, lastMove, currentPlayer->symbol));
                makeMove(board, &lastMove, currentPlayer->symbol);
                currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
                break;
        }
    }
    return 0;
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
void printBoard(NodeOctuple *board, Move *validMoves, int numValidMoves, int selectedIndex, char player, boolean showScore) {
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
    if (showScore == true){
        printCurrentScores(board);
    }
    else{
        return;
    }
}

void gameOverScreen(NodeOctuple * board, Player player1, Player player2){
    printf("Game Over! No valid moves left.\n\n");
    // Calculate final scores
    int blackScore = calculateScore(board, BLACK);
    int whiteScore = calculateScore(board, WHITE);
    
    printf("Final Scores:\n");
    printf("Black (X): %d pieces\n", blackScore);
    printf("White (O): %d pieces\n", whiteScore);
    
    // Determine winner and handle scoring for human players
    if (blackScore > whiteScore) {
        printf("Black wins!\n\n");
        if (player1.type == HUMAN) {
            char playerName[4];
            printf("Congratulations! You achieved a high score!\n");
            getPlayerName(playerName);
            addHighScore(playerName, blackScore);
            printf("High score saved: %s - %d pieces\n", playerName, blackScore);
        }
    } else if (whiteScore > blackScore) {
        printf("White wins!\n\n");
        if (player2.type == HUMAN) {
            char playerName[4];
            printf("Congratulations! You achieved a high score!\n");
            getPlayerName(playerName);
            addHighScore(playerName, whiteScore);
            printf("High score saved: %s - %d pieces\n", playerName, whiteScore);
        }
    } else {
        printf("It's a tie!\n\n");
        // For ties, allow both human players to save their scores
        if (player1.type == HUMAN) {
            char playerName[4];
            printf("Black player achieved a tie!\n");
            getPlayerName(playerName);
            addHighScore(playerName, blackScore);
            printf("Score saved: %s - %d pieces\n", playerName, blackScore);
        }
        if (player2.type == HUMAN) {
            char playerName[4];
            printf("White player achieved a tie!\n");
            getPlayerName(playerName);
            addHighScore(playerName, whiteScore);
            printf("Score saved: %s - %d pieces\n", playerName, whiteScore);
        }
    }
    
    printf("\nPress ENTER to return to the main menu...\n");
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