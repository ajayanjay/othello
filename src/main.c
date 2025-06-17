#include "../include/util/menu.h"
#include "../include/game.h"
#include "../include/attribute/piece.h"
#include "../include/attribute/score.h"
#include "../include/replay.h"
#include "../include/util/storage.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void newGame();
void mainMenu();
void continueGame();
void selectMode();
void howToPlay();

int main(void)
{
    printf("\e[?25l");
    atexit(deleteTree);
    srand((unsigned int)time(NULL));
    createDirectory("gamedata");
    initScore();
    mainMenu();
    printf("\e[?25h");
    return 0;
}

void mainMenu()
{
    const char* menuHeader =    "Welcome to Othello\n\n";
    const char* menuItems[] = {
                                "New Game\n",
                                "Continue\n",
                                "Watch Replays\n",
                                "Scoreboard\n",
                                "How To Play\n\n",

                                "Exit\n\n",
                                NULL
    };
    const char* menuFooter =    "Press ENTER to Select...\n\n";

    while (1) // Loop the game until the user exits.
        switch (menu(menuHeader, menuItems, menuFooter)) {
            case 0: newGame(); break;
            case 1: continueGame(); break;
            case 2: replayMainMenu(); break;
            case 3: printScoreboard(); break;
            case 4: howToPlay(); break;
            case 5: return;
            default: return;
        }
}

void continueGame() {
    Player player1, player2;
    Stack stackRedo;
    Deque dequeUndo;
    NodeOctuple * board;
    char currentPlayer;

    // Load the last saved game
    if (loadGame(&board, &player1, &player2, &stackRedo, &dequeUndo, &currentPlayer)) {
        int gameResult = game(player1, player2, board, &stackRedo, &dequeUndo, currentPlayer);
        
        // Only ask to save replay if game finished normally (not escaped)
        if (gameResult == 0) {
            saveReplayMenu(&dequeUndo);
        }
        
        freeStack(&stackRedo);
        freeDeque(&dequeUndo);
        freeBoard(board);
    } else {
        printf("No saved game found. please create a new game first.\n");
        inputUntilEnter();
    }
}

void newGame() {
    if (isDirectoryExist(SAVEDATA_DIR)) {
        const char* header =    "You already have a saved game\n"
                                "Do you want to create a new game?\n\n";
        const char* options[] = {
                                "Yes\n",
                                "No\n\n", NULL
        };
        const char* footer =    "Press ENTER to Select...\n";
        switch (menu(header, options, footer)) {
            case 0: selectMode(); return;
            case 1: return;
            default: return;
        }
    }
    selectMode();
}

// Author: Ihsan
void selectMode() {
    const char* player1Header = "Select Player 1\n\n";
    const char* player2Header = "Select Player 2\n\n";
    const char* playerOptions[] = {
        "Human\n",
        "Easy AI\n",
        "Medium AI\n",
        "Hard AI\n\n",
        "Back\n\n",
        NULL
    };
    const char* modeSelector = "Press Enter to Select...";

    Player player1, player2;
    int player1Selection;

    while (1) {
        // Select Player 1
        player1Selection = menu(player1Header, playerOptions, modeSelector);
        
        // Map selection to PlayerType
        switch (player1Selection) {
            case 0: player1 = player(HUMAN, BLACK); break;
            case 1: player1 = player(AI_EASY, BLACK); break;
            case 2: player1 = player(AI_MEDIUM, BLACK); break;
            case 3: player1 = player(AI_HARD, BLACK); break;
            case 4: return;
            default: return;
        }
        
        // Select Player 2
        while (1) {
            int player2Selection = menu(player2Header, playerOptions, modeSelector);
            
            // Handle Back option (return to Player 1 selection)
            if (player2Selection == 4 || player2Selection < 0) {
                break;
            }
            
            // Map selection to PlayerType
            switch (player2Selection) {
                case 0: player2 = player(HUMAN, WHITE); break;
                case 1: player2 = player(AI_EASY, WHITE); break;
                case 2: player2 = player(AI_MEDIUM, WHITE); break;
                case 3: player2 = player(AI_HARD, WHITE); break;
                default: continue; // Invalid selection, try again
            }
            
            // Init stack dan queue dan board.
            Stack stackRedo;
            Deque dequeUndo;
            NodeOctuple * board;
            constructOthelloBoard(&board);
            initStack(&stackRedo, sizeof(Move), 64);
            initDeque(&dequeUndo);

            char startingPlayer = BLACK;
            // Start the game with selected player types
            int gameResult = game(player1, player2, board, &stackRedo, &dequeUndo, startingPlayer);
            
            // Only ask to save replay if game finished normally (not escaped)
            if (gameResult == 0) {
                saveReplayMenu(&dequeUndo);
            }

            freeStack(&stackRedo);
            freeDeque(&dequeUndo);
            freeBoard(board);
            
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