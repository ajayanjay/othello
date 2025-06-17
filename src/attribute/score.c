#include "../../include/attribute/score.h"
#include "../../include/attribute/player.h"
#include "../../include/util/menu.h"
#include "../../include/attribute/piece.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern Player gBlackPlayer;
extern Player gWhitePlayer;

static Scoreboard scoreboard = {0};

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
    
    int i;
    for (i = 0; i < scoreboard.playerCount; i++) {
        printf("%2d.   %-3s   %3d\n", 
               i + 1,
               scoreboard.scores[i].playerName,
               scoreboard.scores[i].highScore);
    }
    
    printf("================\n");
    printf("\nPress ENTER to return to the main menu...\n");
    inputUntilEnter();
}

int calculatePieces(NodeOctuple *board, char player) {
    if (board == NULL) return 0;
    
    int pieceCount = 0;
    NodeOctuple *row = board;
    
    // Count pieces for the specified player
    int i;
    for (i = 0; i < 8 && row != NULL; i++) {
        NodeOctuple *col = row;
        int j;
        for (j = 0; j < 8 && col != NULL; j++) {
            if (col->info == player) {
                pieceCount++;
            }
            col = col->right;
        }
        row = row->down;
    }
    
    return pieceCount;
}

float getDifficultyWeight(PlayerType playerType) {
    switch (playerType) {
        case HUMAN:
            return HUMAN_WEIGHT;
        case AI_EASY:
            return AI_EASY_WEIGHT;
        case AI_MEDIUM:
            return AI_MEDIUM_WEIGHT;
        case AI_HARD:
            return AI_HARD_WEIGHT;
        default:
            return HUMAN_WEIGHT;
    }
}

int calculateWeightedScore(int pieceCount, PlayerType playerType) {
    if (pieceCount <= 0) return 0;
    
    float weight = getDifficultyWeight(playerType);
    return (int)(pieceCount * weight);
}

void printCurrentScores(NodeOctuple *board) {
    if (board == NULL) return;
    
    int blackPieces = calculatePieces(board, BLACK);
    int whitePieces = calculatePieces(board, WHITE);
    
    // Calculate weighted scores for both players
    int blackWeightedScore = calculateWeightedScore(blackPieces, gWhitePlayer.type);
    int whiteWeightedScore = calculateWeightedScore(whitePieces, gBlackPlayer.type);
    
    // Convert PlayerType enum to string
    const char* blackType = playerTypeToString(gBlackPlayer.type);
    const char* whiteType = playerTypeToString(gWhitePlayer.type);
    
    printf("\n        Scores:\n"
        " "
        "Black (X) | White (O)\n "
        "%9s | %-9s\n "
        "%2d pieces | %d pieces\n "
        "%9d | %-9d\n",
        blackType, whiteType, blackPieces, whitePieces, blackWeightedScore, whiteWeightedScore
        );
}

void getPlayerName(char* playerName) {
    printf("Enter your name (3 characters): ");
    inputLimitedString(playerName, 1, 3, isalpha, toupper);
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
    // Simple bubble sort by weighted score (descending)
    int i;
    for (i = 0; i < scoreboard.playerCount - 1; i++) {
        int j;
        for (j = 0; j < scoreboard.playerCount - i - 1; j++) {
            if (scoreboard.scores[j].highScore < scoreboard.scores[j + 1].highScore) {
                // Swap scores
                PlayerScore temp = scoreboard.scores[j];
                scoreboard.scores[j] = scoreboard.scores[j + 1];
                scoreboard.scores[j + 1] = temp;
            }
        }
    }
}