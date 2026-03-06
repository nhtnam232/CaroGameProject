#include"UI.h"


void DrawMainMenu(int screenWidth, int screenHeight, int selectedIndex, const GameAssets& assets) {

    //Set up buttons
    int btnWidth = 184;
    int btnHeight = 84;
    int spacing = 90;
    float startX = (screenWidth - btnWidth) / 2.0f;
    float startY = 200.0f;
    std::vector<Button> menuButtons = {
        { { startX, startY, (float)btnWidth, (float)btnHeight }, "NEW GAME" },
        { { startX, startY + spacing, (float)btnWidth, (float)btnHeight }, "LOAD GAME" },
        { { startX, startY + spacing * 2, (float)btnWidth, (float)btnHeight }, "SETTINGS" },
        { { startX, startY + spacing * 3, (float)btnWidth, (float)btnHeight }, "HELP" },
        { { startX, startY + spacing * 4, (float)btnWidth, (float)btnHeight }, "EXIT" }
    };
    //Fix Background
    Rectangle sourceRec = { 0.0f, 0.0f, (float)assets.bgMainMenu.width, (float)assets.bgMainMenu.height };
    Rectangle destRec = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
    Vector2 origin = { 0.0f, 0.0f };
    DrawTexturePro(assets.bgMainMenu, sourceRec, destRec, origin, 0.0f, WHITE);
    

    const char* title = "CARO GAME";
    float titleFontSize = 100.0f;
    float titleSpacing = 5.0f; 
    
    Vector2 titleSize = MeasureTextEx(assets.gameFont, title, titleFontSize, titleSpacing);
    Vector2 titlePos = { (screenWidth - titleSize.x) / 2.0f, 80.0f };
    
    DrawTextEx(assets.gameFont, title, titlePos, titleFontSize, titleSpacing, GetColor(0xBC0000FF));


    for (int i = 0; i < (int)menuButtons.size(); i++) {
        bool isSelected = (i == selectedIndex);
        Texture2D tex = isSelected ? assets.btnPressed : assets.btnNormal;

        Rectangle sourceRec = { 0.0f, 0.0f, 98.0f, 42.0f };
        Rectangle destRec = menuButtons[i].rect;

        DrawTexturePro(tex, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);

        int fontSize = 20;
        float spacing = 1.0f;
        int textWidth = MeasureText(menuButtons[i].text, fontSize);

        Vector2 textSize = MeasureTextEx(assets.gameFont, menuButtons[i].text, fontSize, spacing);

        float textX = menuButtons[i].rect.x + (menuButtons[i].rect.width - textSize.x) / 2.0f;
        float textY = menuButtons[i].rect.y + (menuButtons[i].rect.height - textSize.y) / 2.0f;

        Color textColor = isSelected ? RED : BLACK;
        DrawTextEx(assets.gameFont, menuButtons[i].text, { textX, textY }, fontSize, spacing, textColor);
    }
}