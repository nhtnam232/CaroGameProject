#include "raylib.h"
#include "UI.h"

enum GameState{
    STATE_MENU,      
    STATE_PLAYING,   
    STATE_SETTINGS,  
    STATE_HELP,     
    STATE_EXIT     
};

int main() {

    GameState currentState = STATE_MENU;
    //Set up screen
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "KONOHA CARO");
    SetTargetFPS(60);

    
    int btnWidth = 184;
    int btnHeight = 84;
    int spacing = 90;
    float startX = (screenWidth - btnWidth) / 2.0f;
    float startY =200.0f; 

    //Load Assets
    GameAssets assets;
    LoadGameAssets(assets);
    
    //Set up Menu Button
    std::vector<Button> menuButtons = {
        { { startX, startY, (float)btnWidth, (float)btnHeight }, "NEW GAME" },
        { { startX, startY + spacing, (float)btnWidth, (float)btnHeight }, "LOAD GAME" },
        { { startX, startY + spacing * 2, (float)btnWidth, (float)btnHeight }, "SETTINGS" },
        { { startX, startY + spacing * 3, (float)btnWidth, (float)btnHeight }, "HELP" },
        { { startX, startY + spacing * 4, (float)btnWidth, (float)btnHeight }, "EXIT" }
    };
 
    int selectedIndex = 0;
    while (!WindowShouldClose() && currentState != STATE_EXIT) {
        //Switch state in game
        switch (currentState) {
        case STATE_MENU:
            if (IsKeyPressed(KEY_S)) {
                selectedIndex++;

                if (selectedIndex >= menuButtons.size()) {
                    selectedIndex = 0;
                }
            }
            if (IsKeyPressed(KEY_W)) {
                selectedIndex--;

                if (selectedIndex < 0) {
                    selectedIndex = menuButtons.size() - 1;
                }
            }
            if (IsKeyPressed(KEY_ENTER)) {
                if (selectedIndex == 0) currentState = STATE_PLAYING;
                else if (selectedIndex == 3) currentState = STATE_HELP;
                else if (selectedIndex == 4) currentState = STATE_EXIT;
            }
            break;
        }
        //Drawing
        BeginDrawing();
        switch (currentState) {
        case STATE_MENU:
            DrawMainMenu(screenWidth, screenHeight, menuButtons, selectedIndex, assets);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}