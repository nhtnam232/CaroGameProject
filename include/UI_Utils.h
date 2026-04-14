#ifndef UI_UTILS
#define UI_UTILS

#include "raylib.h"
#include<vector>
#include<string.h>
#include<string>
#include"resources.h"
#include"GameLogic.h"
struct Button {
    Rectangle rect;
    const char* text;
};
//Color theme
//COLOR helpers
Color COLOR(unsigned char r, unsigned char g, unsigned char b);
Color COLORA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

const Color ColorBrownDark = COLORA(16, 8, 2, 248);
const Color ColorBrownRust = COLORA(75, 36, 7, 225);
const Color ColorBrownMedium = COLORA(30, 15, 5, 200);
const Color ColorBronzeSolid = COLOR(195, 115, 18);
const Color ColorBronzeAlpha = COLORA(195, 115, 18, 155);

const Color ColorGoldSolid = COLOR(250, 185, 48);
const Color ColorGoldAlpha = COLORA(250, 185, 48, 175);
const Color ColorOrangeSolid = COLOR(252, 155, 0);
const Color ColorCreamSolid = COLOR(255, 230, 160);
const Color ColorGoldAlpha50 = COLORA(250, 185, 48, 50);

const Color ColorShadow = COLORA(8, 4, 0, 115);
const Color ColorHighlight = COLORA(255, 255, 255, 25);
const Color ColorDeepBlueSolid = COLOR(30, 51, 255);
const Color ColorPinkNeon = COLOR(255, 50, 180);
const Color ColorRedNeon = COLORA(255, 50, 50, 255);
const Color ColorBlueNeon = COLOR(0, 170, 255);
const Color ColorCyanSolid = COLOR(100, 220, 255);
const Color ColorLightGreenSolid = COLOR(150, 255, 100);
const Color ColorLightGreen = COLORA(50, 255, 50, 255);
const Color ColorLightYellow = COLORA(255, 200, 0, 255);
//Glowing horizontal line
void DrawGlowLine(float x1, float y, float x2, Color col);
//Diamond shape
void DrawDiamond(float x, float y, float r, Color c);
//Ornate panel border with corner brackets
void DrawOrnateFrame(float x, float y, float w, float h, Color border, Color inner);
//Fancy slider
void DrawFancySlider(float x, float y, float w, float val, bool active);
//Music note icon
void DrawMusicNoteIcon(float x, float y, Color c);
//Speaker icon
void DrawSpeakerIcon(float x, float y, Color c, float vol);
//Side scroll banner
void DrawSideBanner(float x, float topY, float botY, const GameAssets& assets);
//Screen corner ornament
void DrawCornerOrnament(float x, float y, float dx, float dy, Color c);
//Draw Member Card
void DrawCard(Font font, float x, float y, const char* id, const char* name, const char* role, int skillLevel, Color borderColor);
//Draw base menu for using in any page (DRY : Don't Repeat Yourself)
Rectangle DrawBaseMenu(int screenWidth, int screenHeight, const GameAssets& assets, const char* title);
//Draw hint
void DrawHint(int screenWidth, int screenHeight, float hintX, float hintY, float hintW, const char* hint, const GameAssets& assets);
//Draw flip card used in selected mode
void DrawFlipCard(float x, float y, float width, float height, const char* title, const char* description, Color accentColor, const GameAssets& assets, bool isSelected, bool isFlipped, float scaleX);
//Draw step mode for user select game mode
void DrawStepMode(int screenWidth, int screenHeight, const GameAssets& assets, int& selectedSlot, bool& backToMenu);
//Draw page for choose opponent
void DrawStepOpponent(int screenWidth, int screenHeight, const GameAssets& assets, int& selectedSlot, bool& backToMenu);
//Draw Utils function for user input
void DrawInputBox(float x, float y, float w, float h, const char* text, bool isActive);
//Draw Page input player name
void DrawStepEnterName(int screenWidth, int screenHeight, const GameAssets& assets, int opponentMode, char* p1Name, int& count1, char* p2Name, int& count2, int& activeField);
//Draw Playing Screen

struct PlayerInfo {
    char name[16] = "\0";
    int score = 0;
    char symbol;
    Color color;
    bool isBot = false;
};
void DrawPauseOverlay(int sw, int sh, const MatchManager& match, const GameAssets& assets);
void DrawMatchScreen(int screenWidth, int screenHeight, const MatchManager& match, const GameAssets& assets, const std::vector<PlayerInfo>& players);
void DrawCaroBoard(const MatchManager& match, float boardX, float boardY);
void DrawWinnerDisplay(int screenWidth, float panelY, int winnerIdx, const std::vector<PlayerInfo>& players, const GameAssets& assets, float winTimer);
#endif 