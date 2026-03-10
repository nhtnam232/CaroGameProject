#include "raylib.h"
#include "UI.h"

enum GameState {
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

    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "CARO GAME");
    SetTargetFPS(60);
    InitAudioDevice(); //Init audio

    // Load Assets
    GameAssets assets;
    LoadGameAssets(assets);

    // Init default setting
    GameSettings gameSettings;
    gameSettings.musicVolume = 0.5f;
    gameSettings.sfxVolume = 0.5f;
    gameSettings.isMusicMuted = false;
    gameSettings.isSfxMuted = false;

    // Playing background music
    PlaySound(assets.bgMusic);

    int selectedIndex = 0;
    int settingsSelectedIndex = 0;
    bool backToMenu = false;

    while (!WindowShouldClose() && currentState != STATE_EXIT) {
        if (!IsSoundPlaying(assets.bgMusic) && !gameSettings.isMusicMuted) {
            PlaySound(assets.bgMusic);
        }

        if (currentState == STATE_MENU) {
            if (IsKeyPressed(KEY_S)) {
                selectedIndex = (selectedIndex + 1) % 6;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
            if (IsKeyPressed(KEY_W)) {
                selectedIndex = (selectedIndex - 1 + 6) % 6;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
            if (IsKeyPressed(KEY_ENTER)) {
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
                if (selectedIndex == 0) currentState = STATE_PLAYING;
                else if (selectedIndex == 1) currentState = STATE_LOAD;
                else if (selectedIndex == 2) currentState = STATE_SETTINGS;
                else if (selectedIndex == 3) currentState = STATE_HELP;
                else if (selectedIndex == 4) currentState = STATE_EXIT;
                else if (selectedIndex == 5) currentState = STATE_ABOUT;
            }
        }
        else if (currentState == STATE_SETTINGS) {
            if (IsKeyPressed(KEY_S)) {
                settingsSelectedIndex = (settingsSelectedIndex + 1) % 2;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
            if (IsKeyPressed(KEY_W)) {
                settingsSelectedIndex = (settingsSelectedIndex - 1 + 2) % 2;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }

            Vector2 mousePoint = GetMousePosition();
            bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
            UpdateSettings(gameSettings, settingsSelectedIndex, mousePoint, mousePressed);

            //Using audio
            SetSoundVolume(assets.bgMusic, gameSettings.isMusicMuted ? 0.0f : gameSettings.musicVolume);
            SetSoundVolume(assets.clickSound, gameSettings.isSfxMuted ? 0.0f : gameSettings.sfxVolume);
            SetSoundVolume(assets.moveSound, gameSettings.isSfxMuted ? 0.0f : gameSettings.sfxVolume);
            SetSoundVolume(assets.winSound, gameSettings.isSfxMuted ? 0.0f : gameSettings.sfxVolume);

            if (gameSettings.isMusicMuted && IsSoundPlaying(assets.bgMusic)) {
                StopSound(assets.bgMusic);
            }
            else if (!gameSettings.isMusicMuted && !IsSoundPlaying(assets.bgMusic)) {
                PlaySound(assets.bgMusic);
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        // Drawing for each state
        switch (currentState) {
        case STATE_MENU:
            DrawMainMenu(screenWidth, screenHeight, selectedIndex, assets);
            break;

        case STATE_SETTINGS:
            DrawSettingsMenu(screenWidth, screenHeight, gameSettings, assets, settingsSelectedIndex, backToMenu);
            if (backToMenu) {
                currentState = STATE_MENU;
                backToMenu = false;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
            break;

        case STATE_PLAYING:
        case STATE_LOAD:
        case STATE_HELP:
        case STATE_ABOUT:
            // Comming soon
            DrawText("Coming soon...", screenWidth / 2 - 100, screenHeight / 2, 30, WHITE);
            DrawText("Press ESC to return to menu", screenWidth / 2 - 150, screenHeight / 2 + 50, 20, GRAY);

            if (IsKeyPressed(KEY_ESCAPE)) {
                currentState = STATE_MENU;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
            break;

        default:
            break;
        }

        EndDrawing();
    }

    // Cleaning
    UnloadGameAssets(assets);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}