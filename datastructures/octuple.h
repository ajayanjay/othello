#ifndef octuple_h
#define octuple_h

typedef int OctupleInfo;

typedef struct nodeoctuple {
    OctupleInfo info;
    struct nodeoctuple *left, *right, *up, *down, *upleft, *upright, *downleft, *downright;
} NodeOctuple;

NodeOctuple * createNodeOctuple(OctupleInfo info);

// creates othello board, consists of 8x8 octuples. All linked to each other.
void constructOthelloBoard(NodeOctuple **root);

// returns the node at the given row and column
NodeOctuple * getNodeAt(NodeOctuple *root, int row, int col);

void printBoard(NodeOctuple *board);

NodeOctuple * setNodeAt (NodeOctuple *root, OctupleInfo info,  int row, int col);

void convertOctupleToArray (NodeOctuple *root, char boardArray[8][8]);

void convertArrayToOctuple(NodeOctuple *root, char boardArray[8][8]);

void saveBoard(NodeOctuple *root, const char *filename);

int loadBoard(NodeOctuple **root, const char *filename);

#endif