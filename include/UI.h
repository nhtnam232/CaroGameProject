#ifndef UI_H
#include "raylib.h"
#include<vector>
#include"resources.h"
struct Button {
    Rectangle rect;
    const char* text;
};


void DrawMainMenu(int screenWidth, int screenHeight, int selectedIndex, const GameAssets& assets);


#define UI_H
#endif 
