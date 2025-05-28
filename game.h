#ifndef game_h
#define game_h

#include "datastructures/octuple.h"
#include "datastructures/stack.h"
#include "datastructures/deque.h"
#include "move.h"

// cek jika game sudah berakhir.
// antara board sudah penuh atau kedua player tidak bisa melakukan gerakan
int isGameOver(NodeOctuple *board);
int hasValidMove(NodeOctuple * board, char player);

void getValidMoves(NodeOctuple *board, char player, Move *valid_moves, int *num_valid_moves);
void printBoard(NodeOctuple *board, Move *valid_moves, int num_valid_moves, int selected_idx, char player);
int isValidMove(NodeOctuple* node, char player);

Move inputMove (NodeOctuple *root, char player);

int undo(NodeOctuple * board, Deque * queue_undo, Stack * stack_undo, char * currentPlayer);
int redo(NodeOctuple * board, Deque * queue_undo, Stack * stack_redo, char * currentPlayer);
Activity activity(NodeOctuple * board, Move lastMove, char currentPlayer);

#endif