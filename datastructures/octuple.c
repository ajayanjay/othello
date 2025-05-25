#include <stdio.h>
#include <stdlib.h>
#include "octuple.h"

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
    (*root) = createNodeOctuple('.');
    NodeOctuple *current = *root;

    int i = 0; //init first line and connect left right
    while (i < 7) {
        NodeOctuple *new_node = createNodeOctuple('.');
        current->right = new_node;
        new_node->left = current;
        current = new_node; //update current
        i++;
    }

    // save row before
    NodeOctuple *prev_row_start = *root;

    int row = 0;
    while (row < 7) {
        NodeOctuple *firstrow = createNodeOctuple('.');
        current = firstrow;

        // connect first node to up and down
        current->up = prev_row_start;
        if (prev_row_start != NULL) {
            prev_row_start->down = current;
        }

        NodeOctuple *prev_current = prev_row_start; //create current for traversal in prev row
        i = 0;
        while (i < 7) {
            // Set infotype based on position (especially middle set rule othello)
            char color = '.';
            if (row == 2 && i == 2) color = 'O'; // White on row 3, col 3 (d4)
            else if (row == 2 && i == 3) color = 'X'; // Black on row 3, col 4 (e4)
            else if (row == 3 && i == 2) color = 'X'; // Black on row 4, col 3 (d5)
            else if (row == 3 && i == 3) color = 'O'; // White on row 4, col 4 (e5)

            NodeOctuple *new_node = createNodeOctuple(color);
            // connect left and right 
            current->right = new_node;
            new_node->left = current;

            // Connect up and down (ensure not node in right end)
            if (prev_current != NULL && prev_current->right != NULL) {
                new_node->up = prev_current->right; // because new_node in right, not straight from prev current
                prev_current->right->down = new_node;
            }

            // diagonal upleft and upright
            if (new_node->up != NULL) {
                if (new_node->up->left != NULL) {
                    new_node->upleft = new_node->up->left;
                }
                if (new_node->up->right != NULL) {
                    new_node->upright = new_node->up->right;
                }
            }

            // diagonal downright and downleft
            if (prev_current != NULL) {
                if (current != NULL) {
                    prev_current->downright = new_node;
                }
                if (new_node->left != NULL) {
                    prev_current->downleft = current;
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

void printRow(NodeOctuple *row) {
    if (row == NULL) return;
    printf("%c", row->info);
    if (row->right != NULL) {
        printRow(row->right);
    }
}

void printBoard(NodeOctuple *board) {
    if (board == NULL) return;
    printRow(board);
    printf("\n");
    if (board->down != NULL) {
        printBoard(board->down);
    }
}

NodeOctuple * getNodeAt(NodeOctuple *root, int row, int col) {
    NodeOctuple *current = root;
    for (int i = 0; i < row; ++i)
        current = current->down;
    for (int j = 0; j < col; ++j)
        current = current->right;
    return current;
}