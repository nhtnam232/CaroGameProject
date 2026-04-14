#ifndef UI_H
#define UI_H
#include "raylib.h"
#include "resources.h"
#include"UI_Utils.h"






void DrawMainMenu(int screenWidth, int screenHeight, int selectedIndex, const GameAssets& assets);
void DrawSettingsMenu(int screenWidth, int screenHeight, GameSettings& settings, const GameAssets& assets, int& selectedOption, bool& backToMenu);
void UpdateSettings(GameSettings& settings, int selectedOption, Vector2 mousePoint, bool mousePressed);
void DrawAboutMenu(int screenWidth, int screenHeight, const GameAssets& assets, int selectedIndex);
void DrawHelpMenu(int screenWidth, int screenHeight, const GameAssets& assets, int selectedIndex);
void DrawNewGameFlow(int screenWidth, int screenHeight, const GameAssets& assets, bool& backToMenu, bool& startMatch, std::vector<PlayerInfo> &players, MatchManager& match);


#endif // UI_H
