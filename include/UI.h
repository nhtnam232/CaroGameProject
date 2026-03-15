#ifndef UI_H
#define UI_H
#include "raylib.h"
#include <vector>
#include "resources.h"
#include"UI_Utils.h"






void DrawMainMenu(int screenWidth, int screenHeight, int selectedIndex, const GameAssets& assets);
void DrawSettingsMenu(int screenWidth, int screenHeight, GameSettings& settings, const GameAssets& assets, int& selectedOption, bool& backToMenu);
void UpdateSettings(GameSettings& settings, int selectedOption, Vector2 mousePoint, bool mousePressed);
void DrawAboutMenu(int screenWidth, int screenHeight, const GameAssets& assets, int selectedIndex);
void DrawHelpMenu(int screenWidth, int screenHeight, const GameAssets& assets, int selectedIndex);



#endif // UI_H
