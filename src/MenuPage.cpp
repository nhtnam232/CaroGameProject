#include"UI.h"


void DrawMainMenu(int screenWidth, int screenHeight, int selectedIndex, const GameAssets& assets) {

    //Set up buttons
    int btnWidth = 184;
    int btnHeight = 84;

    float gap = 50.0f;
    int spacing = 90;

    float leftX = (screenWidth / 2.0f) - (gap / 2.0f) - btnWidth;
    float rightX = (screenWidth / 2.0f) + (gap / 2.0f);

    float startY = 300.0f;
    float row1Y = startY;                 
    float row2Y = startY + spacing;       
    float row3Y = startY + spacing * 2;   

    std::vector<Button> menuButtons = {
        { { leftX, row1Y, (float)btnWidth, (float)btnHeight }, "NEW GAME" },
        { { rightX, row1Y, (float)btnWidth, (float)btnHeight }, "LOAD GAME" },
        { { leftX, row2Y, (float)btnWidth, (float)btnHeight }, "SETTINGS" },
        { { rightX, row2Y, (float)btnWidth, (float)btnHeight }, "HELP" },
        { { leftX, row3Y, (float)btnWidth, (float)btnHeight }, "ABOUT US" },
        { { rightX, row3Y, (float)btnWidth, (float)btnHeight }, "EXIT" }
    };
    //Fix Background
    Rectangle sourceRec = { 0.0f, 0.0f, (float)assets.bgMainMenu.width, (float)assets.bgMainMenu.height };
    Rectangle destRec = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
    Vector2 origin = { 0.0f, 0.0f };
    DrawTexturePro(assets.bgMainMenu, sourceRec, destRec, origin, 0.0f, WHITE);
    
    //Draw title
    float logoX = (screenWidth - assets.gameLogo.width) / 2.0f;
    float logoY = -130.0f; 

    DrawTextureV(assets.gameLogo, { logoX, logoY }, WHITE);

    //Draw button
    for (int i = 0; i < (int)menuButtons.size(); i++) {
        bool isSelected = (i == selectedIndex);
        Texture2D tex = isSelected ? assets.btnPressed : assets.btnNormal;

        Rectangle sourceRec = { 0.0f, 0.0f, 111.0f, 42.0f };
        Rectangle destRec = menuButtons[i].rect;

        DrawTexturePro(tex, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);

        int fontSize = 35;
        float spacing = 1.0f;

        Vector2 textSize = MeasureTextEx(assets.gameFont, menuButtons[i].text, fontSize, spacing);

        float textX = menuButtons[i].rect.x + (menuButtons[i].rect.width - textSize.x) / 2.0f;
        float textY = menuButtons[i].rect.y + (menuButtons[i].rect.height - textSize.y) / 2.0f;

        Color textColor = isSelected ? RAYWHITE : BLACK;
        DrawTextEx(assets.gameFont, menuButtons[i].text, { textX, textY }, fontSize, spacing, textColor);
    }
}