#include "raylib.h"
#include "UI.h"
enum GameState { STATE_MENU, STATE_LOAD, STATE_PLAYING, STATE_SETTINGS, STATE_EXIT, STATE_ABOUT, STATE_HELP, STATE_NEWGAME };

int main() {
    GameState currentState = STATE_MENU;
    GameState nextState = STATE_MENU;
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
    bool startMatch = false;
    std::vector<PlayerInfo> players = std::vector<PlayerInfo>(2);
    MatchManager match;
    for (int i = 0; i < 5; i++) {
        match.flashingPieces[i] = { -1.0f, -1.0f };
    }
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
            if (IsKeyReleased(KEY_ENTER)) {
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
                if (menuSelectedIndex == 0)      nextState = STATE_NEWGAME;
                else if (menuSelectedIndex == 1) nextState = STATE_LOAD;
                else if (menuSelectedIndex == 2) nextState = STATE_SETTINGS;
                else if (menuSelectedIndex == 3) nextState = STATE_HELP;
                else if (menuSelectedIndex == 4) nextState = STATE_ABOUT;
                else if (menuSelectedIndex == 5) nextState = STATE_EXIT;
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
                aboutSelectedIndex = (aboutSelectedIndex - 1 + 2) % 2;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
            else if (IsKeyPressed(KEY_ESCAPE)) {
                nextState = STATE_MENU;
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
                nextState = STATE_MENU;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
        }
        else if (currentState == STATE_LOAD) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                nextState = STATE_MENU;
                if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
            }
        }
        else if (currentState == STATE_PLAYING) {
            if (!match.isMatchOver) {
                UpdateMatchLogic(match, backToMenu);
                if (backToMenu) {
                    nextState = STATE_MENU;
                    backToMenu = false;
                    memset(players[0].name, 0, sizeof(players[0].name));
                    memset(players[1].name, 0, sizeof(players[1].name));
                }
            }
            else {
                // Increment timer for the animated victory text
                match.winTimer += GetFrameTime();

                if (match.winTimer >= 2.0f) {
                    if (IsKeyPressed(KEY_ENTER)) {
                        memset(match.board, 0, sizeof(match.board)); // Clear board array
                        match.isMatchOver = false;
                        match.winner = 0;
                        match.winTimer = 0.0f;
                        match.currentPlayer = 1;
                        match.cursorX = 7;
                        match.cursorY = 7;
                        for (int i = 0; i < 5; i++) {
                            match.flashingPieces[i] = { -1.0f, -1.0f };
                        }
                        if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
                    }
                    if (IsKeyPressed(KEY_ESCAPE)) {
                        nextState = STATE_MENU;
                        backToMenu = false;
                        memset(players[0].name, 0, sizeof(players[0].name));
                        memset(players[1].name, 0, sizeof(players[1].name));
                        if (!gameSettings.isSfxMuted) PlaySound(assets.clickSound);
                    }
                }
            }
        }
        else if (currentState == STATE_NEWGAME) {
         
        }
        else if (currentState == STATE_EXIT) {
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
                nextState = STATE_MENU; backToMenu = false; settingsSelectedIndex = 0;
                if (!gameSettings.isSfxMuted)PlaySound(assets.clickSound);
            }
            break;
        case STATE_PLAYING:
            DrawMatchScreen(SW, SH, match, assets, players);

            // Overlay the winner banner when the game ends
            if (match.isMatchOver  && match.winTimer >= 2.0f) {
                // Dim the background slightly to focus on the announcement
                DrawRectangle(0, 0, SW, SH, Fade(BLACK, 0.4f));

                // Anchor point for the victory text
                float panelH = 300.0f;
                float bannerY = (SH / 2.0f) - (panelH / 2.0f);

                // Map match.winner (1 or 2) to 0-based index (0 or 1) for the vector
                int winnerIdx = match.winner;
                // Call the function we defined earlier
                DrawWinnerDisplay(SW, bannerY, winnerIdx, players, assets, match.winTimer - 2.0f);
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
        case STATE_NEWGAME:
            DrawNewGameFlow(SW, SH, assets, backToMenu, startMatch, players, match);
            if (backToMenu) {
                nextState = STATE_MENU;
                backToMenu = false;
            }
            if (startMatch) {
                nextState = STATE_PLAYING;
                startMatch = false;
                memset(match.board, 0, sizeof(match.board));
                match.isMatchOver = false;
                match.winner = 0;
                match.winTimer = 0.0f;
                match.currentPlayer = 1;
                match.cursorX = 7;
                match.cursorY = 7;
            }
            break;
        default: break;
        }
        currentState = nextState;
        EndDrawing();
    }
    UnloadGameAssets(assets); 
    CloseAudioDevice(); 
    CloseWindow();
    return 0;
}
