#include "resources.h"



void LoadGameAssets(GameAssets& assets) {
    assets.bgMainMenu = LoadTexture("resources\\bgMainMenu.png");
    assets.btnNormal = LoadTexture("resources\\Button\\scrollButton.png");
    assets.btnPressed = LoadTexture("resources\\Button\\scrollButton.png");
    assets.gameFont = LoadFont("resources\\font\\Jersey10-Regular.ttf");
    assets.gameLogo = LoadTexture("resources\\logo.png");
}

void UnloadGameAssets(GameAssets& assets) {
    UnloadTexture(assets.bgMainMenu);
    UnloadTexture(assets.btnNormal);
    UnloadTexture(assets.btnPressed);
    UnloadFont(assets.gameFont);
    UnloadTexture(assets.gameLogo);
}