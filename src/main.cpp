#include "raylib.h"
#include "UI.h"

enum GameState{
    STATE_MENU,      
    STATE_LOAD,
    STATE_PLAYING,   
    STATE_SETTINGS,  
    STATE_HELP,     
    STATE_EXIT,
    STATE_ABOUT
};

int main() {

    GameState currentState = STATE_MENU;
    //Set up screen
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "CARO GAME");
    SetTargetFPS(60);


    //Load Assets
    GameAssets assets;
    LoadGameAssets(assets);    
 
    int selectedIndex = 0;
    while (!WindowShouldClose() && currentState != STATE_EXIT) {
        //Switch state in game
        switch (currentState) {
        case STATE_MENU:
            if (IsKeyPressed(KEY_S)) {
                selectedIndex = (selectedIndex + 1) % 6;
            }
            if (IsKeyPressed(KEY_W)) {
                selectedIndex = (selectedIndex - 1 + 6) % 6;
            }
            if (IsKeyPressed(KEY_ENTER)) {
                if (selectedIndex == 0) currentState = STATE_PLAYING;
                else if (selectedIndex == 1) currentState = STATE_LOAD;
                else if (selectedIndex == 2) currentState = STATE_SETTINGS;
                else if (selectedIndex == 3) currentState = STATE_HELP;
                else if (selectedIndex == 4) currentState = STATE_EXIT;
                else if (selectedIndex == 5) currentState = STATE_ABOUT;
            }
            break;
        }
        //Drawing
        BeginDrawing();
        switch (currentState) {
        case STATE_MENU:
            DrawMainMenu(screenWidth, screenHeight, selectedIndex, assets);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}