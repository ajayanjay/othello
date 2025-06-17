#ifndef game_h
#define game_h

#include "attribute/player.h"
#include "util/boolean.h"
#include "datastructure/octuple.h"
#include "datastructure/nbtree.h"

#define SAVEDATA_DIR "gamedata/savedata"
#define SAVEDATA_BOARD_FILE "gamedata/savedata/board.dat"
#define SAVEDATA_PLAYER1_FILE "gamedata/savedata/player1.dat"
#define SAVEDATA_PLAYER2_FILE "gamedata/savedata/player2.dat"
#define SAVEDATA_REDO_FILE "gamedata/savedata/redo.dat"
#define SAVEDATA_UNDO_FILE "gamedata/savedata/undo.dat"
#define SAVEDATA_CURRENT_PLAYER_FILE "gamedata/savedata/currentPlayer.dat"

int game(Player player1, Player player2, NodeOctuple * board, Stack *stackRedo, Deque *dequeUndo, char startingPlayer);

void getValidMoves(NodeOctuple *root, char player, Move *validMoves, int *numValidMoves);
int isValidMove(NodeOctuple* node, char player);
void getPiecesToFlip(NodeOctuple *board, Move *move, char player, Move *piecesToFlip, int *numPiecesToFlip);
void printBoard(NodeOctuple *board, Move *validMoves, int numValidMoves, int selectedIndex, char player, boolean showScore);
void makeMove(NodeOctuple *board, Move *move, char player);
int isGameOver(NodeOctuple * board);
void gameOverScreen(NodeOctuple * board, Player player1, Player player2);
Activity activity(NodeOctuple * board, Move lastMove, char currentPlayer);
int undo(NodeOctuple * board, Deque * dequeUndo, Stack * stackRedo, char * currentPlayer);
int redo(NodeOctuple * board, Deque * dequeUndo, Stack * stackRedo, char * currentPlayer);

int saveGame(NodeOctuple * board, Player player1, Player player2, Stack stackRedo, Deque dequeUndo, char currentPlayer);
int loadGame(NodeOctuple ** board, Player * player1, Player * player2, Stack * stackRedo, Deque * dequeUndo, char * currentPlayer);
void removeSavedGameFiles();

Move getBestMove(NodeOctuple *board, char player, Move * moves, int movesSize,  int depth);

int minimax(NbTree *root, int depth, char aiPlayer, boolean isMax, int alpha, int beta);

void updateTree(Move lastMove);
void deleteTree();

#endif