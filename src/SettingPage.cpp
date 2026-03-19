#define _CRT_SECURE_NO_WARNINGS
#include "UI.h"
#include <algorithm>
#include <cmath>
#include <cstring>

// UpdateSettings (compatibility with main.cpp)
void UpdateSettings(GameSettings& settings, int selectedOption, Vector2 mousePoint, bool mousePressed) {
    // All logic is handled inside DrawSettingsMenu
}

// DrawSettingsMenu
void DrawSettingsMenu(int screenWidth, int screenHeight, GameSettings& settings,
    const GameAssets& assets, int& selectedOption, bool& backToMenu)
{
    Vector2 mouse = GetMousePosition();
    bool clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool held = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    //PANEL DIMENSIONS
    float pw = 780.0f, ph = 490.0f;
    float px = screenWidth / 2.0f - pw / 2.0f;
    float py = screenHeight / 2.0f - ph / 2.0f;
    float hh = 68.0f;
   

    // Title
    const char* title = "SETTINGS";

    DrawBaseMenu(screenWidth, screenHeight, assets, title);

    //ROW SETUP
    float rowX = px + 24.0f;
    float rowW = pw - 48.0f;
    float rowH = 85.0f;
    float r1y = py + 86.0f;
    float r2y = py + 194.0f;
    float r3y = py + 338.0f;
    float slW = rowW - 305.0f;

    //Helper: draw row background
    auto DrawRow = [&](float ry, bool active) {
        // Shadow
        DrawRectangleRounded({ rowX + 3, ry + 4, rowW, rowH }, 0.12f, 8, COLORA(0, 0, 0, 75));
        // Base
        Color base = active ? COLORA(88, 46, 7, 205) : COLORA(32, 16, 4, 185);
        DrawRectangleRounded({ rowX, ry, rowW, rowH }, 0.12f, 8, base);
        // Active: left accent bar + inner glow
        if (active) {
            DrawRectangleRounded({ rowX, ry, 6, rowH }, 0.8f, 6, COLOR(252, 155, 0));
            DrawRectangleRounded({ rowX + 6, ry, rowW - 6, rowH }, 0.12f, 8,
                COLORA(252, 135, 0, 16));
        }
        // Border
        Color bc = active ? COLOR(252, 165, 18) : COLORA(115, 62, 14, 175);
        DrawRectangleRoundedLines({ rowX, ry, rowW, rowH }, 0.12f, 8, bc);
        // Active outer glow
        if (active) {
            DrawRectangleRoundedLines({ rowX - 2, ry - 2, rowW + 4, rowH + 4 }, 0.12f, 8,
                COLORA(252, 175, 0, 58));
           /* DrawRectangleRoundedLines({ rowX - 4, ry - 4, rowW + 8, rowH + 8 }, 0.12f, 8,
                COLORA(252, 155, 0, 22));*/
        }
        };

    //Helper: draw icon circle
    auto DrawIconBg = [&](float ry, bool active) -> Color {
        Color ic = active ? COLOR(252, 175, 18) : COLORA(155, 88, 14, 175);
        DrawCircle((int)(rowX + 44), (int)(ry + rowH / 2), 26, COLORA(48, 24, 4, 205));
        DrawCircleLines((int)(rowX + 44), (int)(ry + rowH / 2), 26, ic);
        return ic;
        };

    //Helper: draw percent box
    auto DrawPctBox = [&](float ry, bool active, float vol) {
        char pct[8]; sprintf_s(pct, sizeof(pct), "%d%%", (int)(vol * 100));
        Rectangle pb = { rowX + rowW - 102, ry + 19, 80, 46 };
        DrawRectangleRounded(pb, 0.25f, 6, COLORA(28, 14, 2, 225));
        DrawRectangleRoundedLines(pb, 0.25f, 6,
            active ? COLOR(252, 165, 18) : COLORA(105, 62, 11, 175));
        if (active)
            DrawRectangleRounded({ pb.x + 2, pb.y + 2, pb.width - 4, 4 }, 0.5f, 4,
                COLORA(252, 195, 58, 75));
        Vector2 sz = MeasureTextEx(assets.gameFont, pct, 28, 1);
        DrawTextEx(assets.gameFont, pct,
            { pb.x + (pb.width - sz.x) / 2, pb.y + (pb.height - sz.y) / 2 },
            28, 1, active ? COLOR(252, 218, 78) : COLOR(185, 138, 58));
        };

    //MUSIC VOLUME
    DrawRow(r1y, selectedOption == 0);
    Color ic1 = DrawIconBg(r1y, selectedOption == 0);
    DrawMusicNoteIcon(rowX + 28, r1y + rowH / 2 - 17, ic1);
    Color lc1 = (selectedOption == 0) ? COLOR(252, 218, 68) : COLOR(182, 128, 48);
    DrawTextEx(assets.gameFont, "MUSIC", { rowX + 82, r1y + 14 }, 28, 1, lc1);
    DrawTextEx(assets.gameFont, "VOLUME", { rowX + 82, r1y + 42 }, 18, 1, Fade(lc1, 0.62f));
    DrawFancySlider(rowX + 182, r1y + 23, slW, settings.musicVolume, selectedOption == 0);
    DrawPctBox(r1y, selectedOption == 0, settings.musicVolume);
    if (selectedOption == 0)
        DrawTextEx(assets.gameFont, "A  <  adjust  >  D",
            { rowX + 182, r1y + 65 }, 14, 1, COLORA(252, 175, 38, 115));

    // SFX VOLUME
    DrawRow(r2y, selectedOption == 1);
    Color ic2 = DrawIconBg(r2y, selectedOption == 1);
    DrawSpeakerIcon(rowX + 26, r2y + rowH / 2 - 16, ic2, settings.sfxVolume);
    Color lc2 = (selectedOption == 1) ? COLOR(252, 218, 68) : COLOR(182, 128, 48);
    DrawTextEx(assets.gameFont, "SFX", { rowX + 82, r2y + 14 }, 28, 1, lc2);
    DrawTextEx(assets.gameFont, "VOLUME", { rowX + 82, r2y + 42 }, 18, 1, Fade(lc2, 0.62f));
    DrawFancySlider(rowX + 182, r2y + 23, slW, settings.sfxVolume, selectedOption == 1);
    DrawPctBox(r2y, selectedOption == 1, settings.sfxVolume);
    if (selectedOption == 1)
        DrawTextEx(assets.gameFont, "A  <  adjust  >  D",
            { rowX + 182, r2y + 65 }, 14, 1, COLORA(252, 175, 38, 115));

    //DIVIDER
    float divY = r2y + rowH + 22.0f;
    DrawGlowLine(px + 42, divY, px + pw - 42, COLORA(155, 88, 14, 135));
    DrawDiamond(px + pw / 2, divY, 5, COLORA(195, 138, 24, 155));
    DrawDiamond(px + pw / 2 - 100, divY, 3, COLORA(155, 95, 18, 105));
    DrawDiamond(px + pw / 2 + 100, divY, 3, COLORA(155, 95, 18, 105));

    //ROW 3: BACK TO MENU
    bool backAct = (selectedOption == 2);
    DrawRow(r3y, backAct);
    Color backIc = backAct ? COLOR(252, 208, 48) : COLOR(168, 108, 34);
    DrawCircle((int)(rowX + 44), (int)(r3y + rowH / 2), 26, COLORA(48, 24, 4, 205));
    DrawCircleLines((int)(rowX + 44), (int)(r3y + rowH / 2), 26, backIc);
    // Arrow
    DrawTriangle(
        { rowX + 32, r3y + rowH / 2 },
        { rowX + 49, r3y + rowH / 2 + 12 },
        { rowX + 49, r3y + rowH / 2 - 12 },
        backIc);
    DrawRectangle((int)(rowX + 45), (int)(r3y + rowH / 2 - 5), 16, 10, backIc);
    // Text
    const char* btxt = "BACK TO MENU";
    Vector2 bsz = MeasureTextEx(assets.gameFont, btxt, 34, 1);
    float btx = rowX + rowW / 2 - bsz.x / 2 + 18;
    float bty = r3y + rowH / 2 - bsz.y / 2;
    DrawTextEx(assets.gameFont, btxt, { btx + 2, bty + 2 }, 34, 1, COLORA(0, 0, 0, 95));
    DrawTextEx(assets.gameFont, btxt, { btx, bty }, 34, 1,
        backAct ? COLOR(252, 218, 68) : COLOR(182, 128, 48));
    // ENTER badge
    if (backAct) {
        Rectangle eb = { btx + bsz.x + 16, bty, 88, 32 };
        DrawRectangleRounded(eb, 0.4f, 6, COLORA(195, 115, 0, 155));
        DrawRectangleRoundedLines(eb, 0.4f, 6, COLOR(252, 175, 28));
        DrawTextEx(assets.gameFont, "ENTER", { eb.x + 7, eb.y + 5 }, 18, 1, COLOR(252, 228, 98));
    }

    // Hint Bar
    float hintY = py + ph + 14.0f;
    float hintW = 300.0f;
    float hintX = screenWidth / 2.0f - hintW / 2.0f;
    DrawRectangleRounded({ hintX, hintY, hintW, 30 }, 0.5f, 8, ColorBrownDark);
    DrawRectangleRoundedLines({ hintX, hintY, hintW, 30 }, 0.5f, 8, ColorBronzeAlpha);

    const char* hint = "W/S: Navigate  Enter: Select  A/D: Adjust";
    Vector2 hsz = MeasureTextEx(assets.gameFont, hint, 17, 1);
    DrawTextEx(assets.gameFont, hint, { screenWidth / 2.0f - hsz.x / 2.0f, hintY + 6 }, 17, 1, ColorGoldAlpha);


    // INPUT
    // ENTER → back
    if (backAct && IsKeyPressed(KEY_ENTER)) {
        backToMenu = true;
        if (assets.clickSound.stream.buffer != NULL)
            PlaySound(assets.clickSound);
    }

    // A/D adjust slider
    float step = 0.05f;
    if (selectedOption == 0) {
        if (IsKeyPressed(KEY_D)) settings.musicVolume = std::min(1.0f, settings.musicVolume + step);
        if (IsKeyPressed(KEY_A)) settings.musicVolume = std::max(0.0f, settings.musicVolume - step);
    }
    else if (selectedOption == 1) {
        if (IsKeyPressed(KEY_D)) settings.sfxVolume = std::min(1.0f, settings.sfxVolume + step);
        if (IsKeyPressed(KEY_A)) settings.sfxVolume = std::max(0.0f, settings.sfxVolume - step);
    }

    // Mouse drag sliders
    if (clicked || held) {
        float slStartX = rowX + 182;
        Rectangle ta1 = { slStartX - 15, r1y + 5, slW + 30, rowH - 10 };
        Rectangle ta2 = { slStartX - 15, r2y + 5, slW + 30, rowH - 10 };
        if (CheckCollisionPointRec(mouse, ta1)) {
            selectedOption = 0;
            settings.musicVolume = std::max(0.0f, std::min(1.0f, (mouse.x - slStartX) / slW));
        }
        else if (CheckCollisionPointRec(mouse, ta2)) {
            selectedOption = 1;
            settings.sfxVolume = std::max(0.0f, std::min(1.0f, (mouse.x - slStartX) / slW));
        }
    }
}
