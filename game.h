#ifndef game_h
#define game_h

#include "datastructures/octuple.h"
#include "datastructures/stack.h"
#include "datastructures/deque.h"
#include "move.h"
#include "activity.h"
#include "player.h"

Move playAIMedium();
Move playAIHard();
Move playReplay();

typedef struct {
    Move (*play)(NodeOctuple *board, Deque * dequeUndo, Stack * stackRedo, char player);
    char symbol;
    PlayerType playerType;
} Player;

int game();

// cek jika game sudah berakhir.
// antara board sudah penuh atau kedua player tidak bisa melakukan gerakan
int isGameOver(NodeOctuple *board);
int hasValidMove(NodeOctuple * board, char player);

void getValidMoves(NodeOctuple *board, char player, Move *validMoves, int *numValidMoves);
void printBoard(NodeOctuple *board, Move *validMoves, int numValidMoves, int selectedIndex, char player);
int isValidMove(NodeOctuple* node, char player);

Move playHuman(NodeOctuple *board, Deque * dequeUndo, Stack * stackRedo, char player);
Move playAIEasy(NodeOctuple *board, Deque * dequeUndo, Stack * stackRedo, char player);

void makeMove(NodeOctuple *board, Move *move, char player);

int undo(NodeOctuple * board, Deque * dequeUndo, Stack * stackUndo, char * currentPlayer);
int redo(NodeOctuple * board, Deque * dequeUndo, Stack * stackRedo, char * currentPlayer);
Activity activity(NodeOctuple * board, Move lastMove, char currentPlayer);

#endif