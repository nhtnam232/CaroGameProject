#ifndef RESOURCES_H
#include "raylib.h"


struct GameAssets {
    Texture2D bgMainMenu; 
    Texture2D btnNormal;
    Texture2D btnPressed;
    Font gameFont;
    Texture2D gameLogo;
    //More resources
};

void LoadGameAssets(GameAssets& assets);
void UnloadGameAssets(GameAssets& assets);

#define RESOURCES_H
#endif // !RESOURCES_H
