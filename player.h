#ifndef player_h
#define player_h

#ifdef __GNUC__
    #define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
    #define UNUSED(x) UNUSED_ ## x
#endif

#include "datastructures/octuple.h"
#include "datastructures/deque.h"
#include "datastructures/stack.h"
#include "move.h"

typedef enum {
    HUMAN,
    AI_EASY,
    AI_MEDIUM,
    AI_HARD,
    REPLAY
} PlayerType;

typedef struct player{
    PlayerType type;
    Move (*play) (NodeOctuple *, Deque *, Stack *, char);
    char symbol;
} Player;

Move playHuman(NodeOctuple *root, Deque * dequeUndo, Stack * stackRedo, char player);
Move playAIEasy(NodeOctuple *board, Deque * dequeUndo, Stack * stackRedo, char player);
Move playAIMedium(NodeOctuple *board, Deque * dequeUndo, Stack * stackRedo, char player);
Move playAIHard(NodeOctuple *board, Deque * UNUSED(dequeUndo), Stack * UNUSED(stackRedo), char player);

Player player(PlayerType type, char symbol);

int savePlayer(Player player, const char *filename);
int loadPlayer(Player * player, const char *filename);

#endif