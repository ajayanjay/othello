#ifndef score_h
#define score_h

#include "../datastructure/octuple.h"

typedef struct {
    char playerName[4]; // 3 characters + null terminator
    int highScore;      // Highest piece count achieved
} PlayerScore;

typedef struct {
    PlayerScore scores[10]; // Top 10 players
    int playerCount;
} Scoreboard;


void initScore();

// Print scoreboard
void printScoreboard();

// Calculate current score based on piece count
int calculateScore(NodeOctuple *board, char player);

// Add a high score to the scoreboard if it qualifies
void addHighScore(const char* playerName, int score);

// Print current game scores during gameplay
void printCurrentScores(NodeOctuple *board);

// Get player name input (3 uppercase characters)
void getPlayerName(char* playerName);

// Save scoreboard to file
void saveScoreboard();

// Load scoreboard from file
void loadScoreboard();

void sortScoreboard();

#endif