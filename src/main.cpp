#include "raylib.h"
#include "UI.h"

enum GameState { STATE_MENU, STATE_LOAD, STATE_PLAYING, STATE_SETTINGS, STATE_EXIT, STATE_ABOUT, STATE_HELP };

int main() {
    GameState currentState = STATE_MENU;
    const int SW = 1280, SH = 720;
    InitWindow(SW, SH, "CARO GAME"); SetTargetFPS(60); InitAudioDevice();
    SetExitKey(0);
    GameAssets assets; LoadGameAssets(assets);
    GameSettings gameSettings;
    gameSettings.musicVolume = 0.5f; gameSettings.sfxVolume = 0.5f;
    gameSettings.isMusicMuted = false; gameSettings.isSfxMuted = false;
    PlaySound(assets.bgMusic);

    int menuSelectedIndex = 0;
    int settingsSelectedIndex = 0;
    int aboutSelectedIndex = 0;
    int helpSelectedIndex = 0;
    bool backToMenu = false;

    while (!WindowShouldClose() && currentState != STATE_EXIT) {
        if (!IsSoundPlaying(assets.bgMusic) && !gameSettings.isMusicMuted) PlaySound(assets.bgMusic);

        // Xử lý input theo từng state
        if (currentState == STATE_MENU) {
            if (IsKeyPressed(KEY_S)) {
                menuSelectedIndex = (menuSelectedIndex + 1) % 6;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
            if (IsKeyPressed(KEY_W)) {
                menuSelectedIndex = (menuSelectedIndex - 1 + 6) % 6;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
            if (IsKeyPressed(KEY_ENTER)) {
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
                if (menuSelectedIndex == 0)      currentState = STATE_PLAYING;
                else if (menuSelectedIndex == 1) currentState = STATE_LOAD;
                else if (menuSelectedIndex == 2) currentState = STATE_SETTINGS;
                else if (menuSelectedIndex == 3) currentState = STATE_HELP;
                else if (menuSelectedIndex == 4) currentState = STATE_ABOUT;
                else if (menuSelectedIndex == 5) currentState = STATE_EXIT;
            }
        }
        else if (currentState == STATE_SETTINGS) {
            if (IsKeyPressed(KEY_S)) { settingsSelectedIndex = (settingsSelectedIndex + 1) % 3; if (!gameSettings.isSfxMuted)PlaySound(assets.clickSound); }
            if (IsKeyPressed(KEY_W)) { settingsSelectedIndex = (settingsSelectedIndex - 1 + 3) % 3; if (!gameSettings.isSfxMuted)PlaySound(assets.clickSound); }
            SetSoundVolume(assets.bgMusic, gameSettings.isMusicMuted ? 0.f : gameSettings.musicVolume);
            SetSoundVolume(assets.clickSound, gameSettings.isSfxMuted ? 0.f : gameSettings.sfxVolume);
            SetSoundVolume(assets.moveSound, gameSettings.isSfxMuted ? 0.f : gameSettings.sfxVolume);
            SetSoundVolume(assets.winSound, gameSettings.isSfxMuted ? 0.f : gameSettings.sfxVolume);
            if (gameSettings.isMusicMuted && IsSoundPlaying(assets.bgMusic)) StopSound(assets.bgMusic);
            else if (!gameSettings.isMusicMuted && !IsSoundPlaying(assets.bgMusic)) PlaySound(assets.bgMusic);
        }
        else if (currentState == STATE_ABOUT) {
            // Handle user input for tab switching (A/D)
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
                aboutSelectedIndex = (aboutSelectedIndex + 1) % 2;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
            else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
                aboutSelectedIndex = (aboutSelectedIndex - 1 + 1) % 2;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
            else if (IsKeyPressed(KEY_ESCAPE)) {
                currentState = STATE_MENU;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
        }
        else if (currentState == STATE_HELP) {
            // Handle user input for tab switching (A/D)
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
                helpSelectedIndex = (helpSelectedIndex + 1) % 3;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
            else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
                helpSelectedIndex = (helpSelectedIndex - 1 + 3) % 3;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
            else if (IsKeyPressed(KEY_ESCAPE)) {
                currentState = STATE_MENU;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
        }
        else if (currentState == STATE_EXIT) {
            //Quit while loop to clear the memory and end the program
            break;
        }

        BeginDrawing(); ClearBackground(BLACK);
        switch (currentState) {
        case STATE_MENU:
            DrawMainMenu(SW, SH, menuSelectedIndex, assets);
            break;

        case STATE_SETTINGS:
            DrawSettingsMenu(SW, SH, gameSettings, assets, settingsSelectedIndex, backToMenu);
            if (backToMenu) {
                currentState = STATE_MENU; backToMenu = false; settingsSelectedIndex = 0;
                if (!gameSettings.isSfxMuted)PlaySound(assets.clickSound);
            }
            break;
        case STATE_PLAYING:
            if (backToMenu) {
                currentState = STATE_MENU; backToMenu = false;
                if (!gameSettings.isSfxMuted)PlaySound(assets.clickSound);
            }
            break;
        case STATE_LOAD:
            break;
        case STATE_HELP:
            DrawHelpMenu(SW, SH, assets, helpSelectedIndex);
            break;
        case STATE_ABOUT:
            DrawAboutMenu(SW, SH, assets, aboutSelectedIndex);
            break;
        default: break;
        }
        EndDrawing();
    }
    UnloadGameAssets(assets); 
    CloseAudioDevice(); 
    CloseWindow();
    return 0;
}
