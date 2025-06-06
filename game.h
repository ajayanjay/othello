#ifndef game_h
#define game_h

#include "player.h"
#include "boolean.h"

int game(Player player1, Player player2, NodeOctuple * board, Stack *stackRedo, Deque *dequeUndo, char startingPlayer);

void getValidMoves(NodeOctuple *root, char player, Move *validMoves, int *numValidMoves);
int isValidMove(NodeOctuple* node, char player);
void printBoard(NodeOctuple *board, Move *validMoves, int numValidMoves, int selectedIndex, char player, boolean showScore);
void makeMove(NodeOctuple *board, Move *move, char player);
int hasValidMove(NodeOctuple * board, char player);
int isGameOver(NodeOctuple * board);
void gameOverScreen(NodeOctuple * board, Player player1, Player player2);
Activity activity(NodeOctuple * board, Move lastMove, char currentPlayer);
int undo(NodeOctuple * board, Deque * dequeUndo, Stack * stackRedo, char * currentPlayer);
int redo(NodeOctuple * board, Deque * dequeUndo, Stack * stackRedo, char * currentPlayer);

Move getBestMove(NodeOctuple *board, char player, Move * moves, int movesSize,  int depth);

void updateTree(Move lastMove);

void deleteTree();

#endif