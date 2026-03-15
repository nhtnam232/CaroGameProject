#pragma once
#pragma once
#include "raylib.h"
#include "resources.h"
#include <string>


enum NewGameSubState {
    SUB_MODE_SELECT, SUB_OPPONENT_SELECT, SUB_NAME_INPUT,
    SUB_READY, SUB_PLAYING, SUB_WIN_BLINK, SUB_WIN
};
enum GameMode { MODE_STANDARD = 0, MODE_BLOCK2 = 1, MODE_FREE = 2 };
enum OpponentType { OPP_AI = 0, OPP_AI_HARD = 1, OPP_PVP = 2 };

static const int   BOARD_SIZE = 15;
static const int   CELL = 36;
static const float TURN_TIME = 30.f;
static const float BLINK_DURATION = 2.4f;
static const int   MAX_PARTICLES = 300;

struct CardDef {
    const char* category;
    const char* icon;
    const char* title;
    Color       accent;
    const char* lines[5];
    int         lineCount;
};

struct Particle {
    float x, y, vx, vy, life, maxLife, size;
    Color color;
};

struct WinLine { int x0, y0, x1, y1; };

struct GameSession {
    NewGameSubState sub = SUB_MODE_SELECT;
    GameMode        mode = MODE_STANDARD;
    OpponentType    opp = OPP_PVP;

    char        name1[32] = "Player 1";
    char        name2[32] = "Player 2";
    int         nameStep = 0;
    std::string nameBuf = "";

    float readyTimer = 0.f;

    int   board[BOARD_SIZE][BOARD_SIZE] = {};
    int   curP = 1, curX = 7, curY = 7;
    int   winner = 0;
    float turnTimer = TURN_TIME;

    bool  paused = false;
    int   pauseSel = 0;

    float   blinkTimer = 0.f;
    WinLine winLine = {};

    float    winTimer = 0.f;
    Particle particles[MAX_PARTICLES] = {};
    int      particleCount = 0;
    int      winDisplaySel = 0;

    int   mSel = 0, mTgt = 0, mSlot = 0;
    float mFlip = 1.f;
    bool  mMid = false;

    int   oSel = 0, oTgt = 0, oSlot = 0;
    float oFlip = 1.f;
    bool  oMid = false;

    int nameSlot = 0;
};


void ResetNewGame();
void DrawNewGameFlow(int screenWidth, int screenHeight,
const GameAssets& assets, bool& backToMenu);