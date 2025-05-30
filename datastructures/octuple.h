#ifndef octuple_h
#define octuple_h

typedef char OctupleInfo;

typedef struct nodeoctuple {
    OctupleInfo info;
    struct nodeoctuple
    *upleft,    *up,    *upright,
    *left,              *right,
    *downleft,  *down,  *downright;
} NodeOctuple;

NodeOctuple * createNodeOctuple(OctupleInfo info);

// creates othello board, consists of 8x8 octuples. All linked to each other.
void constructOthelloBoard(NodeOctuple **root);

// returns the node at the given row and column
NodeOctuple * getNodeAt(NodeOctuple *root, int row, int col);

NodeOctuple * setNodeAt (NodeOctuple *root, OctupleInfo info,  int row, int col);

NodeOctuple * getNext(NodeOctuple *node, int direction);

void convertOctupleToArray (NodeOctuple *root, char boardArray[8][8]);

void convertArrayToOctuple(NodeOctuple *root, char boardArray[8][8]);

void saveBoard(NodeOctuple *root, const char *filename);

int loadBoard(NodeOctuple **root, const char *filename);

void freeBoard (NodeOctuple *row);

#endif