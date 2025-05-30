#include "score.h"
#include "menu.h"
#include "piece.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static Scoreboard scoreboard = {0};
static const char* SCORE_FILE = "scores.dat";

void initScore() {
    scoreboard.playerCount = 0;
    loadScoreboard();
}

void printScoreboard() {
    clearScreen();
    
    printf("== SCOREBOARD ==\n\n");
    
    if (scoreboard.playerCount == 0) {
        printf("No high scores yet!\n\n");
        printf("Press ENTER to return to main menu...\n");
        inputUntilEnter();
        return;
    }
    
    // Sort scoreboard by high score (descending)
    sortScoreboard();
    
    printf("RANK  NAME  SCORE\n");
    printf("================\n");
    
    for (int i = 0; i < scoreboard.playerCount; i++) {
        printf("%2d.   %s   %3d\n", 
               i + 1,
               scoreboard.scores[i].playerName,
               scoreboard.scores[i].highScore);
    }
    
    printf("================\n");
    printf("\nPress ENTER to return to the main menu...\n");
    inputUntilEnter();
}

int calculateScore(NodeOctuple *board, char player) {
    if (board == NULL) return 0;
    
    int pieceCount = 0;
    NodeOctuple *row = board;
    
    // Count pieces for the specified player
    for (int i = 0; i < 8 && row != NULL; i++) {
        NodeOctuple *col = row;
        for (int j = 0; j < 8 && col != NULL; j++) {
            if (col->info == player) {
                pieceCount++;
            }
            col = col->right;
        }
        row = row->down;
    }
    
    return pieceCount;
}

void printCurrentScores(NodeOctuple *board) {
    if (board == NULL) return;
    
    int blackScore = calculateScore(board, BLACK);
    int whiteScore = calculateScore(board, WHITE);
    
    printf("\n   Scores:\n");
    printf("   X: %2d pieces\n", blackScore);
    printf("   O: %2d pieces\n", whiteScore);
}

void getPlayerName(char* playerName) {
    printf("Enter your 3-character name: ");
    
    char input[10];
    while (1) {
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // Remove newline if present
            input[strcspn(input, "\n")] = 0;
            
            // Check input chars
            if (strlen(input) == 3) {
                // Convert to uppercase and copy
                for (int i = 0; i < 3; i++) {
                    playerName[i] = toupper(input[i]);
                }
                playerName[3] = '\0';
                break;
            }
        }
        printf("Enter your name (3 characters): ");
    }
}

void addHighScore(const char* playerName, int score) {
    // Check if the score qualifies for the high score list
    if (scoreboard.playerCount < 10) {
        // Empty room available
        strcpy(scoreboard.scores[scoreboard.playerCount].playerName, playerName);
        scoreboard.scores[scoreboard.playerCount].highScore = score;
        scoreboard.playerCount++;
    } else {
        // Score is higher than the lowest score
        sortScoreboard();
        if (score > scoreboard.scores[9].highScore) {
            // Replace the lowest score
            strcpy(scoreboard.scores[9].playerName, playerName);
            scoreboard.scores[9].highScore = score;
        }
    }
    
    saveScoreboard();
}

void saveScoreboard() {
    FILE* file = fopen(SCORE_FILE, "wb");
    if (file == NULL) {
        return;
    }
    
    fwrite(&scoreboard, sizeof(Scoreboard), 1, file);
    fclose(file);
}

void loadScoreboard() {
    FILE* file = fopen(SCORE_FILE, "rb");
    if (file == NULL) {
        // File doesn't exist yet, start with empty scoreboard
        scoreboard.playerCount = 0;
        return;
    }
    
    size_t result = fread(&scoreboard, sizeof(Scoreboard), 1, file);
    fclose(file);
    
    if (result != 1) {
        // File corrupted or incomplete, start fresh
        scoreboard.playerCount = 0;
    }
}

void sortScoreboard() {
    // Simple bubble sort by high score (descending)
    for (int i = 0; i < scoreboard.playerCount - 1; i++) {
        for (int j = 0; j < scoreboard.playerCount - i - 1; j++) {
            if (scoreboard.scores[j].highScore < scoreboard.scores[j + 1].highScore) {
                // Swap scores
                PlayerScore temp = scoreboard.scores[j];
                scoreboard.scores[j] = scoreboard.scores[j + 1];
                scoreboard.scores[j + 1] = temp;
            }
        }
    }
}