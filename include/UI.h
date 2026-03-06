#ifndef UI_H
#include "raylib.h"
#include<vector>
#include"resources.h"
struct Button {
    Rectangle rect;
    const char* text;
};


void DrawMainMenu(int screenWidth, int screenHeight, std::vector<Button> menuButtons, int selectedIndex, const GameAssets& assets);


#define UI_H
#endif 
