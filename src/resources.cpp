#include "resources.h"

void LoadGameAssets(GameAssets& assets) {
    // Load images
    assets.bgMainMenu = LoadTexture("resources\\bgMainMenu.png");
    assets.btnNormal = LoadTexture("resources\\Button\\scrollButton.png");
    assets.btnPressed = LoadTexture("resources\\Button\\scrollButton.png");
    assets.gameFont = LoadFont("resources\\font\\Jersey10-Regular.ttf");
    assets.gameLogo = LoadTexture("resources\\logo.png");

    // Settings assets
    assets.sliderBar = LoadTexture("resources\\slider_bar.png");
    assets.sliderKnob = LoadTexture("resources\\slider_knob.png");
    assets.soundIcon = LoadTexture("resources\\sound_icon.png");
    assets.musicIcon = LoadTexture("resources\\music_icon.png");

    // Audio assets
    assets.bgMusic = LoadSound("resources\\audio\\bg_music.wav");
    assets.clickSound = LoadSound("resources\\audio\\click.wav");
    assets.moveSound = LoadSound("resources\\audio\\move.wav");
    assets.winSound = LoadSound("resources\\audio\\win.wav");

    // Checking load audio
    if (assets.bgMusic.stream.buffer == NULL) {
        TraceLog(LOG_WARNING, "Cannot load bg_music.wav - Check path");
    }
    if (assets.clickSound.stream.buffer == NULL) {
        TraceLog(LOG_WARNING, "Cannot load click.wav");
    }
    if (assets.moveSound.stream.buffer == NULL) {
        TraceLog(LOG_WARNING, "Cannot load move.wav");
    }
    if (assets.winSound.stream.buffer == NULL) {
        TraceLog(LOG_WARNING, "Cannot load win.wav");
    }
}

void UnloadGameAssets(GameAssets& assets) {
    // Unload textures
    UnloadTexture(assets.bgMainMenu);
    UnloadTexture(assets.btnNormal);
    UnloadTexture(assets.btnPressed);
    UnloadFont(assets.gameFont);
    UnloadTexture(assets.gameLogo);
    UnloadTexture(assets.sliderBar);
    UnloadTexture(assets.sliderKnob);
    UnloadTexture(assets.soundIcon);
    UnloadTexture(assets.musicIcon);

    // Unload audio
    UnloadSound(assets.bgMusic);
    UnloadSound(assets.clickSound);
    UnloadSound(assets.moveSound);
    UnloadSound(assets.winSound);
}