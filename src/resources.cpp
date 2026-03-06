#include "resources.h"



void LoadGameAssets(GameAssets& assets) {
    assets.bgMainMenu = LoadTexture("resources\\test4.png");
    assets.btnNormal = LoadTexture("resources\\Button\\Rect\\Fill\\Hover@0.5x.png");
    assets.btnPressed = LoadTexture("resources\\Button\\Rect\\Fill\\Default@0.5x.png");
    assets.gameFont = LoadFont("resources\\font\\JAPANESE_2020.ttf");
}

void UnloadGameAssets(GameAssets& assets) {
    UnloadTexture(assets.bgMainMenu);
    UnloadTexture(assets.btnNormal);
    UnloadTexture(assets.btnPressed);
    UnloadFont(assets.gameFont);
}