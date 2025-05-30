#include <stdlib.h>
#include <stdio.h>
#include "menu.h"
#include "game.h"
#include "piece.h"
#include <time.h>

void mainMenu();
void selectMode();
void howToPlay();

int main(void)
{
    srand((unsigned int)time(NULL));
    mainMenu();
    //test();
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
            case 0: selectMode(); break;
            case 1: //continue(); break;
            case 2: //watchReplay(); break;
            case 3: //scoreboard(); break;
            case 4: howToPlay(); break;
            case 5: return;
        }
}

// Author: Ihsan
void selectMode() {
    const char* player1Header = "Select Player 1\n\n";
    const char* player2Header = "Select Player 2\n\n";
    const char* playerOptions[] = {
        "Human\n",
        "Easy AI\n",
        "Medium AI\n",
        "Hard AI\n",
        "Replay\n",
        "Back\n\n",
        NULL
    };
    const char* modeSelector = "Press Enter to Select...";

    PlayerType player1, player2;
    int player1Selection;

    while (1) {
        // Select Player 1
        player1Selection = menu(player1Header, playerOptions, modeSelector);
        
        // Handle Back option
        if (player1Selection == 5) {
            return;
        }
        
        // Map selection to PlayerType
        switch (player1Selection) {
            case 0: player1 = HUMAN; break;
            case 1: player1 = AI_EASY; break;
            case 2: player1 = AI_MEDIUM; break;
            case 3: player1 = AI_HARD; break;
            case 4: player1 = REPLAY; break;
            default: continue;
        }
        
        // Select Player 2
        while (1) {
            int player2Selection = menu(player2Header, playerOptions, modeSelector);
            
            // Handle Back option (return to Player 1 selection)
            if (player2Selection == 5) {
                break;
            }
            
            // Map selection to PlayerType
            switch (player2Selection) {
                case 0: player2 = HUMAN; break;
                case 1: player2 = AI_EASY; break;
                case 2: player2 = AI_MEDIUM; break;
                case 3: player2 = AI_HARD; break;
                case 4: player2 = REPLAY; break;
                default: continue; // Invalid selection, try again
            }
            
            // Init stack dan queue
            Stack stackRedo;
            Deque dequeUndo;
            initStack(&stackRedo, sizeof(Move), 64);
            initDeque(&dequeUndo);
            char startingPlayer = BLACK;
            // Start the game with selected player types
            game(player1, player2, &stackRedo, &dequeUndo, startingPlayer);
            return; // Return to main menu after game ends
        }
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