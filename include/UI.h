#ifndef UI_H
#include "raylib.h"
#include<vector>
#include"resources.h"
struct Button {
    Rectangle rect;
    const char* text;
};
void DrawMainMenu(int screenWidth, int screenHeight, int selectedIndex, const GameAssets& assets);
void DrawSettingsMenu(int screenWidth, int screenHeight, GameSettings& settings, const GameAssets& assets, int& selectedOption, bool& backToMenu);
void UpdateSettings(GameSettings& settings, int selectedOption, Vector2 mousePoint, bool mousePressed);


#define UI_H
#endif 
