#include"UI_Utils.h"

//COLOR helpers
Color COLOR(unsigned char r, unsigned char g, unsigned char b) {
    Color c; c.r = r; c.g = g; c.b = b; c.a = 255; return c;
}
Color COLORA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    Color c; c.r = r; c.g = g; c.b = b; c.a = a; return c;
}

//Glowing horizontal line
void DrawGlowLine(float x1, float y, float x2, Color col) {
    DrawLineEx({ x1, y }, { x2, y }, 3.0f, col);
    DrawLineEx({ x1, y }, { x2, y }, 1.0f, Fade(WHITE, 0.25f));
}

//Diamond shape
void DrawDiamond(float x, float y, float r, Color c) {
    DrawTriangle({ x, y - r }, { x + r, y }, { x - r, y }, c);
    DrawTriangle({ x - r, y }, { x + r, y }, { x, y + r }, c);
}

// Ornate panel border with corner brackets
void DrawOrnateFrame(float x, float y, float w, float h, Color border, Color inner) {
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

//Fancy slider
void DrawFancySlider(float x, float y, float w, float val, bool active) {
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

//Music note icon
void DrawMusicNoteIcon(float x, float y, Color c) {
    DrawCircle((int)(x + 4), (int)(y + 26), 8, c);
    DrawCircle((int)(x + 24), (int)(y + 20), 8, c);
    DrawRectangle((int)(x + 10), (int)(y + 2), 4, 20, c);
    DrawRectangle((int)(x + 30), (int)(y + 0), 4, 16, c);
    DrawRectangle((int)(x + 10), (int)(y + 2), 24, 5, c);
}

//Speaker icon
void DrawSpeakerIcon(float x, float y, Color c, float vol) {
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

//Side scroll banner
void DrawSideBanner(float x, float topY, float botY,
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

//Screen corner ornament
void DrawCornerOrnament(float x, float y, float dx, float dy, Color c) {
    float len = 52.0f, thick = 3.0f;
    DrawLineEx({ x, y }, { x + dx * len, y }, thick, c);
    DrawLineEx({ x, y }, { x, y + dy * len }, thick, c);
    DrawLineEx({ x + dx * len * 0.5f, y }, { x + dx * len, y }, thick * 0.4f, Fade(c, 0.35f));
    DrawLineEx({ x, y + dy * len * 0.5f }, { x, y + dy * len }, thick * 0.4f, Fade(c, 0.35f));
    DrawCircle((int)x, (int)y, 5, c);
    DrawCircle((int)(x + dx * len), (int)y, 3, Fade(c, 0.55f));
    DrawCircle((int)x, (int)(y + dy * len), 3, Fade(c, 0.55f));
}

//Draw Card
void DrawCard(Font font, float x, float y, const char* id, const char* name, const char* role, int skillLevel, Color borderColor) {
    float cw = 255.0f; // Card width
    float ch = 115.0f; // Card height

    // Card Background & Border 
    DrawRectangleRounded({ x, y, cw, ch }, 0.15f, 8, ColorBrownDark);
    DrawRectangleRoundedLines({ x, y, cw, ch }, 0.15f, 8, borderColor);

    // Padding and starting Y coordinate for text
    float padX = x + 15.0f;
    float startY = y + 12.0f;

    // Draw text information
    DrawTextEx(font, TextFormat("ID: %s", id), { padX, startY }, 22, 1, ColorOrangeSolid);
    startY += 26.0f;

    DrawTextEx(font, name, { padX, startY }, 22, 1, ColorCreamSolid);
    startY += 26.0f;

    DrawTextEx(font, TextFormat("Role: %s", role), { padX, startY }, 18, 1, GRAY);
    startY += 22.0f;

    // Draw "Skill:" label
    DrawTextEx(font, "Skill:", { padX, startY }, 18, 1, ColorGoldSolid);

    float boxX = padX + 60.0f;
    float boxY = startY + 2.0f;

    for (int i = 0; i < 5; i++) {
        if (i < skillLevel) {
            DrawRectangle((int)boxX, (int)boxY, 12, 12, ColorGoldSolid);
        }
        else {
            DrawRectangle((int)boxX, (int)boxY, 12, 12, ColorGoldAlpha50);
        }
        boxX += 16.0f;
    }
}
Rectangle DrawBaseMenu(int screenWidth, int screenHeight, const GameAssets& assets, const char* title) {
    // Background & Vignette
    Rectangle srcBg = { 0, 0, (float)assets.bgMainMenu.width, (float)assets.bgMainMenu.height };
    Rectangle dstBg = { 0, 0, (float)screenWidth, (float)screenHeight };
    DrawTexturePro(assets.bgMainMenu, srcBg, dstBg, { 0, 0 }, 0.0f, WHITE);
    DrawRectangle(0, 0, screenWidth, screenHeight, ColorShadow);

    // Screen border and corner
    Color goldDim = ColorBronzeAlpha;
    DrawRectangleLinesEx({ 6, 6, (float)screenWidth - 12, (float)screenHeight - 12 }, 2.0f, ColorBronzeAlpha);
    DrawRectangleLinesEx({ 12, 12, (float)screenWidth - 24, (float)screenHeight - 24 }, 1.0f, ColorBronzeAlpha);
    DrawCornerOrnament(18, 18, 1, 1, goldDim);
    DrawCornerOrnament(screenWidth - 18, 18, -1, 1, goldDim);
    DrawCornerOrnament(18, screenHeight - 18, 1, -1, goldDim);
    DrawCornerOrnament(screenWidth - 18, screenHeight - 18, -1, -1, goldDim);

    // Panel dimensions
    float pw = 780.0f, ph = 490.0f;
    float px = screenWidth / 2.0f - pw / 2.0f;
    float py = screenHeight / 2.0f - ph / 2.0f;

    // Side panner
    float bannerX1 = px - 55.0f;
    float bannerX2 = px + pw + 55.0f;
    float bTop = py + 12.0f, bBot = py + ph - 12.0f;
    DrawSideBanner(bannerX1, bTop, bBot, false, assets);
    DrawSideBanner(bannerX2, bTop, bBot, true, assets);

    // Panel fill & Subtle top highlight
    DrawRectangleRounded({ px, py, pw, ph }, 0.07f, 8, ColorBrownDark);
    /*DrawRectangleRounded({ px, py, pw, ph * 0.42f }, 0.07f, 8, ColorHighlight);*/


    // Dashed connectors banner → panel
    float midY = py + ph / 2.0f;
    float lx1s = bannerX1 + 18, lx1e = px;
    float lx2s = px + pw, lx2e = bannerX2 - 18;
    for (int i = 0; i < 5; i++) {
        float a = lx1s + (lx1e - lx1s) * i / 5.0f;
        float b2 = lx1s + (lx1e - lx1s) * (i + 0.75f) / 5.0f;
        float c2 = lx2s + (lx2e - lx2s) * i / 5.0f;
        float d = lx2s + (lx2e - lx2s) * (i + 0.75f) / 5.0f;
        DrawLineEx({ a, midY }, { b2, midY }, 2.0f, ColorBronzeAlpha);
        DrawLineEx({ c2, midY }, { d, midY }, 2.0f, ColorBronzeAlpha);
    }
    for (int i = 1; i <= 2; i++) {
        DrawCircle((int)(lx1s + (lx1e - lx1s) * i / 3.0f), (int)midY, 4, ColorGoldAlpha);
        DrawCircle((int)(lx2s + (lx2e - lx2s) * i / 3.0f), (int)midY, 4, ColorGoldAlpha);
    }

    // Header
    float hh = 68.0f;
    DrawRectangleRounded({ px, py, pw, hh }, 0.07f, 8, ColorBrownRust);
    DrawGlowLine(px + 22, py + hh, px + pw - 22, ColorBronzeSolid);
    DrawDiamond(px + pw / 2, py + hh, 8, ColorGoldSolid);
    DrawDiamond(px + pw / 2 - 110, py + hh, 4, ColorGoldAlpha);
    DrawDiamond(px + pw / 2 + 110, py + hh, 4, ColorGoldAlpha);

    // Ornate border
    DrawOrnateFrame(px, py, pw, ph, ColorBronzeSolid, ColorGoldSolid);

    Vector2 tsz = MeasureTextEx(assets.gameFont, title, 56, 4);
    float ttx = px + pw / 2 - tsz.x / 2;
    float tty = py + hh / 2 - tsz.y / 2;

    // Side decorators
    DrawTextEx(assets.gameFont, "<<", { ttx - 56, tty + 10 }, 30, 1, ColorGoldAlpha);
    DrawTextEx(assets.gameFont, ">>", { ttx + tsz.x + 16, tty + 10 }, 30, 1, ColorGoldAlpha);

    // Title drawing
    DrawTextEx(assets.gameFont, title, { ttx + 4, tty + 4 }, 56, 4, ColorShadow);
    DrawTextEx(assets.gameFont, title, { ttx + 2, tty + 2 }, 56, 4, ColorBronzeAlpha);
    DrawTextEx(assets.gameFont, title, { ttx, tty }, 56, 4, ColorGoldSolid);

    

    return { px, py, pw, ph };
}
void DrawHint(int screenWidth, int screenHeight, float hintX, float hintY, float hintW, const char* hint, const GameAssets& assets) {
    // Hint Bar
 
    DrawRectangleRounded({ hintX, hintY, hintW, 30 }, 0.5f, 8, ColorBrownDark);
    DrawRectangleRoundedLines({ hintX, hintY, hintW, 30 }, 0.5f, 8, ColorBronzeAlpha);

    Vector2 hsz = MeasureTextEx(assets.gameFont, hint, 17, 1);
    DrawTextEx(assets.gameFont, hint, { screenWidth / 2.0f - hsz.x / 2.0f, hintY + 6 }, 17, 1, ColorGoldAlpha);
}
