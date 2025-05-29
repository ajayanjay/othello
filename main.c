#include <stdlib.h>
#include <stdio.h>
#include "menu.h"
#include "game.h"
#include "piece.h"

void mainMenu();
void howToPlay();

void test() {
    NodeOctuple * root = NULL;
    constructOthelloBoard(&root);
    char player = BLACK;
    while (1) {
        Move move = inputMove(root, player);

        if (move.x == -1 && move.y == -1) {
            printf("Game Over!\n");
            inputUntilEnter();
            freeBoard (root);
            break;
        }

        makeMove(root, &move, player);
        player = (player == BLACK) ? WHITE : BLACK;
    }
}

int main(void)
{
    test();
    return 0;
}

void mainMenu()
{
    const char* menuHeader =    "Welcome to Othello\n\n";
    const char* menuItems[] = {
                                "New Game\n",
                                "Continue\n",
                                "Watch Replay\n",
                                "Scoreboard\n",
                                "How To Play\n",
                                "Exit\n\n",
                                NULL
    };
    const char* menuFooter =    "Press ENTER to Select...";

    while (1) // Loop the game until the user exits.
        switch (menu(menuHeader, menuItems, menuFooter)) {
            case 0: //newGame(); break;
            case 1: //continue(); break;
            case 2: //watchReplay(); break;
            case 3: //scoreboard(); break;
            case 4: howToPlay(); break;
            case 5: return;
        }
}

void howToPlay()
{
    clearScreen();
    printf(
        "How to Play Othello:\n"
        "1. The game is played on an 8x8 board.\n"
        "2. Players take turns placing their pieces on the board.\n"
        "3. A player can capture the opponent's pieces by enclosing them.\n"
        "4. The game ends when the board is full or no more moves are possible.\n"
        "5. The player with the most pieces on the board wins.\n"
        "Press ENTER to return to the main menu...\n"
    );
    inputUntilEnter();
}