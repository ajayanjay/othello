#ifndef score_h
#define score_h

#include "../datastructure/octuple.h"
#include "player.h"

#define SCORE_FILE "gamedata/scores.dat"

// Difficulty weight multipliers for scoring
#define HUMAN_WEIGHT 1.0f
#define AI_EASY_WEIGHT 1.2f
#define AI_MEDIUM_WEIGHT 1.5f
#define AI_HARD_WEIGHT 2.0f

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
int calculatePieces(NodeOctuple *board, char player);

// Calculate weighted score based on player type and piece count
int calculateWeightedScore(int pieceCount, PlayerType playerType);

// Get difficulty weight for a player type
float getDifficultyWeight(PlayerType playerType);

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