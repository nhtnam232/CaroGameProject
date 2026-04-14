#pragma once
#include"raylib.h"
enum GameMode { MODE_STANDARD = 0, MODE_BLOCK_2_ENDS = 1, MODE_RENJU = 2 };
const int BOARD_SIZE = 15;
const int CELL = 36;
struct MatchManager {
    int board[BOARD_SIZE][BOARD_SIZE] = { 0 };
    int cursorX = 7;
    int cursorY = 7;
    int currentPlayer = 1;
    int gameMode = MODE_STANDARD;
    bool isMatchOver = false;
    int winner = 0; // 0: no winner, 1: Player 1, 2: Player 2
    float winTimer = 0.0f; // Track time for victory animations
    bool isPaused = false;
    int pauseSelected = 0; // 0: Continue, 1: Save game, 2: Menu
    Vector2 flashingPieces[5]; // For drawing winingline
};


void UpdateMatchLogic(MatchManager& match, bool &backToMenu);
bool IsValidMove(int mode, int board[15][15], int x, int y, int player);
bool CheckWinCondition(int mode, int board[15][15], int x, int y, int player);