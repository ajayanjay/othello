#include "move.h"

int isMoveEqual(Move a, Move b) {
    return a.x == b.x && a.y == b.y;
}