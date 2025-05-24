#include "octuple.h"

NodeOctuple * getNodeAt(NodeOctuple *root, int row, int col) {
    NodeOctuple *current = root;
    for (int i = 0; i < row; ++i)
        current = current->down;
    for (int j = 0; j < col; ++j)
        current = current->right;
    return current;
}