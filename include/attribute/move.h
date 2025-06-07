#ifndef move_h
#define move_h

typedef struct {
    char x;
    char y;
} Move;

int isMoveEqual(Move a, Move b);

#endif