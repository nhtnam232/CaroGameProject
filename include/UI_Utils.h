#ifndef UI_UTILS
#define UI_UTILS

#include "raylib.h"
#include<vector>
#include"resources.h"

struct Button {
    Rectangle rect;
    const char* text;
};

struct Slider {
    Rectangle barRect;
    Rectangle knobRect;
    float value;
    bool isDragging;
    const char* label;
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
const Color ColorBlueNeon = COLOR(0, 170, 255);
const Color ColorCyanSolid = COLOR(100, 220, 255);
const Color ColorLightGreenSolid = COLOR(150, 255, 100);
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
void DrawSideBanner(float x, float topY, float botY, bool flipText, const GameAssets& assets);
//Screen corner ornament
void DrawCornerOrnament(float x, float y, float dx, float dy, Color c);
//Draw Member Card
void DrawCard(Font font, float x, float y, const char* id, const char* name, const char* role, int skillLevel, Color borderColor);
//Draw base menu for using in any page (DRY : Don't Repeat Yourself)
Rectangle DrawBaseMenu(int screenWidth, int screenHeight, const GameAssets& assets, const char* title);
//Draw hint
void DrawHint(int screenWidth, int screenHeight, float hintX, float hintY, float hintW, const char* hint, const GameAssets& assets);
#endif 