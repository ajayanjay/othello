#include "../include/game.h"
#include "../include/attribute/piece.h"
#include "../include/attribute/score.h"
#include "../include/util/menu.h"
#include "../include/util/storage.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

NbTree * gRoot = NULL;
boolean gIsAgainstHardAI = false;
Player gBlackPlayer;
Player gWhitePlayer;

//Author: Azzar & Ihsan
int game(Player player1, Player player2, NodeOctuple * board, Stack *stackRedo, Deque *dequeUndo, char startingPlayer) {

    createDirectory(SAVEDATA_DIR);

    gIsAgainstHardAI = player1.type == AI_HARD || player2.type == AI_HARD;
    gBlackPlayer = player1;
    gWhitePlayer = player2;

    Player * currentPlayer = (startingPlayer == BLACK) ? &player1 : &player2;
    Move lastMove = {-1, -1};
    char temp;

    while (1) {
        clearScreen();

        if (isGameOver(board)) {
            deleteTree();
            pushTail(dequeUndo, activity(board, (Move) {-1, -1}, currentPlayer->symbol));
            printBoard(board, NULL, 0, 0, EMPTY, false);
            gameOverScreen(board, player1, player2);
            removeSavedGameFiles();
            inputUntilEnter();
            break;
        }

        saveGame(board, player1, player2, *stackRedo, *dequeUndo, currentPlayer->symbol);
        
        lastMove = currentPlayer->play(board, dequeUndo, stackRedo, currentPlayer->symbol);

        if (gIsAgainstHardAI || player1.type == AI_MEDIUM ||  player2.type == AI_MEDIUM)
            updateTree(lastMove);

        switch (lastMove.x) {
            case -1: // current player has no available moves
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

            case -4: //escape
                return 1;

            default: // valid move.
                emptyStack(stackRedo);
                pushTail(dequeUndo, activity(board, lastMove, currentPlayer->symbol));
                makeMove(board, &lastMove, currentPlayer->symbol);
                currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
                break;
        }
    }
    return 0;
}

void updateTree(Move lastMove) {
    if (gRoot == NULL) return;
    if (lastMove.x < -1 || lastMove.y < -1) return;

    NbTree * temp = gRoot->fs;
    while (temp != NULL) {
        if (isMoveEqual(temp->info.move, lastMove)) {
            disconnectTreeExcept(&gRoot, temp);
            break;
        }
        temp = temp->nb;
    }
}

// Author: Azzar
int isGameOver(NodeOctuple * board) {
    NodeOctuple * y = board;
    NodeOctuple * x;

    while (y != NULL) {
        x = y;
        while (x != NULL) {
            if (x->info != EMPTY) {
                x = x->right;
                continue;
            }
            if (isValidMove(x, BLACK) || isValidMove(x, WHITE))
                return 0;
            
            x = x->right;
        }
        y = y->down;
    }

    return 1;
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
                validMoves[*numValidMoves].x = (char)colIndex; //*numvalidmove beacuse acces score
                validMoves[*numValidMoves].y = (char)rowIndex;
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
    char opponent = getOppositePiece(player);
    // player x then opponent O, otherwise

    // Check all all direction
    int dir;
    for (dir = 0; dir < 8; dir++) {
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

    // Get pieces that will be flipped for preview
    Move piecesToFlip[64];
    int numPiecesToFlip = 0;
    if (selectedIndex >= 0 && selectedIndex < numValidMoves) {
        getPiecesToFlip(board, &validMoves[selectedIndex], player, piecesToFlip, &numPiecesToFlip);
    }

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
            int willBeFlipped = 0;

            //check if position contain valid move or isSelected 
            int validMovesIndex = 0;
            while (validMovesIndex < numValidMoves){
                if (validMoves[validMovesIndex].x == colIndex && validMoves[validMovesIndex].y == rowIndex){
                    isValid = 1;
                    if (validMovesIndex == selectedIndex){
                        isSelected = 1;
                    }
                    break;
                }

                validMovesIndex++;
            }

            // Check if this piece will be flipped (for preview)
            if (selectedIndex >= 0) {
                int flipIndex = 0;
                while (flipIndex < numPiecesToFlip) {
                    if (piecesToFlip[flipIndex].x == colIndex && piecesToFlip[flipIndex].y == rowIndex) {
                        willBeFlipped = 1;
                        break;
                    }
                    flipIndex++;
                }
            }

            if (isSelected){ // isSelected>0 with special formatting
                offset += sprintf (buffer + offset, " \033[30;47m%c\033[0m", player);
            }
            else if (willBeFlipped){ // pieces that will be flipped
                offset += sprintf (buffer + offset, " \033[1;93m%c\033[0m", c);
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
    offset += sprintf(buffer + offset, "    A B C D E F G H\n\n");

    offset += sprintf(buffer + offset, "    Current Player:\n");
    offset += sprintf(buffer + offset, "      (%c) [%d, %d]\n", player, validMoves[selectedIndex].x + 1, validMoves[selectedIndex].y + 1);

    // Print all
    printf("%s", buffer);
    if (showScore == true){
        printCurrentScores(board);
    }
}

void gameOverScreen(NodeOctuple * board, Player player1, Player player2){
    printf("\nGame Over! No valid moves left.\n\n");
    // Calculate final scores
    int blackPieces = calculatePieces(board, BLACK);
    int whitePieces = calculatePieces(board, WHITE);
    int blackScore = calculateWeightedScore(blackPieces, gWhitePlayer.type);
    int whiteScore = calculateWeightedScore(whitePieces, gBlackPlayer.type);
    
    printf("Final Scores:\n");
    printf("Black (X): %d\n", blackScore);
    printf("White (O): %d\n", whiteScore);
    
    // Determine winner and handle scoring for human players
    if (blackScore > whiteScore) {
        printf("Black wins!\n\n");
        if (player1.type == HUMAN) {
            char playerName[4];
            printf("Congratulations! You achieved a high score!\n");
            getPlayerName(playerName);
            addHighScore(playerName, blackScore);
            printf("High score saved: %s - %d\n", playerName, blackScore);
        }
    } else if (whiteScore > blackScore) {
        printf("White wins!\n\n");
        if (player2.type == HUMAN) {
            char playerName[4];
            printf("Congratulations! You achieved a high score!\n");
            getPlayerName(playerName);
            addHighScore(playerName, whiteScore);
            printf("High score saved: %s - %d\n", playerName, whiteScore);
        }
    } else {
        printf("It's a tie!\n\n");
        // For ties, allow both human players to save their scores
        if (player1.type == HUMAN) {
            char playerName[4];
            printf("Black player achieved a tie!\n");
            getPlayerName(playerName);
            addHighScore(playerName, blackScore);
            printf("Score saved: %s - %d\n", playerName, blackScore);
        }
        if (player2.type == HUMAN) {
            char playerName[4];
            printf("White player achieved a tie!\n");
            getPlayerName(playerName);
            addHighScore(playerName, whiteScore);
            printf("Score saved: %s - %d\n", playerName, whiteScore);
        }
    }
    
    printf("Press ENTER...\n");
}

// Author: Azzar
int undo(NodeOctuple * board, Deque * dequeUndo, Stack * stackRedo, char * currentPlayer) {
    
    Activity lastActivity;
    popTail(dequeUndo, &lastActivity);

    convertArrayToOctuple(board, lastActivity.board);
    *currentPlayer = lastActivity.currentPlayer;

    push(stackRedo, &(lastActivity.lastMove));
    return 1;
}

// Author: Azzar
int redo(NodeOctuple * board, Deque * dequeUndo, Stack * stackRedo, char * currentPlayer) {

    Move lastMove;
    pop(stackRedo, &lastMove);

    pushTail(dequeUndo, activity(board, lastMove, *currentPlayer));
    
    makeMove(board, &lastMove, *currentPlayer);
    *currentPlayer = getOppositePiece(*currentPlayer);

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
    char opponent = getOppositePiece(player);
    
    NodeOctuple *moveNode = getNodeAt(board, move->x, move->y);
    
    moveNode->info = player;
    
    // Check all directions and flip opponent pieces
    int dir;
    for (dir = 0; dir < 8; dir++) {
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
            int i;
            for (i = 0; i < flipCount; i++) {
                piecesToFlip[i]->info = player;
            }
        }
    }
}

// Author: Ihsan
// Function to get pieces that will be flipped for preview
void getPiecesToFlip(NodeOctuple *board, Move *move, char player, Move *piecesToFlip, int *numPiecesToFlip) {
    *numPiecesToFlip = 0;
    char opponent = getOppositePiece(player);
    
    NodeOctuple *moveNode = getNodeAt(board, move->x, move->y);

    int dx[] = {
        -1,  // 0: left
        1,  // 1: right
        0,  // 2: up
        0,  // 3: down
        -1,  // 4: upleft
        1,  // 5: upright
        -1,  // 6: downleft
        1   // 7: downright
    };
    int dy[] = {
        0,  // 0: left
        0,  // 1: right
        -1,  // 2: up
        1,  // 3: down
        -1,  // 4: upleft
        -1,  // 5: upright
        1,  // 6: downleft
        1   // 7: downright
    };

    int dir;
    for (dir = 0; dir < 8; dir++) {
        NodeOctuple *current = getNext(moveNode, dir);
        Move tempPieces[8];
        int tempCount = 0;
        boolean foundOpponent = false;

        // Calculate the coordinates in the direction
        int cx = move->x + dx[dir];
        int cy = move->y + dy[dir];

        while (current != NULL && current->info == opponent) {
            // Save the opponent piece coordinates
            tempPieces[tempCount].x = (char)cx;
            tempPieces[tempCount].y = (char)cy;
            tempCount++;
            foundOpponent = true;

            // next node in the direction
            current = getNext(current, dir);
            cx += dx[dir];
            cy += dy[dir];
        }

        // if found our pion, save all tempPieces
        if (foundOpponent && current != NULL && current->info == player) {
            int i;
            for (i = 0; i < tempCount; i++) {
                piecesToFlip[*numPiecesToFlip] = tempPieces[i];
                (*numPiecesToFlip)++;
            }
        }
    }
}

int saveCurrentPlayer(char currentPlayer, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }
    fwrite(&currentPlayer, sizeof(char), 1, file);
    fclose(file);
    return 1;
}

int loadCurrentPlayer(char *currentPlayer, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return 0;
    }
    fread(currentPlayer, sizeof(char), 1, file);
    fclose(file);
    return 1;
}

void removeSavedGameFiles() {
    remove(SAVEDATA_BOARD_FILE);
    remove(SAVEDATA_PLAYER1_FILE);
    remove(SAVEDATA_PLAYER2_FILE);
    remove(SAVEDATA_REDO_FILE);
    remove(SAVEDATA_UNDO_FILE);
    remove(SAVEDATA_CURRENT_PLAYER_FILE);
    removeDirectory(SAVEDATA_DIR);
}

int saveGame(NodeOctuple * board, Player player1, Player player2, Stack stackRedo, Deque dequeUndo, char currentPlayer) {
    saveBoard(board, SAVEDATA_BOARD_FILE);
    saveStack(&stackRedo, SAVEDATA_REDO_FILE);
    saveDeque(&dequeUndo, SAVEDATA_UNDO_FILE);
    savePlayer(player1, SAVEDATA_PLAYER1_FILE);
    savePlayer(player2, SAVEDATA_PLAYER2_FILE);
    saveCurrentPlayer(currentPlayer, SAVEDATA_CURRENT_PLAYER_FILE);

    return 1;
}

int loadGame(NodeOctuple ** board, Player * player1, Player * player2, Stack * stackRedo, Deque * dequeUndo, char * currentPlayer) {
    if (!loadBoard(board, SAVEDATA_BOARD_FILE) ||
        !loadStack(stackRedo, SAVEDATA_REDO_FILE) ||
        !loadDeque(dequeUndo, SAVEDATA_UNDO_FILE) ||
        !loadPlayer(player1, SAVEDATA_PLAYER1_FILE) ||
        !loadPlayer(player2, SAVEDATA_PLAYER2_FILE) ||
        !loadCurrentPlayer(currentPlayer, SAVEDATA_CURRENT_PLAYER_FILE)) {
            return 0;
        }

    return 1;
}

Move getBestMove(NodeOctuple *board, char player, Move * moves, int movesSize, int depth) {
    char boardArray[8][8];
    convertOctupleToArray(board, boardArray);

    if (gRoot == NULL) gRoot = createNodeTree(createAIInfo(boardArray, player, (Move){-1, -1}));

    if (!isBoardEqual(gRoot->info.board, boardArray)) {
        // if the board is not equal, we will reset the tree.
        deleteTree();
        gRoot = createNodeTree(createAIInfo(boardArray, player, (Move){-1, -1}));
    }

    if (gRoot->fs == NULL) {
        // create the first level of the tree with all possible moves.
        int i;
        for (i = 0; i < movesSize; ++i) {
            char tempBoard[8][8];
            copyBoard(tempBoard, boardArray);
            makeMoveArray(tempBoard, &moves[i], player);

            char nextPlayer = getOppositePiece(player);
            NbTree * childNode = createNodeTree(createAIInfo(tempBoard, nextPlayer, moves[i]));
            insertChild(gRoot, childNode);
        }
    }

    NbTree * child = gRoot->fs;
    Move bestMove = {-1, -1};
    int bestValue = INT_MIN;

    while (child != NULL) {
        int val = minimax(child, depth - 1, player, false, bestValue, INT_MAX);
        
        if (val > bestValue) {
            bestValue = val;
            bestMove = child->info.move;
        }
        
        child = child->nb;
    }

    return bestMove;
    
}

int minimax(NbTree * node, int depth, char player, boolean isMax, int alpha, int beta) {

    if (node->info.isGameFinished)
        return evaluateFinishedGameBoardArray(node->info.board, player);

    if (depth == 0)
        return evaluateBoardArray(node->info.board, player);

    if (node->fs == NULL) {

        // if has no son, build the tree from this node.

        int validMoveCount;
        Move * validMoves = getValidMovesArray(node->info.board, node->info.currentPlayer, &validMoveCount);

        if (validMoveCount == 0) {
            char opponent = getOppositePiece(node->info.currentPlayer);
            NbTree * passNode = createNodeTree(createAIInfo(node->info.board, opponent, (Move){-1, -1}));
            node->fs = passNode;
            return minimax(passNode, depth - 1, player, !isMax, alpha, beta);
        }

        int bestScore = isMax ? INT_MIN : INT_MAX;
        boolean cutoff = false;
        int i;
        for (i = 0; i < validMoveCount; ++i) {
            char tempBoard[8][8];
            copyBoard(tempBoard, node->info.board);
            makeMoveArray(tempBoard, &validMoves[i], node->info.currentPlayer);

            char nextPlayer = getOppositePiece(node->info.currentPlayer);
            NbTree * childNode = createNodeTree(createAIInfo(tempBoard, nextPlayer, validMoves[i]));
            insertChild(node, childNode);

            int eval = isMax ? INT_MIN : INT_MAX;
            if (!cutoff) eval = minimax(childNode, depth - 1, player, !isMax, alpha, beta);
            
            if (isMax) {
                bestScore = max(bestScore, eval);
                alpha = max(alpha, eval);
            } else {
                bestScore = min(bestScore, eval);
                beta = min(beta, eval);
            }

            // if cutoff did happen, stop evaluation but keep adding children of the current note.
            if (beta <= alpha) cutoff = true;
        }

        free(validMoves);
        return bestScore;

    } else {

        // if has son, we will iterate through all of the sons.

        NbTree * child = node->fs;
        
        // if the node is a pass node, we will evaluate the first son.
        if (isMoveEqual(node->info.move, (Move){-1, -1})) 
            return minimax(child, depth - 1, player, !isMax, alpha, beta);
        
        // if the node has children, we will iterate through them.
        // this is used to build the tree from an existing node.
        int bestScore = isMax ? INT_MIN : INT_MAX;
        while (child != NULL) {
            int eval = minimax(child, depth - 1, player, !isMax, alpha, beta);
            
            if (isMax) {
                bestScore = max(bestScore, eval);
                alpha = max(alpha, eval);
            } else {
                bestScore = min(bestScore, eval);
                beta = min(beta, eval);
            }

            if (beta <= alpha) break;

            child = child->nb;
        }
        return bestScore;
    }

    return 0;
}

void deleteTree() {
    if (gRoot != NULL) {
        deleteEntireTree(&gRoot);
        gRoot = NULL;
    }
}