#ifndef game_h
#define game_h

#include "datastructures/octuple.h"
#include "datastructures/deque.h"
#include "datastructures/stack.h"
#include "player.h"
#include "move.h"
#include "activity.h"

typedef struct {
    Move (*play)(NodeOctuple*, Deque*, Stack*, char);
    char symbol;
} Player;

int game(PlayerType player1Type, PlayerType player2Type, Stack *stackRedo, Deque *dequeUndo, char startingPlayer);
        
Move playHuman(NodeOctuple *root, Deque * dequeUndo, Stack * stackRedo, char player);
Move playAIEasy(NodeOctuple *board, Deque * dequeUndo, Stack * stackRedo, char player);
// Move playAIMedium(NodeOctuple *board, Deque * dequeUndo, Stack * stackRedo, char player);
// Move playAIHard(NodeOctuple *board, Deque * dequeUndo, Stack * stackRedo, char player);

void getValidMoves(NodeOctuple *root, char player, Move *validMoves, int *numValidMoves);
int isValidMove(NodeOctuple* node, char player);
void printBoard(NodeOctuple *board, Move *validMoves, int numValidMoves, int selectedIndex, char player);
void makeMove(NodeOctuple *board, Move *move, char player);
int hasValidMove(NodeOctuple * board, char player);
int isGameOver(NodeOctuple * board);
Activity activity(NodeOctuple * board, Move lastMove, char currentPlayer);
int undo(NodeOctuple * board, Deque * dequeUndo, Stack * stackRedo, char * currentPlayer);
int redo(NodeOctuple * board, Deque * dequeUndo, Stack * stackRedo, char * currentPlayer);

#endif