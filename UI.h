#ifndef UI_H
#define UI_H
#include "raylib.h"
#include <vector>
#include "resources.h"

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

void DrawMainMenu(int screenWidth, int screenHeight, int selectedIndex, const GameAssets& assets);
void DrawSettingsMenu(int screenWidth, int screenHeight, GameSettings& settings, const GameAssets& assets, int& selectedOption, bool& backToMenu);
void UpdateSettings(GameSettings& settings, int selectedOption, Vector2 mousePoint, bool mousePressed);

#endif // UI_H