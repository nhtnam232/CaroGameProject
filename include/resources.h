#ifndef RESOURCES_H
#define RESOURCES_H
#include "raylib.h"

struct GameAssets {
    // Image assets
    Texture2D bgMainMenu;
    Texture2D btnNormal;
    Texture2D btnPressed;
    Font gameFont;
    Texture2D gameLogo;


    // Audio assets
    Sound bgMusic;
    Sound clickSound;
    Sound moveSound;
    Sound winSound;
};

struct GameSettings {
    float musicVolume;
    float sfxVolume;
    bool isMusicMuted;
    bool isSfxMuted;
};

void LoadGameAssets(GameAssets& assets);
void UnloadGameAssets(GameAssets& assets);

#endif // RESOURCES_H