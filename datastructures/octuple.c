#include <stdio.h>
#include <stdlib.h>
#include "octuple.h"
#include "../piece.h"

NodeOctuple * createNodeOctuple (OctupleInfo info){
    NodeOctuple * new_node = malloc (sizeof(NodeOctuple));
    new_node->info = info;
    new_node->up = NULL; 
    new_node->down = NULL;
    new_node->left = NULL; 
    new_node->right = NULL;
    new_node->upleft = NULL; 
    new_node->upright = NULL;
    new_node->downleft = NULL; 
    new_node->downright = NULL; 
    return new_node;
}

void constructOthelloBoard(NodeOctuple **root) {
    /*
    create octuple linked list for board othello 8x8
    */

    // Init first node
    (*root) = createNodeOctuple(BLANK);
    NodeOctuple *current = *root;

    int i = 0; //init first line and connect left right
    while (i < 7) {
        NodeOctuple *new_node = createNodeOctuple(BLANK);
        current->right = new_node;
        new_node->left = current;
        current = new_node; //update current
        i++;
    }

    // save row before
    NodeOctuple *prev_row_start = *root;

    int row = 0;
    while (row < 7) {
        NodeOctuple *firstrow = createNodeOctuple(BLANK);
        current = firstrow;

        // connect first node to up and down
        current->up = prev_row_start;
        if (prev_row_start != NULL) {
            prev_row_start->down = current;
            // Set diagonal (upright & downleft) for first node
            if (prev_row_start->right != NULL) {
                current->upright = prev_row_start->right;
                prev_row_start->right->downleft = current;
            }
        }

        NodeOctuple *prev_current = prev_row_start; //create current for traversal in prev row
        i = 0;
        while (i < 7) {
            // Set infotype based on position (especially middle set rule othello)
            char color = '.';
            if (row == 2 && i == 2) color = WHITE; // White on row 3, col 3 (d4)
            else if (row == 2 && i == 3) color = BLACK; // Black on row 3, col 4 (e4)
            else if (row == 3 && i == 2) color = BLACK; // Black on row 4, col 3 (d5)
            else if (row == 3 && i == 3) color = WHITE; // White on row 4, col 4 (e5)

            NodeOctuple *new_node = createNodeOctuple(color);
            // connect left and right 
            current->right = new_node;
            new_node->left = current;

            // Connect up and down (ensure not node in right end)
            if (prev_current != NULL && prev_current->right != NULL) {
                new_node->up = prev_current->right; // because new_node in right, not straight from prev current
                // not straight from prev current
                prev_current->right->down = new_node;
            }

            // diagonal upleft and upright
            if (new_node->up != NULL) {
                if (new_node->up->left != NULL) {
                    new_node->upleft = new_node->up->left;
                }
                if (i < 6 && new_node->up->right != NULL) { // Ensure j+1 < 8
                    new_node->upright = new_node->up->right;
                    new_node->up->right->downleft = new_node; // Set reverse pointer
                }
            }

            // diagonal downright and downleft
            if (prev_current != NULL) {
                if (current != NULL) {
                    prev_current->downright = new_node;
                    new_node->upleft = prev_current; // Set reverse pointer
                }
                if (current->left != NULL) {
                    prev_current->downleft = current->left;
                    current->left->upright = prev_current; // Set reverse pointer
                } else {
                    prev_current->downleft = NULL;
                }
            }

            current = new_node;
            if (prev_current != NULL) {
                prev_current = prev_current->right; //update prev_current
            }
            i++;
        }

        row++;
        prev_row_start = firstrow; //update prev_row_start
    }
}

void printBoard(NodeOctuple *board) {
    char buffer[1024];
    int offset = 0;

    // Baris atas
    offset += sprintf(buffer + offset, "  +-----------------+\n"); //border top

    NodeOctuple *current_row = board;
    int row_number = 1;

    while (current_row != NULL && row_number <= 8) {
        offset += sprintf(buffer + offset, "%d |", row_number); //print row number and border
        NodeOctuple *current_col = current_row;
        int i = 0;
        while (i < 8 && current_col != NULL) {
            offset += sprintf(buffer + offset, " %c", current_col->info); // print info node
            current_col = current_col->right; // update current to right
            i++;
        }
        offset += sprintf(buffer + offset, " |\n");
        current_row = current_row->down; // update current to bottom
        row_number++; //update row number
    }

    offset += sprintf(buffer + offset, "  +-----------------+\n"); //border bottom
    offset += sprintf(buffer + offset, "    A B C D E F G H\n");

    // Print all
    printf("%s", buffer);
}

NodeOctuple * setNodeAt (NodeOctuple *root, OctupleInfo info,  int row, int col){
    NodeOctuple *current= root;
    int i=0;
    while (i<row){
        current = current->down;
        i++;
    }
    i=0;
    while (i<col){
        current = current->right;
    }
    current->info = info;
}


NodeOctuple * getNodeAt(NodeOctuple *root, int row, int col) {
    NodeOctuple *current = root;
    for (int i = 0; i < row; ++i)
        current = current->down;
    for (int j = 0; j < col; ++j)
        current = current->right;
    return current;
}

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

void saveBoard(NodeOctuple *root, const char *filename) {
    /*
    Save the octuple linked list board to a text file in 8x8 grid format
    Each line contains 8 characters ('X', 'O', or '.') followed by a newline
    */
    if (root == NULL) {
        return;
    }

    FILE *file = fopen(filename, "w");
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

int loadBoard(NodeOctuple **root, const char *filename) {
    /*
    Load the octuple linked list board from a text file in 8x8 grid format
    Each line contains 8 characters ('X', 'O', or '.') followed by a newline
    */
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0; // Failed to open file
    }

    // Read the board into a 2D array first for easier processing
    char boardArray[8][8];
    char line[10]; // Buffer for reading lines (8 chars + newline + null terminator)
    int row_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && row_count < 8) {
        // Skip empty lines
        if (line[0] == '\n') continue;
        
        // Validate line has exactly 8 characters
        int char_count = 0;
        while (char_count < 8 && line[char_count] != '\n' && line[char_count] != '\0') {
            if (line[char_count] == 'X' || line[char_count] == 'O' || line[char_count] == '.') {
                boardArray[row_count][char_count] = line[char_count];
                char_count++;
            } else {
                fclose(file);
                return 0; // Invalid character
            }
        }
        
        if (char_count != 8) {
            fclose(file);
            return 0; // Invalid line length
        }
        
        row_count++;
    }
    
    fclose(file);
    
    if (row_count != 8) {
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
        NodeOctuple *new_node = createNodeOctuple(boardArray[0][j]);
        current->right = new_node;
        new_node->left = current;
        current = new_node;
    }

    NodeOctuple *prev_row_start = *root;

    // Create remaining rows with full octuple connections
    for (int i = 1; i < 8; i++) {
        NodeOctuple *first_node = createNodeOctuple(boardArray[i][0]);
        current = first_node;

        // Connect up-down for first node
        current->up = prev_row_start;
        prev_row_start->down = current;

        // Set diagonal connections for first node
        if (prev_row_start->right != NULL) {
            current->upright = prev_row_start->right;
            prev_row_start->right->downleft = current;
        }

        NodeOctuple *prev_current = prev_row_start;
        
        // Create rest of the row
        for (int j = 1; j < 8; j++) {
            NodeOctuple *new_node = createNodeOctuple(boardArray[i][j]);
            
            // Connect left-right
            current->right = new_node;
            new_node->left = current;

            // Connect up-down
            if (prev_current->right != NULL) {
                new_node->up = prev_current->right;
                prev_current->right->down = new_node;
            }

            // Set diagonal connections
            if (new_node->up != NULL) {
                // upleft connection
                if (new_node->up->left != NULL) {
                    new_node->upleft = new_node->up->left;
                    new_node->up->left->downright = new_node;
                }
                // upright connection
                if (j < 7 && new_node->up->right != NULL) {
                    new_node->upright = new_node->up->right;
                    new_node->up->right->downleft = new_node;
                }
            }

            // downleft connection for previous node in previous row
            if (prev_current != NULL && current != NULL) {
                prev_current->downright = new_node;
                new_node->upleft = prev_current;
            }

            current = new_node;
            if (prev_current != NULL) {
                prev_current = prev_current->right;
            }
        }

        prev_row_start = first_node;
    }

    return 1; // Board loaded successfully
}