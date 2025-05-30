#ifndef game_h
#define game_h

#include "datastructures/octuple.h"
#include "datastructures/stack.h"
#include "datastructures/deque.h"
#include "move.h"
#include "activity.h"

typedef struct {
    playerType player;
} playerType;

// cek jika game sudah berakhir.
// antara board sudah penuh atau kedua player tidak bisa melakukan gerakan
int isGameOver(NodeOctuple *board);
int hasValidMove(NodeOctuple * board, char player);

void getValidMoves(NodeOctuple *board, char player, Move *validMoves, int *numValidMoves);
void printBoard(NodeOctuple *board, Move *validMoves, int numValidMoves, int selected_idx, char player);
int isValidMove(NodeOctuple* node, char player);

Move inputMove (NodeOctuple *root, char player);

void makeMove(NodeOctuple *board, Move *move, char player);

int undo(NodeOctuple * board, Deque * queue_undo, Stack * stack_undo, char * currentPlayer);
int redo(NodeOctuple * board, Deque * queue_undo, Stack * stack_redo, char * currentPlayer);
Activity activity(NodeOctuple * board, Move lastMove, char currentPlayer);

#endif