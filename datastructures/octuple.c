#include <stdio.h>
#include <stdlib.h>
#include "octuple.h"
#include "../piece.h"

// Author: Ihsan
NodeOctuple * createNodeOctuple (OctupleInfo info){
    NodeOctuple * newNode = malloc (sizeof(NodeOctuple));
    newNode->info = info;
    newNode->up = NULL; 
    newNode->down = NULL;
    newNode->left = NULL; 
    newNode->right = NULL;
    newNode->upleft = NULL; 
    newNode->upright = NULL;
    newNode->downleft = NULL; 
    newNode->downright = NULL; 
    return newNode;
}

// Author: Ihsan
void constructOthelloBoard(NodeOctuple **root) {
    /*
    create octuple linked list for board othello 8x8
    */

    // Init first node
    (*root) = createNodeOctuple(EMPTY);
    NodeOctuple *current = *root;

    int i = 0; //init first line and connect left right
    while (i < 7) {
        NodeOctuple *newNode = createNodeOctuple(EMPTY);
        current->right = newNode;
        newNode->left = current;
        current = newNode; //update current
        i++;
    }

    // save row before
    NodeOctuple *prevRowStart = *root;

    int row = 0;
    while (row < 7) {
        NodeOctuple *firstrow = createNodeOctuple(EMPTY);
        current = firstrow;

        // connect first node to up and down
        current->up = prevRowStart;
        if (prevRowStart != NULL) {
            prevRowStart->down = current;
            // Set diagonal (upright & downleft) for first node
            if (prevRowStart->right != NULL) {
                current->upright = prevRowStart->right;
                prevRowStart->right->downleft = current;
            }
        }

        NodeOctuple *prevCurrent = prevRowStart; //create current for traversal in prev row
        i = 0;
        while (i < 7) {
            // Set infotype based on position (especially middle set rule othello)
            char color = EMPTY;
            if (row == 2 && i == 2) color = WHITE; // White on row 3, col 3 (d4)
            else if (row == 2 && i == 3) color = BLACK; // Black on row 3, col 4 (e4)
            else if (row == 3 && i == 2) color = BLACK; // Black on row 4, col 3 (d5)
            else if (row == 3 && i == 3) color = WHITE; // White on row 4, col 4 (e5)

            NodeOctuple *newNode = createNodeOctuple(color);
            // connect left and right 
            current->right = newNode;
            newNode->left = current;

            // Connect up and down (ensure not node in right end)
            if (prevCurrent != NULL && prevCurrent->right != NULL) {
                newNode->up = prevCurrent->right; // because newNode in right, not straight from prev current
                // not straight from prev current
                prevCurrent->right->down = newNode;
            }

            // diagonal upleft and upright
            if (newNode->up != NULL) {
                if (newNode->up->left != NULL) {
                    newNode->upleft = newNode->up->left;
                }
                if (i < 6 && newNode->up->right != NULL) { // Ensure j+1 < 8
                    newNode->upright = newNode->up->right;
                    newNode->up->right->downleft = newNode; // Set reverse pointer
                }
            }

            // diagonal downright and downleft
            if (prevCurrent != NULL) {
                if (current != NULL) {
                    prevCurrent->downright = newNode;
                    newNode->upleft = prevCurrent; // Set reverse pointer
                }
                if (current->left != NULL) {
                    prevCurrent->downleft = current->left;
                    current->left->upright = prevCurrent; // Set reverse pointer
                } else {
                    prevCurrent->downleft = NULL;
                }
            }

            current = newNode;
            if (prevCurrent != NULL) {
                prevCurrent = prevCurrent->right; //update prevCurrent
            }
            i++;
        }

        row++;
        prevRowStart = firstrow; //update prevRowStart
    }
}

// Author: Ihsan
NodeOctuple * setNodeAt (NodeOctuple *root, OctupleInfo info,  int col, int row){
    NodeOctuple *current= root;
    int i=0;
    while (i<row){
        current = current->down;
        i++;
    }
    i=0;
    while (i<col){
        current = current->right;
        ++i;
    }
    current->info = info;
    return current;
}

// Author: Ihsan
NodeOctuple * getNodeAt(NodeOctuple *root, int col, int row) {
    NodeOctuple *current = root;
    for (int i = 0; i < row; ++i)
        current = current->down;
    for (int j = 0; j < col; ++j)
        current = current->right;
    return current;
}

// Author: Ihsan
void convertOctupleToArray (NodeOctuple *root, char boardArray[8][8]){
    /*
    Convert Octuple link list to 8x8 array by traversing board
    */
    if (root==NULL) {
        printf ("Papan Octuple kosong\n");
        return;
    }
    
    NodeOctuple *row = root;
    int i = 0;
    while (row != NULL && i < 8) {
        NodeOctuple *col = row;
        int j = 0;
        while (col != NULL && j < 8) {
            boardArray[i][j] = col->info; //duplicate
            col = col->right; //right
            j++;
        }
        row = row->down; //down
        i++;
    }
}

// Author: Ihsan
void convertArrayToOctuple(NodeOctuple *root, char boardArray[8][8]) {
    /*
    Update the info field of an existing octuple linked list with values from an 8x8 array
    Assumes the linked list already has all pointers correctly connected
    */
    if (root == NULL) {
        return; // Handle empty list
    }

    NodeOctuple *row = root;
    int i = 0;
    while (row != NULL && i < 8) {
        NodeOctuple *col = row;
        int j = 0;
        while (col != NULL && j < 8) {
            col->info = boardArray[i][j]; // Update info with array value
            col = col->right; // Move to next column
            j++;
        }
        row = row->down; // Move to next row
        i++;
    }
}

// Author: Ihsan
void saveBoard(NodeOctuple *root, const char *filename) {
    /*
    Save the octuple linked list board to a text file in 8x8 grid format
    Each line contains 8 characters ('X', 'O', or '.') followed by a newline
    */
    if (root == NULL) {
        return;
    }

    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Gagal membuka file %s untuk menulis\n", filename);
        return;
    }

    NodeOctuple *row = root;
    while (row != NULL) {
        NodeOctuple *col = row;
        while (col != NULL) {
            fputc(col->info, file); // Write info to file
            col = col->right;
        }
        fputc('\n', file); // Newline after each row
        row = row->down;
    }

    fclose(file);
}

// Author: Idotoho
int loadBoard(NodeOctuple **root, const char *filename) {
    /*
    Load the octuple linked list board from a text file in 8x8 grid format
    Each line contains 8 characters ('X', 'O', or '.') followed by a newline
    */
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return 0; // Failed to open file
    }

    // Read the board into a 2D array first for easier processing
    char boardArray[8][8];
    char line[10]; // Buffer for reading lines (8 chars + newline + null terminator)
    int rowCount = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && rowCount < 8) {
        // Skip empty lines
        if (line[0] == '\n') continue;
        
        // Validate line has exactly 8 characters
        int charCount = 0;
        while (charCount < 8 && line[charCount] != '\n' && line[charCount] != '\0') {
            if (line[charCount] == BLACK || line[charCount] == WHITE || line[charCount] == EMPTY) {
                boardArray[rowCount][charCount] = line[charCount];
                charCount++;
            } else {
                fclose(file);
                return 0; // Invalid character
            }
        }
        
        if (charCount != 8) {
            fclose(file);
            return 0; // Invalid line length
        }
        
        rowCount++;
    }
    
    fclose(file);
    
    if (rowCount != 8) {
        return 0; // Invalid number of rows
    }

    /* Create a new board structure using constructOthelloBoard logic
       from this point, it is possible to create a new board directly from the array with:
    constructOthelloBoard(root);
    convertArrayToOctuple(*root, boardArray);
       but the method used below saves one iteration.
    */
    *root = createNodeOctuple(boardArray[0][0]);
    NodeOctuple *current = *root;

    // Create first row and connect left-right
    for (int j = 1; j < 8; j++) {
        NodeOctuple *newNode = createNodeOctuple(boardArray[0][j]);
        current->right = newNode;
        newNode->left = current;
        current = newNode;
    }

    NodeOctuple *prevRowStart = *root;

    // Create remaining rows with full octuple connections
    for (int i = 1; i < 8; i++) {
        NodeOctuple *firstNode = createNodeOctuple(boardArray[i][0]);
        current = firstNode;

        // Connect up-down for first node
        current->up = prevRowStart;
        prevRowStart->down = current;

        // Set diagonal connections for first node
        if (prevRowStart->right != NULL) {
            current->upright = prevRowStart->right;
            prevRowStart->right->downleft = current;
        }

        NodeOctuple *prevCurrent = prevRowStart;
        
        // Create rest of the row
        for (int j = 1; j < 8; j++) {
            NodeOctuple *newNode = createNodeOctuple(boardArray[i][j]);
            
            // Connect left-right
            current->right = newNode;
            newNode->left = current;

            // Connect up-down
            if (prevCurrent->right != NULL) {
                newNode->up = prevCurrent->right;
                prevCurrent->right->down = newNode;
            }

            // Set diagonal connections
            if (newNode->up != NULL) {
                // upleft connection
                if (newNode->up->left != NULL) {
                    newNode->upleft = newNode->up->left;
                    newNode->up->left->downright = newNode;
                }
                // upright connection
                if (j < 7 && newNode->up->right != NULL) {
                    newNode->upright = newNode->up->right;
                    newNode->up->right->downleft = newNode;
                }
            }

            // downleft connection for previous node in previous row
            if (prevCurrent != NULL && current != NULL) {
                prevCurrent->downright = newNode;
                newNode->upleft = prevCurrent;
            }

            current = newNode;
            if (prevCurrent != NULL) {
                prevCurrent = prevCurrent->right;
            }
        }

        prevRowStart = firstNode;
    }

    return 1; // Board loaded successfully
}

// Author: Ihsan
NodeOctuple* getNext(NodeOctuple* node, int dir) {
    // Function helper to get access various direction. Returns pointer node
    switch(dir) {
        case 0: return node->left;
        case 1: return node->right;
        case 2: return node->up;
        case 3: return node->down;
        case 4: return node->upleft;
        case 5: return node->upright;
        case 6: return node->downleft;
        case 7: return node->downright;
        default: return NULL; // Return NULL if direction is invalid
    }
}

// Author: Ihsan
void freeRow(NodeOctuple *col) {
    if (col == NULL) return;
    freeRow(col->right);
    free(col);
}

// Author: Ihsan
void freeBoard(NodeOctuple *row) {
    if (row == NULL) return;
    freeBoard(row->down);
    freeRow(row);
}
