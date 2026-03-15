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
    SetExitKey(0);
    SetTargetFPS(60);
    InitAudioDevice();

    // Load Assets
    GameAssets assets;
    LoadGameAssets(assets);

    // Khởi tạo settings mặc định
    GameSettings gameSettings;
    gameSettings.musicVolume = 0.5f;
    gameSettings.sfxVolume = 0.5f;
    gameSettings.isMusicMuted = false;
    gameSettings.isSfxMuted = false;

    // Phát nhạc nền
    PlaySound(assets.bgMusic);

    int menuSelectedIndex = 0;
    int settingsSelectedIndex = 0;
    int aboutSelectedIndex = 0;
    int helpSelectedIndex = 0;
    bool backToMenu = false;

    while (!WindowShouldClose() && currentState != STATE_EXIT) {
        // Loop nhạc nền thủ công
        if (!IsSoundPlaying(assets.bgMusic) && !gameSettings.isMusicMuted) {
            PlaySound(assets.bgMusic);
        }

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
            // W/S điều hướng 3 options: 0=Music, 1=SFX, 2=Back
            if (IsKeyPressed(KEY_S)) {
                settingsSelectedIndex = (settingsSelectedIndex + 1) % 3;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
            if (IsKeyPressed(KEY_W)) {
                settingsSelectedIndex = (settingsSelectedIndex - 1 + 3) % 3;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }

            // Áp dụng âm lượng realtime
            SetSoundVolume(assets.bgMusic,
                gameSettings.isMusicMuted ? 0.0f : gameSettings.musicVolume);
            SetSoundVolume(assets.clickSound,
                gameSettings.isSfxMuted ? 0.0f : gameSettings.sfxVolume);
            SetSoundVolume(assets.moveSound,
                gameSettings.isSfxMuted ? 0.0f : gameSettings.sfxVolume);
            SetSoundVolume(assets.winSound,
                gameSettings.isSfxMuted ? 0.0f : gameSettings.sfxVolume);

            // Xử lý mute/unmute nhạc nền
            if (gameSettings.isMusicMuted && IsSoundPlaying(assets.bgMusic)) {
                StopSound(assets.bgMusic);
            }
            else if (!gameSettings.isMusicMuted && !IsSoundPlaying(assets.bgMusic)) {
                PlaySound(assets.bgMusic);
            }
        }
        else if (currentState == STATE_ABOUT) {
            // Handle user input for tab switching (A/D)
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
                aboutSelectedIndex = 1;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            } 
            else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
                aboutSelectedIndex = 0;
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
                helpSelectedIndex = (helpSelectedIndex -1 + 3) % 3;
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
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState) {
        case STATE_MENU:
            DrawMainMenu(screenWidth, screenHeight, menuSelectedIndex, assets);
            break;

        case STATE_SETTINGS:
            DrawSettingsMenu(screenWidth, screenHeight, gameSettings, assets,
                settingsSelectedIndex, backToMenu);
            if (backToMenu) {
                currentState = STATE_MENU;
                backToMenu = false;
                settingsSelectedIndex = 0;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
            break;

        case STATE_PLAYING:
            break;
        case STATE_LOAD:
            break;
        case STATE_HELP:
            DrawHelpMenu(screenWidth, screenHeight, assets, helpSelectedIndex);
            break;
        case STATE_ABOUT:
            DrawAboutMenu(screenWidth, screenHeight, assets, aboutSelectedIndex);
            break;

        default:
            break;
        }

        EndDrawing();
    }

    UnloadGameAssets(assets);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
