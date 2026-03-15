#include "raylib.h"
#include "UI.h"

enum GameState { STATE_MENU, STATE_LOAD, STATE_PLAYING, STATE_SETTINGS, STATE_EXIT, STATE_ABOUT };

int main() {
    GameState currentState = STATE_MENU;
    const int SW = 1280, SH = 720;
    InitWindow(SW, SH, "CARO GAME"); SetTargetFPS(60); InitAudioDevice();

    GameAssets assets; LoadGameAssets(assets);
    GameSettings gameSettings;
    gameSettings.musicVolume = 0.5f; gameSettings.sfxVolume = 0.5f;
    gameSettings.isMusicMuted = false; gameSettings.isSfxMuted = false;
    PlaySound(assets.bgMusic);

    int selectedIndex = 0, settingsIdx = 0;
    bool backToMenu = false;

    while (!WindowShouldClose() && currentState != STATE_EXIT) {
        if (!IsSoundPlaying(assets.bgMusic) && !gameSettings.isMusicMuted) PlaySound(assets.bgMusic);

        if (currentState == STATE_MENU) {
            if (IsKeyPressed(KEY_S)) { selectedIndex = (selectedIndex + 1) % 6; if (!gameSettings.isSfxMuted)PlaySound(assets.clickSound); }
            if (IsKeyPressed(KEY_W)) { selectedIndex = (selectedIndex - 1 + 6) % 6; if (!gameSettings.isSfxMuted)PlaySound(assets.clickSound); }
            if (IsKeyPressed(KEY_ENTER)) {
                if (!gameSettings.isSfxMuted)PlaySound(assets.clickSound);
                // Buttons: 0=NEW GAME 1=LOAD GAME 2=SETTINGS 3=HELP 4=ABOUT US 5=EXIT
                if (selectedIndex == 0) { ResetNewGame(); currentState = STATE_PLAYING; }
                else if (selectedIndex == 1) currentState = STATE_LOAD;
                else if (selectedIndex == 2) currentState = STATE_SETTINGS;
                else if (selectedIndex == 3) currentState = STATE_ABOUT;
                else if (selectedIndex == 5) currentState = STATE_EXIT;
            }
        }
        else if (currentState == STATE_SETTINGS) {
            if (IsKeyPressed(KEY_S)) { settingsIdx = (settingsIdx + 1) % 3; if (!gameSettings.isSfxMuted)PlaySound(assets.clickSound); }
            if (IsKeyPressed(KEY_W)) { settingsIdx = (settingsIdx - 1 + 3) % 3; if (!gameSettings.isSfxMuted)PlaySound(assets.clickSound); }
            SetSoundVolume(assets.bgMusic, gameSettings.isMusicMuted ? 0.f : gameSettings.musicVolume);
            SetSoundVolume(assets.clickSound, gameSettings.isSfxMuted ? 0.f : gameSettings.sfxVolume);
            SetSoundVolume(assets.moveSound, gameSettings.isSfxMuted ? 0.f : gameSettings.sfxVolume);
            SetSoundVolume(assets.winSound, gameSettings.isSfxMuted ? 0.f : gameSettings.sfxVolume);
            if (gameSettings.isMusicMuted && IsSoundPlaying(assets.bgMusic)) StopSound(assets.bgMusic);
            else if (!gameSettings.isMusicMuted && !IsSoundPlaying(assets.bgMusic)) PlaySound(assets.bgMusic);
        }

        BeginDrawing(); ClearBackground(BLACK);
        switch (currentState) {
        case STATE_MENU:
            DrawMainMenu(SW, SH, selectedIndex, assets); break;
        case STATE_SETTINGS:
            DrawSettingsMenu(SW, SH, gameSettings, assets, settingsIdx, backToMenu);
            if (backToMenu) {
                currentState = STATE_MENU; backToMenu = false; settingsIdx = 0;
                if (!gameSettings.isSfxMuted)PlaySound(assets.clickSound);
            }
            break;
        case STATE_PLAYING:
            DrawNewGameFlow(SW, SH, assets, backToMenu);
            if (backToMenu) {
                currentState = STATE_MENU; backToMenu = false;
                if (!gameSettings.isSfxMuted)PlaySound(assets.clickSound);
            }
            break;
        case STATE_LOAD: case STATE_ABOUT:
            DrawText("Coming soon...", SW / 2 - 100, SH / 2, 30, WHITE);
            DrawText("Press ESC to return", SW / 2 - 120, SH / 2 + 50, 20, GRAY);
            if (IsKeyPressed(KEY_ESCAPE)) { currentState = STATE_MENU; if (!gameSettings.isSfxMuted)PlaySound(assets.clickSound); }
            break;
        default: break;
        }
        EndDrawing();
    }
    UnloadGameAssets(assets); CloseAudioDevice(); CloseWindow();
    return 0;
}
