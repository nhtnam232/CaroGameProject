#define _CRT_SECURE_NO_WARNINGS
#include "UI.h"
#include <algorithm>
#include <cmath>
#include <cstring>

// ─── COLOR helpers ────────────────────────────────────────────────────────────
Color COLOR(unsigned char r, unsigned char g, unsigned char b) {
    Color c; c.r = r; c.g = g; c.b = b; c.a = 255; return c;
}
Color COLORA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    Color c; c.r = r; c.g = g; c.b = b; c.a = a; return c;
}

// ─── Glowing horizontal line ──────────────────────────────────────────────────
static void DrawGlowLine(float x1, float y, float x2, Color col) {
    DrawLineEx({ x1, y }, { x2, y }, 3.0f, col);
    DrawLineEx({ x1, y }, { x2, y }, 1.0f, Fade(WHITE, 0.25f));
}

// ─── Diamond shape ────────────────────────────────────────────────────────────
static void DrawDiamond(float x, float y, float r, Color c) {
    DrawTriangle({ x, y - r }, { x + r, y }, { x - r, y }, c);
    DrawTriangle({ x - r, y }, { x + r, y }, { x, y + r }, c);
}

// ─── Ornate panel border with corner brackets ─────────────────────────────────
static void DrawOrnateFrame(float x, float y, float w, float h, Color border, Color inner) {
    // Outer glow rings
    for (int i = 4; i >= 1; i--)
        DrawRectangleRoundedLines(
            { x - i * 2.5f, y - i * 2.5f, w + i * 5.0f, h + i * 5.0f },
            0.06f, 8, Fade(border, 0.05f * i));
    // Main border
    DrawRectangleRoundedLines({ x, y, w, h }, 0.06f, 8, border);
    // Inner thin border
    DrawRectangleRoundedLines({ x + 6, y + 6, w - 12, h - 12 }, 0.06f, 8, Fade(inner, 0.4f));

    // Corner L-brackets
    float cs = 22.0f, ct = 3.0f;
    // TL
    DrawLineEx({ x + 2, y + cs }, { x + 2, y + 2 }, ct, border);
    DrawLineEx({ x + 2, y + 2 }, { x + cs, y + 2 }, ct, border);
    DrawCircle((int)(x + 2), (int)(y + 2), 4, border);
    // TR
    DrawLineEx({ x + w - cs, y + 2 }, { x + w - 2, y + 2 }, ct, border);
    DrawLineEx({ x + w - 2, y + 2 }, { x + w - 2, y + cs }, ct, border);
    DrawCircle((int)(x + w - 2), (int)(y + 2), 4, border);
    // BL
    DrawLineEx({ x + 2, y + h - cs }, { x + 2, y + h - 2 }, ct, border);
    DrawLineEx({ x + 2, y + h - 2 }, { x + cs, y + h - 2 }, ct, border);
    DrawCircle((int)(x + 2), (int)(y + h - 2), 4, border);
    // BR
    DrawLineEx({ x + w - cs, y + h - 2 }, { x + w - 2, y + h - 2 }, ct, border);
    DrawLineEx({ x + w - 2, y + h - cs }, { x + w - 2, y + h - 2 }, ct, border);
    DrawCircle((int)(x + w - 2), (int)(y + h - 2), 4, border);
}

// ─── Fancy slider ─────────────────────────────────────────────────────────────
static void DrawFancySlider(float x, float y, float w, float val, bool active) {
    float th = 10.0f;
    float ty = y + 15.0f;
    float fill = val * w;

    // Shadow
    DrawRectangleRounded({ x + 2, ty + 2, w, th }, 1.0f, 6, Fade(BLACK, 0.5f));
    // Track
    DrawRectangleRounded({ x, ty, w, th }, 1.0f, 6, COLORA(18, 9, 2, 230));
    DrawRectangleLinesEx({ x, ty, w, th }, 1.5f, COLORA(90, 50, 10, 180));

    // Fill
    if (fill > 3.0f) {
        Color fc = active ? COLOR(215, 125, 0) : COLOR(130, 75, 10);
        DrawRectangleRounded({ x, ty, fill, th }, 1.0f, 6, fc);
        DrawRectangleRounded({ x + 2, ty + 1, fill - 4, th * 0.4f }, 1.0f, 4,
            Fade(WHITE, active ? 0.22f : 0.08f));
    }

    // Tick marks
    for (int i = 1; i < 10; i++) {
        float tx2 = x + w / 10.0f * i;
        Color tc = (tx2 <= x + fill) ? COLORA(255, 195, 50, 55) : COLORA(80, 45, 10, 110);
        DrawLineEx({ tx2, ty + 1 }, { tx2, ty + th - 1 }, 1.0f, tc);
    }

    // Knob glow (active only)
    float kx = x + fill, ky = ty + th / 2.0f;
    if (active)
        for (int i = 3; i >= 1; i--)
            DrawCircle((int)kx, (int)ky, (int)(22.0f + i * 3), Fade(COLOR(255, 145, 0), 0.055f * i));

    // Knob
    DrawCircle((int)kx + 2, (int)ky + 2, 16, Fade(BLACK, 0.5f));
    DrawCircle((int)kx, (int)ky, 16, active ? COLOR(225, 130, 0) : COLOR(155, 90, 18));
    DrawCircle((int)kx, (int)ky, 11, active ? COLOR(255, 195, 75) : COLOR(195, 140, 55));
    DrawCircle((int)kx, (int)ky, 5, active ? COLOR(255, 228, 135) : COLOR(218, 170, 95));
    DrawCircleLines((int)kx, (int)ky, 16, active ? COLOR(255, 175, 15) : COLORA(115, 65, 12, 200));
    DrawCircle((int)kx - 4, (int)ky - 4, 4, Fade(WHITE, active ? 0.45f : 0.2f));
}

// ─── Music note icon ──────────────────────────────────────────────────────────
static void DrawMusicNoteIcon(float x, float y, Color c) {
    DrawCircle((int)(x + 4), (int)(y + 26), 8, c);
    DrawCircle((int)(x + 24), (int)(y + 20), 8, c);
    DrawRectangle((int)(x + 10), (int)(y + 2), 4, 20, c);
    DrawRectangle((int)(x + 30), (int)(y + 0), 4, 16, c);
    DrawRectangle((int)(x + 10), (int)(y + 2), 24, 5, c);
}

// ─── Speaker icon ─────────────────────────────────────────────────────────────
static void DrawSpeakerIcon(float x, float y, Color c, float vol) {
    DrawRectangleRounded({ x, y + 8, 12, 14 }, 0.2f, 4, c);
    Vector2 h1 = { x + 12, y + 8 };
    Vector2 h2 = { x + 26, y + 1 };
    Vector2 h3 = { x + 26, y + 29 };
    Vector2 h4 = { x + 12, y + 22 };
    DrawTriangle(h2, h1, h4, c);
    DrawTriangle(h2, h4, h3, c);
    int waves = (vol > 0.66f) ? 3 : (vol > 0.33f) ? 2 : 1;
    for (int i = 0; i < waves; i++)
        DrawCircleSector({ x + 27, y + 15 }, 10.0f + i * 9.0f, -45, 45, 12,
            Fade(c, 0.38f - i * 0.1f));
}

// ─── Side scroll banner ───────────────────────────────────────────────────────
static void DrawSideBanner(float x, float topY, float botY,
    bool flipText, const GameAssets& assets)
{
    float bw = 34.0f;
    float bx = x - bw / 2;
    float time = (float)GetTime();

    // Body
    DrawRectangleRounded({ bx, topY, bw, botY - topY }, 0.3f, 8, COLORA(55, 27, 7, 170));
    DrawRectangleRoundedLines({ bx, topY, bw, botY - topY }, 0.3f, 8, COLOR(155, 85, 18));
    DrawRectangleRoundedLines({ bx + 3, topY + 3, bw - 6, botY - topY - 6 }, 0.3f, 8,
        COLORA(215, 145, 38, 70));

    // Top cap
    DrawRectangleRounded({ bx - 10, topY - 10, bw + 20, 22 }, 0.5f, 8, COLOR(95, 48, 11));
    DrawRectangleRoundedLines({ bx - 10, topY - 10, bw + 20, 22 }, 0.5f, 8, COLOR(195, 125, 28));
    DrawCircle((int)x, (int)(topY - 10), 7, COLOR(215, 155, 38));
    DrawCircleLines((int)x, (int)(topY - 10), 7, COLOR(145, 85, 13));

    // Bottom cap
    DrawRectangleRounded({ bx - 10, botY - 12, bw + 20, 22 }, 0.5f, 8, COLOR(95, 48, 11));
    DrawRectangleRoundedLines({ bx - 10, botY - 12, bw + 20, 22 }, 0.5f, 8, COLOR(195, 125, 28));
    DrawCircle((int)x, (int)(botY + 10), 7, COLOR(215, 155, 38));
    DrawCircleLines((int)x, (int)(botY + 10), 7, COLOR(145, 85, 13));

    // Segment lines + dots
    for (int i = 1; i <= 4; i++) {
        float ly = topY + (botY - topY) * i / 5.0f;
        DrawLineEx({ bx + 4, ly }, { bx + bw - 4, ly }, 1.0f, COLORA(195, 135, 38, 75));
        DrawCircle((int)x, (int)ly, 3, COLORA(215, 160, 30, 175));
    }

    // Vertical text
    float alpha = 0.38f + 0.18f * sinf(time * 1.0f);
    Color tc = Fade(COLOR(255, 205, 75), alpha);
    const char* txt = "CARO";
    int len = (int)strlen(txt);
    float startY2 = flipText ? botY - 22 : topY + 20;
    float dir = flipText ? -1.0f : 1.0f;
    for (int i = 0; i < len; i++) {
        char ch[2] = { txt[i], '\0' };
        Vector2 sz = MeasureTextEx(assets.gameFont, ch, 16, 1);
        DrawTextEx(assets.gameFont, ch,
            { x - sz.x / 2, startY2 + dir * i * 22.0f }, 16, 1, tc);
    }
}

// ─── Screen corner ornament ───────────────────────────────────────────────────
static void DrawCornerOrnament(float x, float y, float dx, float dy, Color c) {
    float len = 52.0f, thick = 3.0f;
    DrawLineEx({ x, y }, { x + dx * len, y }, thick, c);
    DrawLineEx({ x, y }, { x, y + dy * len }, thick, c);
    DrawLineEx({ x + dx * len * 0.5f, y }, { x + dx * len, y }, thick * 0.4f, Fade(c, 0.35f));
    DrawLineEx({ x, y + dy * len * 0.5f }, { x, y + dy * len }, thick * 0.4f, Fade(c, 0.35f));
    DrawCircle((int)x, (int)y, 5, c);
    DrawCircle((int)(x + dx * len), (int)y, 3, Fade(c, 0.55f));
    DrawCircle((int)x, (int)(y + dy * len), 3, Fade(c, 0.55f));
}

// ─── UpdateSettings (compatibility with main.cpp) ────────────────────────────
void UpdateSettings(GameSettings& settings, int selectedOption, Vector2 mousePoint, bool mousePressed) {
    // All logic is handled inside DrawSettingsMenu
}

// ═════════════════════════════════════════════════════════════════════════════
// DrawSettingsMenu
// ═════════════════════════════════════════════════════════════════════════════
void DrawSettingsMenu(int screenWidth, int screenHeight, GameSettings& settings,
    const GameAssets& assets, int& selectedOption, bool& backToMenu)
{
    Vector2 mouse = GetMousePosition();
    bool clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool held = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    float time = (float)GetTime();

    // ── 1. BACKGROUND ────────────────────────────────────────────────────────
    Rectangle srcBg = { 0, 0, (float)assets.bgMainMenu.width, (float)assets.bgMainMenu.height };
    Rectangle dstBg = { 0, 0, (float)screenWidth, (float)screenHeight };
    DrawTexturePro(assets.bgMainMenu, srcBg, dstBg, { 0, 0 }, 0.0f, WHITE);
    // Dark vignette
    DrawRectangle(0, 0, screenWidth, screenHeight, COLORA(8, 4, 0, 115));

    // ── 2. SCREEN BORDER & CORNERS ───────────────────────────────────────────
    Color goldDim = COLORA(175, 105, 18, 155);
    DrawRectangleLinesEx({ 6, 6, (float)screenWidth - 12, (float)screenHeight - 12 },
        2.0f, COLORA(155, 88, 14, 115));
    DrawRectangleLinesEx({ 12, 12, (float)screenWidth - 24, (float)screenHeight - 24 },
        1.0f, COLORA(195, 125, 18, 55));
    DrawCornerOrnament(18, 18, 1, 1, goldDim);
    DrawCornerOrnament(screenWidth - 18, 18, -1, 1, goldDim);
    DrawCornerOrnament(18, screenHeight - 18, 1, -1, goldDim);
    DrawCornerOrnament(screenWidth - 18, screenHeight - 18, -1, -1, goldDim);

    // ── 3. PANEL DIMENSIONS ──────────────────────────────────────────────────
    float pw = 780.0f, ph = 490.0f;
    float px = screenWidth / 2.0f - pw / 2.0f;
    float py = screenHeight / 2.0f - ph / 2.0f;

    // ── 4. SIDE BANNERS ───────────────────────────────────────────────────────
    float bannerX1 = px - 55.0f;
    float bannerX2 = px + pw + 55.0f;
    float bTop = py + 12.0f, bBot = py + ph - 12.0f;
    DrawSideBanner(bannerX1, bTop, bBot, false, assets);
    DrawSideBanner(bannerX2, bTop, bBot, true, assets);

    // Dashed connectors banner → panel
    float midY = py + ph / 2.0f;
    float lx1s = bannerX1 + 18, lx1e = px;
    float lx2s = px + pw, lx2e = bannerX2 - 18;
    for (int i = 0; i < 5; i++) {
        float a = lx1s + (lx1e - lx1s) * i / 5.0f;
        float b2 = lx1s + (lx1e - lx1s) * (i + 0.75f) / 5.0f;
        float c2 = lx2s + (lx2e - lx2s) * i / 5.0f;
        float d = lx2s + (lx2e - lx2s) * (i + 0.75f) / 5.0f;
        DrawLineEx({ a, midY }, { b2, midY }, 2.0f, COLORA(195, 135, 28, 135));
        DrawLineEx({ c2, midY }, { d, midY }, 2.0f, COLORA(195, 135, 28, 135));
    }
    for (int i = 1; i <= 2; i++) {
        DrawCircle((int)(lx1s + (lx1e - lx1s) * i / 3.0f), (int)midY, 4, COLORA(215, 160, 30, 175));
        DrawCircle((int)(lx2s + (lx2e - lx2s) * i / 3.0f), (int)midY, 4, COLORA(215, 160, 30, 175));
    }

    // ── 5. FLOATING PARTICLES ────────────────────────────────────────────────
    for (int i = 0; i < 20; i++) {
        float angle = i * 137.508f * (3.14159f / 180.0f);
        float dist = 255.0f + 115.0f * (float)fmod(i * 0.618f, 1.0f);
        float fpx = screenWidth / 2.0f + cosf(angle + time * 0.07f) * dist;
        float fpy = screenHeight / 2.0f + sinf(angle + time * 0.055f) * dist * 0.52f;
        if (fpx > px - 25 && fpx < px + pw + 25 && fpy > py - 25 && fpy < py + ph + 25) continue;
        float fl = 0.1f + 0.1f * sinf(time * 1.2f + i * 0.95f);
        float sz = 1.5f + (float)fmod(i * 0.3f, 1.5f);
        DrawCircle((int)fpx, (int)fpy, (int)sz, Fade(COLOR(255, 205, 55), fl));
        if (i % 3 == 0)
            DrawCircleLines((int)fpx, (int)fpy, (int)(sz + 3), Fade(COLOR(195, 125, 18), fl * 0.38f));
    }

    // ── 6. PANEL ─────────────────────────────────────────────────────────────
    // Pulse glow
    float pulse = 0.035f + 0.018f * sinf(time * 1.4f);
    for (int i = 5; i >= 1; i--)
        DrawRectangleRounded(
            { px - i * 3.0f, py - i * 3.0f, pw + i * 6.0f, ph + i * 6.0f },
            0.07f, 8, Fade(COLOR(175, 95, 8), pulse * i));

    // Panel fill
    DrawRectangleRounded({ px, py, pw, ph }, 0.07f, 8, COLORA(16, 8, 2, 248));
    // Subtle top highlight
    DrawRectangleRounded({ px, py, pw, ph * 0.42f }, 0.07f, 8, COLORA(32, 16, 5, 55));

    // Ornate border
    DrawOrnateFrame(px, py, pw, ph, COLOR(195, 115, 18), COLOR(250, 185, 48));

    // ── 7. HEADER ────────────────────────────────────────────────────────────
    float hh = 68.0f;
    DrawRectangleRounded({ px, py, pw, hh }, 0.07f, 8, COLORA(75, 36, 7, 225));
    DrawGlowLine(px + 22, py + hh, px + pw - 22, COLOR(195, 125, 18));
    DrawDiamond(px + pw / 2, py + hh, 8, COLOR(250, 195, 38));
    DrawDiamond(px + pw / 2 - 110, py + hh, 4, COLORA(195, 135, 28, 175));
    DrawDiamond(px + pw / 2 + 110, py + hh, 4, COLORA(195, 135, 28, 175));

    // Title
    const char* title = "SETTINGS";
    Vector2 tsz = MeasureTextEx(assets.gameFont, title, 56, 4);
    float ttx = px + pw / 2 - tsz.x / 2;
    float tty = py + hh / 2 - tsz.y / 2;
    DrawTextEx(assets.gameFont, title, { ttx + 4, tty + 4 }, 56, 4, COLORA(0, 0, 0, 115));
    DrawTextEx(assets.gameFont, title, { ttx + 2, tty + 2 }, 56, 4, COLORA(115, 55, 0, 155));
    DrawTextEx(assets.gameFont, title, { ttx, tty }, 56, 4, COLOR(252, 208, 48));
    // Side decorators
    DrawTextEx(assets.gameFont, "<<", { ttx - 56, tty + 10 }, 30, 1, COLORA(252, 175, 38, 175));
    DrawTextEx(assets.gameFont, ">>", { ttx + tsz.x + 16, tty + 10 }, 30, 1, COLORA(252, 175, 38, 175));

    // ── 8. ROW SETUP ─────────────────────────────────────────────────────────
    float rowX = px + 24.0f;
    float rowW = pw - 48.0f;
    float rowH = 85.0f;
    float r1y = py + 86.0f;
    float r2y = py + 194.0f;
    float r3y = py + 338.0f;
    float slW = rowW - 305.0f;

    // ── Helper: draw row background ───────────────────────────────────────────
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
            DrawRectangleRoundedLines({ rowX - 4, ry - 4, rowW + 8, rowH + 8 }, 0.12f, 8,
                COLORA(252, 155, 0, 22));
        }
        };

    // ── Helper: draw icon circle ───────────────────────────────────────────────
    auto DrawIconBg = [&](float ry, bool active) -> Color {
        Color ic = active ? COLOR(252, 175, 18) : COLORA(155, 88, 14, 175);
        DrawCircle((int)(rowX + 44), (int)(ry + rowH / 2), 26, COLORA(48, 24, 4, 205));
        DrawCircleLines((int)(rowX + 44), (int)(ry + rowH / 2), 26, ic);
        return ic;
        };

    // ── Helper: draw percent box ───────────────────────────────────────────────
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

    // ── ROW 1: MUSIC VOLUME ───────────────────────────────────────────────────
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

    // ── ROW 2: SFX VOLUME ─────────────────────────────────────────────────────
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

    // ── DIVIDER ───────────────────────────────────────────────────────────────
    float divY = r2y + rowH + 22.0f;
    DrawGlowLine(px + 42, divY, px + pw - 42, COLORA(155, 88, 14, 135));
    DrawDiamond(px + pw / 2, divY, 5, COLORA(195, 138, 24, 155));
    DrawDiamond(px + pw / 2 - 100, divY, 3, COLORA(155, 95, 18, 105));
    DrawDiamond(px + pw / 2 + 100, divY, 3, COLORA(155, 95, 18, 105));

    // ── ROW 3: BACK TO MENU ───────────────────────────────────────────────────
    bool backAct = (selectedOption == 2);
    DrawRow(r3y, backAct);
    Color backIc = backAct ? COLOR(252, 208, 48) : COLOR(168, 108, 34);
    DrawCircle((int)(rowX + 44), (int)(r3y + rowH / 2), 26, COLORA(48, 24, 4, 205));
    DrawCircleLines((int)(rowX + 44), (int)(r3y + rowH / 2), 26, backIc);
    // Arrow
    DrawTriangle(
        { rowX + 32, r3y + rowH / 2 },
        { rowX + 49, r3y + rowH / 2 - 12 },
        { rowX + 49, r3y + rowH / 2 + 12 }, backIc);
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

    // ── 9. HINT BAR ───────────────────────────────────────────────────────────
    float hintY = py + ph + 14.0f;
    float hintW = 548.0f;
    float hintX = screenWidth / 2.0f - hintW / 2.0f;
    DrawRectangleRounded({ hintX, hintY, hintW, 30 }, 0.5f, 8, COLORA(18, 9, 2, 158));
    DrawRectangleRoundedLines({ hintX, hintY, hintW, 30 }, 0.5f, 8, COLORA(155, 98, 18, 115));
    const char* hint = "W/S  Navigate    A/D  Adjust    ENTER  Confirm";
    Vector2 hsz = MeasureTextEx(assets.gameFont, hint, 17, 1);
    DrawTextEx(assets.gameFont, hint,
        { screenWidth / 2.0f - hsz.x / 2.0f, hintY + 6 }, 17, 1,
        COLORA(208, 162, 78, 178));

    // ── 10. INPUT ─────────────────────────────────────────────────────────────
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
