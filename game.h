#ifndef game_h
#define game_h

#include "octuple.h"
#include "stack.h"
#include "deque.h"

// cek jika game sudah berakhir.
// antara board sudah penuh atau kedua player tidak bisa melakukan gerakan
int isGameOver(NodeOctuple *board);

void undo(NodeOctuple * board, Deque * queue_undo, Stack * stack_undo);
void redo(NodeOctuple * board, Deque * queue_undo, Stack * stack_redo);

#endif