#define _CRT_SECURE_NO_WARNINGS
#include "UI.h"
#include <cmath>
#include <cstring>
#include <string>
#include <algorithm>


// ─── Colors 
static Color NC(unsigned char r, unsigned char g, unsigned char b)
{
    Color c; c.r = r; c.g = g; c.b = b; c.a = 255; return c;
}
static Color NCA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    Color c; c.r = r; c.g = g; c.b = b; c.a = a; return c;
}
// ENUM AND CONTANT
enum NewGameSubState {
    SUB_MODE_SELECT, SUB_OPPONENT_SELECT, SUB_NAME_INPUT,
    SUB_READY, SUB_PLAYING, SUB_WIN_BLINK, SUB_WIN
};
enum GameMode { MODE_STANDARD = 0, MODE_BLOCK2 = 1, MODE_FREE = 2 };
enum OpponentType { OPP_AI = 0, OPP_AI_HARD = 1, OPP_PVP = 2 };

static const int   BOARD_SIZE = 15;
static const int   CELL = 36;
static const float TURN_TIME = 30.f;
static const float BLINK_DURATION = 2.4f;

struct CardDef {
    const char* category;
    const char* icon;
    const char* title;
    Color       accent;
    const char* lines[5];
    int         lineCount;
};
// ─── Card data ──────────────────────────────────────────────────────────────────
static const CardDef MODE_CARDS[3] = {
    {"STANDARD","[S]","Standard",    NC(80,200,255),
     {"Classic Caro rules.","Win by placing 5","pieces in a row.","No forbidden moves.",""}, 4},
    {"BLOCK BOTH","[B]","Block Both", NC(255,180,60),
     {"5 in a row BUT must","NOT be blocked on","both ends to count.","Harder than standard!",""}, 4},
    {"FREE BLOCK","[F]","Free Block",  NC(120,230,140),
     {"Chains longer than","5 pieces are allowed.","Wider strategy space!","",""}, 3},
};
static const CardDef OPP_CARDS[3] = {
    {"AI",     "[BOT]","Vs CPU (Easy)",  NC(160,220,255),
     {"CPU plays random","basic moves.","Good for beginners.","",""}, 3},
    {"AI HARD","[!!!]","Vs CPU (Hard)",  NC(255,120,100),
     {"CPU uses Minimax","algorithm.","A real challenge!","",""}, 3},
    {"PVP",    "[P2]", "2 Players",      NC(180,255,160),
     {"Play with a friend","on the same keyboard.","Take turns placing.","",""}, 3},
};


struct Particle {
    float x, y, vx, vy, life, maxLife, size;
    Color color;
};
static const int MAX_PARTICLES = 300;

struct WinLine { int x0, y0, x1, y1; };

struct GameSession {
    NewGameSubState sub = SUB_MODE_SELECT;
    GameMode        mode = MODE_STANDARD;
    OpponentType    opp = OPP_PVP;

    char        name1[32] = "Player 1";
    char        name2[32] = "Player 2";
    int         nameStep = 0;
    std::string nameBuf = "";

    float readyTimer = 0.f;

    int   board[BOARD_SIZE][BOARD_SIZE] = {};
    int   curP = 1, curX = 7, curY = 7;
    int   winner = 0;
    float turnTimer = TURN_TIME;

    bool  paused = false;
    int   pauseSel = 0;

    float   blinkTimer = 0.f;
    WinLine winLine = {};

    float    winTimer = 0.f;
    Particle particles[MAX_PARTICLES] = {};
    int      particleCount = 0;
    int      winDisplaySel = 0;

    int   mSel = 0, mTgt = 0, mSlot = 0;
    float mFlip = 1.f;
    bool  mMid = false;

    int   oSel = 0, oTgt = 0, oSlot = 0;
    float oFlip = 1.f;
    bool  oMid = false;

    int nameSlot = 0;
};

// ── Static session ───────────────────────────────────────────────────────────
static GameSession gs;
static bool s_needReset = true;   
static bool s_entryFrame = false;


static void N_GlowLine(float x1, float y, float x2, Color c) {
    DrawLineEx({ x1,y }, { x2,y }, 3.f, c);
    DrawLineEx({ x1,y }, { x2,y }, 1.f, Fade(WHITE, 0.18f));
}
static void N_Diamond(float x, float y, float r, Color c) {
    DrawTriangle({ x,y - r }, { x + r,y }, { x - r,y }, c);
    DrawTriangle({ x - r,y }, { x + r,y }, { x,y + r }, c);
}
static void N_OrnateFrame(float x, float y, float w, float h, Color b) {
    for (int i = 4; i >= 1; i--)
        DrawRectangleRoundedLines({ x - i * 2.5f,y - i * 2.5f,w + i * 5.f,h + i * 5.f }, 0.06f, 8, Fade(b, 0.042f * i));
    DrawRectangleRoundedLines({ x,y,w,h }, 0.06f, 8, b);
    DrawRectangleRoundedLines({ x + 6,y + 6,w - 12,h - 12 }, 0.06f, 8, Fade(b, 0.32f));
    float cs = 20.f, ct = 3.f;
    auto Br = [&](float ax, float ay, float bx2, float by, float cx2, float cy2) {
        DrawLineEx({ ax,ay }, { bx2,by }, ct, b); DrawLineEx({ ax,ay }, { cx2,cy2 }, ct, b);
        DrawCircle((int)ax, (int)ay, 4, b); };
    Br(x + 2, y + 2, x + cs + 2, y + 2, x + 2, y + cs + 2);
    Br(x + w - 2, y + 2, x + w - 2 - cs, y + 2, x + w - 2, y + cs + 2);
    Br(x + 2, y + h - 2, x + cs + 2, y + h - 2, x + 2, y + h - 2 - cs);
    Br(x + w - 2, y + h - 2, x + w - 2 - cs, y + h - 2, x + w - 2, y + h - 2 - cs);
}
static void N_Header(float px, float py, float pw, float hh, const char* title, const GameAssets& a) {
    DrawRectangleRounded({ px,py,pw,hh }, 0.07f, 8, NCA(75, 36, 7, 225));
    N_GlowLine(px + 22, py + hh, px + pw - 22, NC(195, 125, 18));
    N_Diamond(px + pw / 2, py + hh, 8, NC(250, 195, 38));
    Vector2 tsz = MeasureTextEx(a.gameFont, title, 46, 4);
    float tx = px + pw / 2 - tsz.x / 2, ty = py + hh / 2 - tsz.y / 2;
    DrawTextEx(a.gameFont, title, { tx + 4,ty + 4 }, 46, 4, NCA(0, 0, 0, 108));
    DrawTextEx(a.gameFont, title, { tx + 2,ty + 2 }, 46, 4, NCA(112, 55, 0, 145));
    DrawTextEx(a.gameFont, title, { tx,ty }, 46, 4, NC(252, 208, 48));
}
static void N_BgPanel(int sw, int sh, float px, float py, float pw, float ph, const GameAssets& a) {
    Rectangle src = { 0,0,(float)a.bgMainMenu.width,(float)a.bgMainMenu.height };
    DrawTexturePro(a.bgMainMenu, src, { 0,0,(float)sw,(float)sh }, { 0,0 }, 0.f, WHITE);
    DrawRectangle(0, 0, sw, sh, NCA(8, 4, 0, 118));
    float t = (float)GetTime(), p = 0.028f + 0.014f * sinf(t * 1.4f);
    for (int i = 5; i >= 1; i--)
        DrawRectangleRounded({ px - i * 3.f,py - i * 3.f,pw + i * 6.f,ph + i * 6.f }, 0.07f, 8, Fade(NC(175, 95, 8), p * i));
    DrawRectangleRounded({ px,py,pw,ph }, 0.07f, 8, NCA(16, 8, 2, 248));
    DrawRectangleRounded({ px,py,pw,ph * 0.38f }, 0.07f, 8, NCA(30, 15, 4, 46));
    N_OrnateFrame(px, py, pw, ph, NC(195, 115, 18));
}
static void N_HintBar(int sw, float bot, const char* hint, const GameAssets& a) {
    float hW = 620.f, hX = sw / 2.f - hW / 2.f, hY = bot + 12.f;
    DrawRectangleRounded({ hX,hY,hW,30 }, 0.5f, 8, NCA(18, 9, 2, 150));
    DrawRectangleRoundedLines({ hX,hY,hW,30 }, 0.5f, 8, NCA(155, 98, 18, 108));
    Vector2 hsz = MeasureTextEx(a.gameFont, hint, 16, 1);
    DrawTextEx(a.gameFont, hint, { sw / 2.f - hsz.x / 2.f,hY + 7 }, 16, 1, NCA(205, 160, 75, 172));
}
static void N_DrawBackBtn(float bx, float by, bool focused, const GameAssets& a) {
    float bw = 160.f, bh = 40.f, fx = bx - bw / 2.f;
    if (focused) for (int i = 3; i >= 1; i--)
        DrawRectangleRounded({ fx - i * 2.f,by - i * 2.f,bw + i * 4.f,bh + i * 4.f }, 0.3f, 6, Fade(NC(255, 190, 30), 0.06f * i));
    DrawRectangleRounded({ fx,by,bw,bh }, 0.3f, 6, focused ? NC(200, 120, 10) : NCA(80, 40, 6, 190));
    DrawRectangleRoundedLines({ fx,by,bw,bh }, 0.3f, 6, focused ? NC(252, 185, 30) : NCA(155, 88, 14, 160));
    float ay = by + bh / 2.f;
    DrawTriangle({ fx + 12,ay }, { fx + 22,ay - 9 }, { fx + 22,ay + 9 }, NC(252, 208, 48));
    DrawRectangle((int)(fx + 18), (int)(ay - 5), 16, 10, NC(252, 208, 48));
    Vector2 bsz = MeasureTextEx(a.gameFont, "BACK", 19, 1);
    DrawTextEx(a.gameFont, "BACK", { fx + 42.f,by + bh / 2.f - bsz.y / 2.f }, 19, 1,
        focused ? NC(255, 228, 80) : NC(200, 160, 55));
}
static void N_Dots(float cx, float cy, int count, int active, const CardDef* cards) {
    float sp = 22.f, sx = cx - (count - 1) * sp / 2.f;
    for (int i = 0; i < count; i++) {
        if (i == active) {
            DrawCircle((int)(sx + i * sp), (int)cy, 6, cards[i].accent);
            DrawCircleLines((int)(sx + i * sp), (int)cy, 9, Fade(cards[i].accent, 0.38f));
        }
        else DrawCircle((int)(sx + i * sp), (int)cy, 3, NCA(118, 68, 12, 148));
    }
}

// ─── Single flip card
static void DrawFlipCard(float cx, float cy, float cw, float ch,
    const CardDef* cards, int idx, float flipT, bool showBack,
    float scaleX, bool isFocused, const GameAssets& a)
{
    float t = (float)GetTime();
    float dw = cw * scaleX, dx = cx + (cw - dw) / 2.f;
    const CardDef& card = cards[idx];
    if (isFocused) for (int i = 4; i >= 1; i--)
        DrawRectangleRounded({ dx - i * 3.f,cy - i * 3.f,dw + i * 6.f,ch + i * 6.f }, 0.1f, 8, Fade(card.accent, 0.055f * i));
    DrawRectangleRounded({ dx + 5,cy + 5,dw,ch }, 0.1f, 8, NCA(0, 0, 0, 62));
    if (!showBack) {
        DrawRectangleRounded({ dx,cy,dw,ch }, 0.1f, 8, NCA(22, 11, 3, 252));
        DrawRectangleRounded({ dx,cy,dw,ch }, 0.1f, 8, NCA(card.accent.r, card.accent.g, card.accent.b, 14));
        DrawRectangleRoundedLines({ dx,cy,dw,ch }, 0.1f, 8, card.accent);
        DrawRectangleRounded({ dx,cy,dw,7 }, 0.5f, 4, card.accent);
        float bw2 = MeasureTextEx(a.gameFont, card.category, 13, 1).x + 16;
        DrawRectangleRounded({ dx + 10,cy + 14,bw2,22 }, 0.5f, 6, NCA(card.accent.r / 4, card.accent.g / 4, card.accent.b / 4, 202));
        DrawRectangleRoundedLines({ dx + 10,cy + 14,bw2,22 }, 0.5f, 6, Fade(card.accent, 0.6f));
        DrawTextEx(a.gameFont, card.category, { dx + 18,cy + 17 }, 13, 1, card.accent);
        float ifs = 66.f;
        Vector2 isz = MeasureTextEx(a.gameFont, card.icon, ifs, 2);
        float ix = dx + dw / 2.f - isz.x / 2.f, iy = cy + ch / 2.f - isz.y / 2.f - 14.f;
        for (int i = 4; i >= 1; i--)
            DrawCircle((int)(ix + isz.x / 2), (int)(iy + isz.y / 2), (int)(isz.x / 2 + i * 9), Fade(card.accent, 0.036f * i));
        DrawCircle((int)(ix + isz.x / 2), (int)(iy + isz.y / 2), (int)(isz.x / 2 + 13), NCA(card.accent.r / 5, card.accent.g / 5, card.accent.b / 5, 192));
        DrawCircleLines((int)(ix + isz.x / 2), (int)(iy + isz.y / 2), (int)(isz.x / 2 + 13), Fade(card.accent, 0.5f));
        DrawTextEx(a.gameFont, card.icon, { ix + 3,iy + 3 }, ifs, 2, NCA(0, 0, 0, 88));
        DrawTextEx(a.gameFont, card.icon, { ix,iy }, ifs, 2, card.accent);
        N_GlowLine(dx + 20, iy + isz.y + 18, dx + dw - 20, Fade(card.accent, 0.34f));
        float ha = 0.34f + 0.22f * sinf(t * 2.f);
        const char* fh = isFocused ? "ENTER to select" : "A/D browse";
        Vector2 fhsz = MeasureTextEx(a.gameFont, fh, 13, 1);
        DrawTextEx(a.gameFont, fh, { dx + dw / 2.f - fhsz.x / 2.f,cy + ch - 30.f }, 13, 1,
            Fade(NCA(card.accent.r, card.accent.g, card.accent.b, 255), ha));
    }
    else {
        DrawRectangleRounded({ dx,cy,dw,ch }, 0.1f, 8, NCA(26, 13, 3, 252));
        DrawRectangleRounded({ dx,cy,dw,ch }, 0.1f, 8, NCA(card.accent.r, card.accent.g, card.accent.b, 10));
        DrawRectangleRoundedLines({ dx,cy,dw,ch }, 0.1f, 8, card.accent);
        DrawRectangleRounded({ dx,cy,dw,7 }, 0.5f, 4, card.accent);
        DrawRectangleRounded({ dx,cy,6,ch }, 0.8f, 4, Fade(card.accent, 0.65f));
        float tbh = 44.f;
        DrawRectangleRounded({ dx,cy,dw,tbh }, 0.1f, 8, NCA(card.accent.r / 4, card.accent.g / 4, card.accent.b / 4, 165));
        DrawTextEx(a.gameFont, card.category, { dx + 14,cy + 6 }, 13, 1, NCA(card.accent.r, card.accent.g, card.accent.b, 145));
        Vector2 ttsz = MeasureTextEx(a.gameFont, card.title, 23, 1);
        DrawTextEx(a.gameFont, card.title, { dx + dw / 2.f - ttsz.x / 2.f,cy + 15 }, 23, 1, card.accent);
        N_GlowLine(dx + 10, cy + tbh, dx + dw - 10, Fade(card.accent, 0.4f));
        N_Diamond(dx + dw / 2.f, cy + tbh, 5, card.accent);
        float lx = dx + 22.f, ly = cy + tbh + 12.f;
        for (int i = 0; i < card.lineCount; i++) {
            if (!card.lines[i] || card.lines[i][0] == '\0') { ly += 9.f; continue; }
            DrawCircle((int)(lx - 9), (int)(ly + 7), 3, Fade(card.accent, 0.5f));
            DrawTextEx(a.gameFont, card.lines[i], { lx,ly }, 16, 1, NCA(214, 184, 128, 215));
            ly += 24.f;
        }
        N_GlowLine(dx + 20, cy + ch - 17, dx + dw - 20, Fade(card.accent, 0.24f));
    }
}
// TREE CARD 
static void DrawCardRow(int sw, float cardY, float cw, float ch,
    const CardDef* cards, int selIdx, int slotFocus,
    float flipT, bool showBack, const GameAssets& a)
{
    float scaleX = (flipT < 0.5f) ? (1.f - flipT * 2.f) : ((flipT - 0.5f) * 2.f);
    float totalW = cw * 3 + 24.f * 2;
    float startX = sw / 2.f - totalW / 2.f;
    for (int i = 0; i < 3; i++) {
        float tw = cw, th = ch, tx = startX + i * (cw + 24.f), ty = cardY;
        bool isSel = (i == selIdx), isFoc = (slotFocus == i);
        if (isSel) {
            tw += 20.f; th += 20.f; tx -= 10.f; ty -= 10.f;
            DrawFlipCard(tx, ty, tw, th, cards, selIdx, flipT, showBack, scaleX, isFoc, a);
        }
        else {
            const CardDef& c = cards[i];
            if (isFoc) for (int j = 3; j >= 1; j--)
                DrawRectangleRounded({ tx - j * 2.5f,ty - j * 2.5f,tw + j * 5.f,th + j * 5.f }, 0.1f, 8, Fade(c.accent, 0.05f * j));
            DrawRectangleRounded({ tx,ty,tw,th }, 0.1f, 8, NCA(18, 9, 2, 192));
            DrawRectangleRounded({ tx,ty,tw,7 }, 0.5f, 4, Fade(c.accent, isFoc ? 0.8f : 0.42f));
            DrawRectangleRoundedLines({ tx,ty,tw,th }, 0.1f, 8, Fade(c.accent, isFoc ? 0.7f : 0.36f));
            Vector2 isz = MeasureTextEx(a.gameFont, c.icon, 52.f, 2);
            DrawTextEx(a.gameFont, c.icon, { tx + tw / 2.f - isz.x / 2.f,ty + th / 2.f - isz.y / 2.f - 8.f },
                52.f, 2, Fade(c.accent, isFoc ? 0.6f : 0.30f));
            Vector2 tsz2 = MeasureTextEx(a.gameFont, c.title, 18, 1);
            DrawTextEx(a.gameFont, c.title, { tx + tw / 2.f - tsz2.x / 2.f,ty + th - 38.f }, 18, 1, Fade(c.accent, isFoc ? 0.7f : 0.40f));
        }
    }
}
// MOD SELECT
static void DrawModeSelect(int sw, int sh, const GameAssets& a, bool& backToMenu) {
    float dt = GetFrameTime();
    if (gs.mFlip < 1.f) {
        gs.mFlip += dt * 3.6f;
        if (gs.mFlip >= 0.5f && !gs.mMid) { gs.mSel = gs.mTgt; gs.mMid = true; }
        if (gs.mFlip >= 1.f) gs.mFlip = 1.f;
    }
    if (gs.mFlip >= 1.f && !s_entryFrame) {
        if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_S)) && gs.mSlot < 3) {
            gs.mSlot++; if (a.clickSound.stream.buffer)PlaySound(a.clickSound);
        }
        if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_W)) && gs.mSlot > 0) {
            gs.mSlot--; if (a.clickSound.stream.buffer)PlaySound(a.clickSound);
        }
        if (gs.mSlot <= 2 && gs.mSlot != gs.mSel) {
            gs.mTgt = gs.mSlot; gs.mFlip = 0.f; gs.mMid = false;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            if (gs.mSlot == 3) { backToMenu = true; }
            else {
                gs.mode = (GameMode)gs.mSel;
                gs.oSel = 0; gs.oSlot = 0; gs.oFlip = 1.f; gs.oMid = false;
                gs.sub = SUB_OPPONENT_SELECT;
            }
            if (a.clickSound.stream.buffer)PlaySound(a.clickSound);
        }
    }
    float pw = 880.f, ph = 510.f, px = sw / 2.f - pw / 2.f, py = sh / 2.f - ph / 2.f;
    N_BgPanel(sw, sh, px, py, pw, ph, a);
    N_Header(px, py, pw, 62.f, "SELECT GAME MODE", a);
    bool showBack = gs.mFlip >= 0.5f;
    float cw = 260.f, ch = 305.f, cardY = py + 74.f;
    DrawCardRow(sw, cardY, cw, ch, MODE_CARDS, gs.mSel, gs.mSlot, gs.mFlip, showBack, a);
    N_Dots(sw / 2.f, cardY + ch + 18.f, 3, gs.mSel, MODE_CARDS);
    N_DrawBackBtn(sw / 2.f, cardY + ch + 36.f, gs.mSlot == 3, a);
    N_HintBar(sw, py + ph, "A/D  Browse      ENTER  Confirm", a);
}
// OPPONET SELECT
static void DrawOpponentSelect(int sw, int sh, const GameAssets& a) {
    float dt = GetFrameTime();
    if (gs.oFlip < 1.f) {
        gs.oFlip += dt * 3.6f;
        if (gs.oFlip >= 0.5f && !gs.oMid) { gs.oSel = gs.oTgt; gs.oMid = true; }
        if (gs.oFlip >= 1.f) gs.oFlip = 1.f;
    }
    if (gs.oFlip >= 1.f) {
        if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_S)) && gs.oSlot < 3) {
            gs.oSlot++; if (a.clickSound.stream.buffer)PlaySound(a.clickSound);
        }
        if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_W)) && gs.oSlot > 0) {
            gs.oSlot--; if (a.clickSound.stream.buffer)PlaySound(a.clickSound);
        }
        if (gs.oSlot <= 2 && gs.oSlot != gs.oSel) {
            gs.oTgt = gs.oSlot; gs.oFlip = 0.f; gs.oMid = false;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            if (gs.oSlot == 3) {
                gs.sub = SUB_MODE_SELECT; gs.mSel = (int)gs.mode;
                gs.mSlot = (int)gs.mode; gs.mFlip = 1.f; gs.mMid = false;
            }
            else {
                gs.opp = (OpponentType)gs.oSel;
                strcpy(gs.name1, gs.opp == OPP_PVP ? "Player 1" : "Player");
                strcpy(gs.name2, "Player 2"); // PVP default, CPU name set later after player types their name
                gs.nameStep = 0; gs.nameBuf = gs.name1; gs.nameSlot = 0;
                gs.sub = SUB_NAME_INPUT;
            }
            if (a.clickSound.stream.buffer)PlaySound(a.clickSound);
        }
    }
    float pw = 880.f, ph = 510.f, px = sw / 2.f - pw / 2.f, py = sh / 2.f - ph / 2.f;
    N_BgPanel(sw, sh, px, py, pw, ph, a);
    const char* mN[] = { "STANDARD","BLOCK BOTH","FREE BLOCK" };
    char hdr[64]; sprintf_s(hdr, sizeof(hdr), "SELECT OPPONENT [%s]", mN[(int)gs.mode]);
    N_Header(px, py, pw, 62.f, hdr, a);
    bool showBack = gs.oFlip >= 0.5f;
    float cw = 260.f, ch = 305.f, cardY = py + 74.f;
    DrawCardRow(sw, cardY, cw, ch, OPP_CARDS, gs.oSel, gs.oSlot, gs.oFlip, showBack, a);
    N_Dots(sw / 2.f, cardY + ch + 18.f, 3, gs.oSel, OPP_CARDS);
    N_DrawBackBtn(sw / 2.f, cardY + ch + 36.f, gs.oSlot == 3, a);
    N_HintBar(sw, py + ph, "A/D  Browse      ENTER  Confirm", a);
}
// NAME INPUT
static void DrawNameInput(int sw, int sh, const GameAssets& a) {
    float t = (float)GetTime();
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_W)) {
        gs.nameSlot ^= 1;
        if (a.clickSound.stream.buffer)PlaySound(a.clickSound);
    }
    if (gs.nameSlot == 0) {
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 125 && (int)gs.nameBuf.size() < 20) gs.nameBuf += (char)key;
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !gs.nameBuf.empty()) gs.nameBuf.pop_back();
    }
    if (IsKeyPressed(KEY_ENTER)) {
        if (gs.nameSlot == 1) {
            gs.sub = SUB_OPPONENT_SELECT;
            gs.oSel = (int)gs.opp; gs.oSlot = (int)gs.opp; gs.oFlip = 1.f; gs.oMid = false;
            if (a.clickSound.stream.buffer)PlaySound(a.clickSound);
            return;
        }
        if (gs.nameStep == 0) {
            strncpy(gs.name1, gs.nameBuf.empty() ? "Player 1" : gs.nameBuf.c_str(), 31);
            if (gs.opp == OPP_PVP) {
                gs.nameStep = 1; gs.nameBuf = gs.name2; gs.nameSlot = 0;
            }
            else {
                // VS CPU: pick a bot name that doesn't match player name
                const char* botNames[] = { "HAL-9000","KASPAROV","DEEP BLUE","ALPHA","NEXUS",
                                        "ORACLE","TITAN","CIPHER","AXIOM","VECTOR" };
                int pick = GetRandomValue(0, 9);
                // keep picking until different from player name
                for (int attempt = 0; attempt < 10; attempt++) {
                    if (strcmp(botNames[pick], gs.name1) != 0) break;
                    pick = (pick + 1) % 10;
                }
                strcpy(gs.name2, botNames[pick]);

                memset(gs.board, 0, sizeof(gs.board));
                gs.curP = 1; gs.curX = 7; gs.curY = 7;
                gs.winner = 0; gs.paused = false;
                gs.turnTimer = TURN_TIME; gs.readyTimer = 0.f;
                gs.sub = SUB_READY;
                if (a.clickSound.stream.buffer)PlaySound(a.clickSound);
                return;
            }
        }
        else {
            // PVP step 2: save name2
            strncpy(gs.name2, gs.nameBuf.empty() ? "Player 2" : gs.nameBuf.c_str(), 31);
            memset(gs.board, 0, sizeof(gs.board));
            gs.curP = 1; gs.curX = 7; gs.curY = 7;
            gs.winner = 0; gs.paused = false;
            gs.turnTimer = TURN_TIME; gs.readyTimer = 0.f;
            gs.sub = SUB_READY;
            if (a.clickSound.stream.buffer)PlaySound(a.clickSound);
        }
    }

    float pw = 600.f, ph = 390.f, px = sw / 2.f - pw / 2.f, py = sh / 2.f - ph / 2.f;
    N_BgPanel(sw, sh, px, py, pw, ph, a);
    N_Header(px, py, pw, 62.f, "ENTER NAME", a);

    if (gs.opp == OPP_PVP) {
        char step[32]; sprintf_s(step, sizeof(step), "Step %d / 2", gs.nameStep + 1);
        Vector2 ssz = MeasureTextEx(a.gameFont, step, 17, 1);
        DrawTextEx(a.gameFont, step, { sw / 2.f - ssz.x / 2.f,py + 70.f }, 17, 1, NCA(200, 155, 50, 150));
    }

    Color pColor = (gs.nameStep == 0) ? NC(80, 200, 255) : NC(255, 160, 80);
    const char* pIcon = (gs.nameStep == 0) ? "[P1]" : "[P2]";
    float iy = py + 96.f;
    DrawCircle((int)(sw / 2.f), (int)(iy + 24), 32, NCA(pColor.r / 5, pColor.g / 5, pColor.b / 5, 192));
    DrawCircleLines((int)(sw / 2.f), (int)(iy + 24), 32, Fade(pColor, 0.65f));
    Vector2 pisz = MeasureTextEx(a.gameFont, pIcon, 25, 1);
    DrawTextEx(a.gameFont, pIcon, { sw / 2.f - pisz.x / 2.f,iy + 11 }, 25, 1, pColor);

    const char* lbl = (gs.nameStep == 0) ? "Player 1 Name:" : "Player 2 Name:";
    Vector2 lsz = MeasureTextEx(a.gameFont, lbl, 20, 1);
    DrawTextEx(a.gameFont, lbl, { sw / 2.f - lsz.x / 2.f,iy + 66.f }, 20, 1, NC(220, 185, 100));

    bool inputFoc = (gs.nameSlot == 0);
    float bxW = 340.f, bxH = 50.f, bxX = sw / 2.f - bxW / 2.f, bxY = iy + 94.f;
    if (inputFoc) for (int i = 3; i >= 1; i--)
        DrawRectangleRoundedLines({ bxX - i * 2.f,bxY - i * 2.f,bxW + i * 4.f,bxH + i * 4.f }, 0.2f, 8, Fade(pColor, 0.05f * i));
    DrawRectangleRounded({ bxX,bxY,bxW,bxH }, 0.2f, 8, NCA(28, 14, 3, 225));
    DrawRectangleRoundedLines({ bxX,bxY,bxW,bxH }, 0.2f, 8, Fade(pColor, inputFoc ? 0.95f : 0.40f));
    bool blink = (sinf(t * 4.f) > 0) && inputFoc;
    std::string disp = gs.nameBuf + (blink ? "|" : "");
    Vector2 isz2 = MeasureTextEx(a.gameFont, disp.c_str(), 24, 1);
    DrawTextEx(a.gameFont, disp.c_str(), { bxX + bxW / 2.f - isz2.x / 2.f,bxY + bxH / 2.f - isz2.y / 2.f }, 24, 1,
        inputFoc ? NC(252, 220, 80) : NCA(200, 168, 60, 180));
    if (inputFoc)
        DrawTextEx(a.gameFont, "[W/S to switch focus]", { bxX + 4.f,bxY + bxH + 5.f }, 13, 1, NCA(160, 120, 40, 128));
    char cnt[12]; sprintf_s(cnt, sizeof(cnt), "%d/20", (int)gs.nameBuf.size());
    DrawTextEx(a.gameFont, cnt, { bxX + bxW - 48.f,bxY + bxH + 5.f }, 13, 1, NCA(155, 118, 38, 128));

    N_DrawBackBtn(sw / 2.f, bxY + bxH + 38.f, gs.nameSlot == 1, a);
    N_HintBar(sw, py + ph, "W/S  Switch focus      ENTER  Confirm", a);
}
// READY SCREEN
static void DrawReadyScreen(int sw, int sh, const GameAssets& a) {
    gs.readyTimer += GetFrameTime();
    float t = (float)GetTime();
    if (gs.readyTimer >= 3.f || IsKeyPressed(KEY_ENTER)) {
        gs.sub = SUB_PLAYING; gs.turnTimer = TURN_TIME;
        if (a.clickSound.stream.buffer)PlaySound(a.clickSound); return;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        gs.sub = SUB_NAME_INPUT; gs.nameStep = 0; gs.nameBuf = gs.name1; gs.nameSlot = 0; return;
    }

    Rectangle src = { 0,0,(float)a.bgMainMenu.width,(float)a.bgMainMenu.height };
    DrawTexturePro(a.bgMainMenu, src, { 0,0,(float)sw,(float)sh }, { 0,0 }, 0.f, WHITE);
    DrawRectangle(0, 0, sw, sh, NCA(5, 3, 0, 158));

    float pw = 700.f, ph = 360.f, px = sw / 2.f - pw / 2.f, py = sh / 2.f - ph / 2.f;
    float glow = 0.055f + 0.038f * sinf(t * 2.5f);
    for (int i = 5; i >= 1; i--)
        DrawRectangleRounded({ px - i * 3.f,py - i * 3.f,pw + i * 6.f,ph + i * 6.f }, 0.08f, 8, Fade(NC(255, 200, 0), glow * i));
    DrawRectangleRounded({ px,py,pw,ph }, 0.08f, 8, NCA(14, 7, 1, 252));
    N_OrnateFrame(px, py, pw, ph, NC(220, 145, 20));
    DrawRectangleRounded({ px,py,pw,56 }, 0.08f, 8, NCA(80, 40, 8, 225));
    N_GlowLine(px + 20, py + 56, px + pw - 20, NC(200, 130, 20));

    const char* mN[] = { "STANDARD","BLOCK BOTH","FREE BLOCK" };
    const char* oN[] = { "VS CPU (EASY)","VS CPU (HARD)","PVP" };
    char mt[64]; sprintf_s(mt, sizeof(mt), "%s  -  %s", mN[(int)gs.mode], oN[(int)gs.opp]);
    Vector2 msz = MeasureTextEx(a.gameFont, mt, 19, 1);
    DrawTextEx(a.gameFont, mt, { sw / 2.f - msz.x / 2.f,py + 18 }, 19, 1, NC(220, 175, 60));

    float p1x = px + 80.f, p2x = px + pw - 80.f, py2 = py + 70.f;
    // P1
    DrawCircle((int)p1x, (int)(py2 + 36), 36, NCA(28, 28, 28, 218));
    DrawCircleLines((int)p1x, (int)(py2 + 36), 36, NC(80, 200, 255));
    DrawCircle((int)p1x - 7, (int)(py2 + 26), 9, NCA(88, 88, 88, 148));
    DrawTextEx(a.gameFont, "P1", { p1x - MeasureTextEx(a.gameFont,"P1",20,1).x / 2.f,py2 + 26 }, 20, 1, NC(80, 200, 255));
    Vector2 n1sz = MeasureTextEx(a.gameFont, gs.name1, 24, 1);
    DrawTextEx(a.gameFont, gs.name1, { p1x - n1sz.x / 2.f,py2 + 82 }, 24, 1, NC(80, 200, 255));
    DrawTextEx(a.gameFont, "(Black)", { p1x - MeasureTextEx(a.gameFont,"(Black)",15,1).x / 2.f,py2 + 110 }, 15, 1, NCA(138, 138, 138, 175));
    // P2
    DrawCircle((int)p2x, (int)(py2 + 36), 36, NCA(232, 232, 232, 218));
    DrawCircleLines((int)p2x, (int)(py2 + 36), 36, NC(255, 160, 80));
    DrawCircle((int)p2x - 7, (int)(py2 + 26), 9, NCA(255, 255, 255, 195));
    DrawTextEx(a.gameFont, "P2", { p2x - MeasureTextEx(a.gameFont,"P2",20,1).x / 2.f,py2 + 26 }, 20, 1, NC(255, 160, 80));
    Vector2 n2sz = MeasureTextEx(a.gameFont, gs.name2, 24, 1);
    DrawTextEx(a.gameFont, gs.name2, { p2x - n2sz.x / 2.f,py2 + 82 }, 24, 1, NC(255, 160, 80));
    DrawTextEx(a.gameFont, "(White)", { p2x - MeasureTextEx(a.gameFont,"(White)",15,1).x / 2.f,py2 + 110 }, 15, 1, NCA(195, 195, 195, 175));
    // VS
    float vsp = 1.f + 0.12f * sinf(t * 3.f), vsfs = 56.f * vsp;
    Vector2 vssz = MeasureTextEx(a.gameFont, "VS", vsfs, 2);
    DrawTextEx(a.gameFont, "VS", { sw / 2.f - vssz.x / 2.f + 3,py2 + 14 + 3 }, vsfs, 2, NCA(0, 0, 0, 98));
    DrawTextEx(a.gameFont, "VS", { sw / 2.f - vssz.x / 2.f,py2 + 14 }, vsfs, 2, NC(252, 208, 48));
    // Countdown
    int cnt2 = (int)ceilf(3.f - gs.readyTimer);
    char cd[4]; sprintf_s(cd, sizeof(cd), "%d", cnt2);
    float cfs = 68.f * (1.f + 0.28f * (1.f - fmodf(gs.readyTimer, 1.f)));
    Vector2 cdsz = MeasureTextEx(a.gameFont, cd, cfs, 1);
    DrawTextEx(a.gameFont, cd, { sw / 2.f - cdsz.x / 2.f,py + ph - 88.f }, cfs, 1,
        Fade(NC(252, 208, 48), 0.8f + 0.18f * sinf(t * 6.f)));
    float ra = 0.48f + 0.38f * sinf(t * 4.f);
    DrawTextEx(a.gameFont, "GET READY!",
        { sw / 2.f - MeasureTextEx(a.gameFont,"GET READY!",26,2).x / 2.f,py + ph - 42.f },
        26, 2, Fade(NC(120, 240, 120), ra));
    float barX = px + 20.f, barY = py + ph - 11.f, barW = pw - 40.f, barH = 6.f;
    DrawRectangleRounded({ barX,barY,barW,barH }, 1.f, 4, NCA(38, 18, 3, 175));
    DrawRectangleRounded({ barX,barY,barW * (1.f - gs.readyTimer / 3.f),barH }, 1.f, 4, NC(80, 200, 255));
    float ha = 0.32f + 0.18f * sinf(t * 2.f);
    DrawTextEx(a.gameFont, "ENTER to skip",
        { sw / 2.f - MeasureTextEx(a.gameFont,"ENTER to skip",15,1).x / 2.f,py + ph + 15.f },
        15, 1, Fade(NCA(200, 160, 75, 255), ha));
}

// CHECK WIN
static bool CheckWin5(int b[][BOARD_SIZE], int p, int lx, int ly, WinLine& wl) {
    int dx[] = { 1,0,1,1 }, dy[] = { 0,1,1,-1 };
    for (int d = 0; d < 4; d++) {
        int c = 1, x0 = lx, y0 = ly, x1 = lx, y1 = ly;
        for (int s = 1; s < 5; s++) {
            int nx = lx + dx[d] * s, ny = ly + dy[d] * s;
            if (nx < 0 || nx >= BOARD_SIZE || ny < 0 || ny >= BOARD_SIZE || b[nx][ny] != p)break; c++; x1 = nx; y1 = ny;
        }
        for (int s = 1; s < 5; s++) {
            int nx = lx - dx[d] * s, ny = ly - dy[d] * s;
            if (nx < 0 || nx >= BOARD_SIZE || ny < 0 || ny >= BOARD_SIZE || b[nx][ny] != p)break; c++; x0 = nx; y0 = ny;
        }
        if (c >= 5) { wl = { x0,y0,x1,y1 }; return true; }
    }
    return false;
}


static void SpawnFirework(int sw, int sh) {
    if (gs.particleCount >= MAX_PARTICLES) gs.particleCount = 0;
    float fx = 40.f + (float)GetRandomValue(0, sw - 80);
    float fy = 40.f + (float)GetRandomValue(0, sh / 2);
    Color cols[] = { NC(255,80,80),NC(255,200,40),NC(80,220,255),NC(180,80,255),NC(80,255,140),NC(255,140,60) };
    Color base = cols[GetRandomValue(0, 5)];
    int count = GetRandomValue(18, 28);
    for (int i = 0; i < count && gs.particleCount < MAX_PARTICLES; i++) {
        float ang = (float)GetRandomValue(0, 360) * (3.14159f / 180.f);
        float spd = (float)GetRandomValue(60, 180) / 60.f;
        gs.particles[gs.particleCount++] = { fx,fy,cosf(ang) * spd,sinf(ang) * spd,
            1.f,1.f,(float)GetRandomValue(3,7) / 2.f,base };
    }
}
static void UpdateDrawParticles(float dt) {
    for (int i = 0; i < gs.particleCount; i++) {
        Particle& p = gs.particles[i];
        p.x += p.vx; p.y += p.vy; p.vy += 0.04f; p.vx *= 0.98f; p.vy *= 0.98f;
        p.life -= dt * 0.6f;
        if (p.life > 0.f) DrawCircle((int)p.x, (int)p.y, (int)p.size, Fade(p.color, p.life));
    }
}

///////////// Draw board
static void DrawBoard(float oX, float oY) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        DrawLineEx({ oX + i * CELL,oY }, { oX + i * CELL,oY + (BOARD_SIZE - 1) * CELL }, 1.2f, NCA(96, 60, 16, 195));
        DrawLineEx({ oX,oY + i * CELL }, { oX + (BOARD_SIZE - 1) * CELL,oY + i * CELL }, 1.2f, NCA(96, 60, 16, 195));
    }
    int stars[] = { 3,7,11 };
    for (int sx : stars) for (int sy : stars)
        DrawCircle((int)(oX + sx * CELL), (int)(oY + sy * CELL), 4, NC(96, 60, 16));
    for (int x = 0; x < BOARD_SIZE; x++) for (int y = 0; y < BOARD_SIZE; y++) {
        if (!gs.board[x][y]) continue;
        float ppx = oX + x * CELL, ppy = oY + y * CELL;
        bool isB = (gs.board[x][y] == 1);
        DrawCircle((int)ppx + 2, (int)ppy + 2, 13, NCA(0, 0, 0, 75));
        DrawCircle((int)ppx, (int)ppy, 13, isB ? NC(26, 26, 26) : NC(236, 236, 236));
        DrawCircle((int)ppx - 4, (int)ppy - 4, 4, isB ? NCA(86, 86, 86, 148) : NCA(255, 255, 255, 195));
        DrawCircleLines((int)ppx, (int)ppy, 13, isB ? NC(6, 6, 6) : NC(175, 175, 175));
    }
}
// WIN  BLINK
static void DrawWinBlink(int sw, int sh, const GameAssets& a) {
    gs.blinkTimer += GetFrameTime();

    if (gs.blinkTimer >= BLINK_DURATION) {
        gs.sub = SUB_WIN; gs.winTimer = 0.f; gs.particleCount = 0; gs.winDisplaySel = 0;
        return;
    }

    Rectangle src = { 0,0,(float)a.bgMainMenu.width,(float)a.bgMainMenu.height };
    DrawTexturePro(a.bgMainMenu, src, { 0,0,(float)sw,(float)sh }, { 0,0 }, 0.f, WHITE);
    DrawRectangle(0, 0, sw, sh, NCA(8, 4, 0, 128));

    float bpX = sw / 2.f - (BOARD_SIZE * CELL) / 2.f - 55.f;
    float bpY = sh / 2.f - (BOARD_SIZE * CELL) / 2.f;
    float bpW = (float)BOARD_SIZE * CELL + 58.f, bpH = (float)BOARD_SIZE * CELL + 58.f;
    DrawRectangleRounded({ bpX,bpY,bpW,bpH }, 0.04f, 6, NCA(196, 155, 86, 240));
    DrawRectangleRoundedLines({ bpX,bpY,bpW,bpH }, 0.04f, 6, NC(135, 85, 26));
    float oX = bpX + 29.f, oY = bpY + 29.f;
    DrawBoard(oX, oY);

    float blinkRate = 8.f;
    float alpha = (sinf(gs.blinkTimer * blinkRate * 3.14159f) > 0) ? 1.f : 0.f;
    Color winnerAccent = (gs.winner == 1) ? NC(80, 200, 255) : NC(255, 160, 80);
    int dx2[] = { 1,0,1,1 }, dy2[] = { 0,1,1,-1 };
    WinLine& wl = gs.winLine;
    float wx0 = oX + wl.x0 * CELL, wy0 = oY + wl.y0 * CELL;
    float wx1 = oX + wl.x1 * CELL, wy1 = oY + wl.y1 * CELL;
    if (alpha > 0.5f) {
        for (int g = 6; g >= 1; g--)
            DrawLineEx({ wx0,wy0 }, { wx1,wy1 }, (float)g * 3.f, Fade(winnerAccent, 0.06f * g));
        DrawLineEx({ wx0,wy0 }, { wx1,wy1 }, 5.f, Fade(winnerAccent, 0.95f));
    }
    int stepX = (wl.x1 == wl.x0) ? 0 : (wl.x1 > wl.x0 ? 1 : -1);
    int stepY = (wl.y1 == wl.y0) ? 0 : (wl.y1 > wl.y0 ? 1 : -1);
    int cx2 = wl.x0, cy2 = wl.y0;
    for (int k = 0; k < 5; k++) {
        float ppx = oX + cx2 * CELL, ppy = oY + cy2 * CELL;
        if (alpha > 0.5f) {
            for (int g = 4; g >= 1; g--)
                DrawCircle((int)ppx, (int)ppy, 13 + g * 3, Fade(winnerAccent, 0.05f * g));
            DrawCircleLines((int)ppx, (int)ppy, 16, winnerAccent);
            DrawCircleLines((int)ppx, (int)ppy, 20, Fade(winnerAccent, 0.5f));
        }
        cx2 += stepX; cy2 += stepY;
    }

    const char* winTxt = "5 IN A ROW!";
    float wt = (float)GetTime();
    float wts = 1.f + 0.05f * sinf(wt * 6.f);
    float wtfs = 40.f * wts;
    Vector2 wtsz = MeasureTextEx(a.gameFont, winTxt, wtfs, 2);
    DrawTextEx(a.gameFont, winTxt, { sw / 2.f - wtsz.x / 2.f + 2,bpY - 50.f + 2 }, wtfs, 2, NCA(0, 0, 0, 108));
    DrawTextEx(a.gameFont, winTxt, { sw / 2.f - wtsz.x / 2.f,bpY - 50.f }, wtfs, 2,
        Fade(winnerAccent, 0.7f + 0.28f * sinf(wt * 5.f)));
}
// Victory GAME SCREEN
static void DrawWinScreen(int sw, int sh, const GameAssets& a, bool& backToMenu) {
    float dt = GetFrameTime(), t = (float)GetTime();
    gs.winTimer += dt;

    static float nextFire = 0.f; nextFire -= dt;
    if (nextFire <= 0.f) { SpawnFirework(sw, sh); nextFire = 0.18f + (float)GetRandomValue(0, 20) / 100.f; }

    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_S)) {
        gs.winDisplaySel = 1; if (a.clickSound.stream.buffer)PlaySound(a.clickSound);
    }
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_W)) {
        gs.winDisplaySel = 0; if (a.clickSound.stream.buffer)PlaySound(a.clickSound);
    }
    if (IsKeyPressed(KEY_ENTER)) {
        if (gs.winDisplaySel == 0) {
            memset(gs.board, 0, sizeof(gs.board));
            gs.curP = 1; gs.curX = 7; gs.curY = 7;
            gs.winner = 0; gs.winTimer = 0.f; gs.particleCount = 0;
            gs.turnTimer = TURN_TIME; gs.readyTimer = 0.f; gs.sub = SUB_READY;
        }
        else { backToMenu = true; gs = GameSession{}; s_needReset = true; }
        if (a.clickSound.stream.buffer)PlaySound(a.clickSound); return;
    }

    Rectangle src = { 0,0,(float)a.bgMainMenu.width,(float)a.bgMainMenu.height };
    DrawTexturePro(a.bgMainMenu, src, { 0,0,(float)sw,(float)sh }, { 0,0 }, 0.f, WHITE);
    DrawRectangle(0, 0, sw, sh, NCA(4, 2, 0, 200));
    UpdateDrawParticles(dt);

    for (int i = 0; i < 30; i++) {
        float ang = i * 47.3f * (3.14159f / 180.f) + gs.winTimer * 0.5f;
        float d = 200.f + 90.f * sinf(gs.winTimer + i * 0.7f);
        float fx = sw / 2.f + cosf(ang) * d, fy = sh / 2.f + sinf(ang) * d * 0.55f;
        DrawCircle((int)fx, (int)fy, (int)(1.5f + sinf((float)i)), Fade(NC(255, 215, 55), 0.18f + 0.14f * sinf(gs.winTimer * 2.f + i)));
    }

    Color winnerAccent = (gs.winner == 1) ? NC(80, 200, 255) : NC(255, 160, 80);
    float pw = 640.f, ph = 420.f, px = sw / 2.f - pw / 2.f, py = sh / 2.f - ph / 2.f;
    float glow = 0.07f + 0.05f * sinf(gs.winTimer * 2.8f);
    for (int i = 6; i >= 1; i--)
        DrawRectangleRounded({ px - i * 4.f,py - i * 4.f,pw + i * 8.f,ph + i * 8.f }, 0.1f, 8, Fade(winnerAccent, glow * i));
    DrawRectangleRounded({ px,py,pw,ph }, 0.1f, 8, NCA(12, 6, 1, 252));
    DrawRectangleRounded({ px,py,pw,ph }, 0.1f, 8, NCA(winnerAccent.r / 6, winnerAccent.g / 6, winnerAccent.b / 6, 100));
    N_OrnateFrame(px, py, pw, ph, winnerAccent);
    DrawRectangleRounded({ px,py,pw,62 }, 0.1f, 8, NCA(winnerAccent.r / 4, winnerAccent.g / 4, winnerAccent.b / 4, 220));
    N_GlowLine(px + 18, py + 62, px + pw - 18, winnerAccent);
    N_Diamond(px + pw / 2, py + 62, 8, winnerAccent);

    // Crown
    float crX = sw / 2.f, crY = py + 28.f;
    DrawRectangle((int)(crX - 22), (int)(crY + 5), 44, 14, NC(255, 210, 30));
    DrawTriangle({ crX - 22,crY + 5 }, { crX - 22,crY - 11 }, { crX - 10,crY + 5 }, NC(255, 210, 30));
    DrawTriangle({ crX,crY - 17 }, { crX - 8,crY + 5 }, { crX + 8,crY + 5 }, NC(255, 225, 50));
    DrawTriangle({ crX + 22,crY + 5 }, { crX + 10,crY + 5 }, { crX + 22,crY - 11 }, NC(255, 210, 30));
    DrawCircle((int)(crX - 22), (int)(crY + 5), 4, NC(255, 80, 80));
    DrawCircle((int)crX, (int)(crY - 17), 4, NC(80, 200, 255));
    DrawCircle((int)(crX + 22), (int)(crY + 5), 4, NC(80, 255, 120));

    // Winner name
    const char* wname = (gs.winner == 1) ? gs.name1 : gs.name2;
    float nfs = 50.f * (1.f + 0.04f * sinf(gs.winTimer * 3.f));
    Vector2 nmsz = MeasureTextEx(a.gameFont, wname, nfs, 2);
    DrawTextEx(a.gameFont, wname, { sw / 2.f - nmsz.x / 2.f + 4,py + 72 + 4 }, nfs, 2, NCA(0, 0, 0, 112));
    DrawTextEx(a.gameFont, wname, { sw / 2.f - nmsz.x / 2.f + 2,py + 72 + 2 }, nfs, 2, NCA(winnerAccent.r / 2, winnerAccent.g / 2, winnerAccent.b / 2, 180));
    DrawTextEx(a.gameFont, wname, { sw / 2.f - nmsz.x / 2.f,py + 72 }, nfs, 2, winnerAccent);

    // WINS!
    float wfs = 34.f * (1.f + 0.06f * sinf(gs.winTimer * 4.f));
    Vector2 wfsz = MeasureTextEx(a.gameFont, "WINS!", wfs, 2);
    DrawTextEx(a.gameFont, "WINS!", { sw / 2.f - wfsz.x / 2.f + 2,py + 128 + 2 }, wfs, 2, NCA(0, 0, 0, 95));
    DrawTextEx(a.gameFont, "WINS!", { sw / 2.f - wfsz.x / 2.f,py + 128 }, wfs, 2, NC(252, 215, 45));

    // Congratulations cycling
    const char* msgs[] = { "Outstanding!","Congratulations!","Well played!" };
    int midx = (int)(gs.winTimer * 0.5f) % 3;
    Vector2 mgsz = MeasureTextEx(a.gameFont, msgs[midx], 20, 1);
    DrawTextEx(a.gameFont, msgs[midx], { sw / 2.f - mgsz.x / 2.f,py + 176.f }, 20, 1,
        Fade(NC(200, 220, 255), 0.5f + 0.35f * sinf(gs.winTimer * 2.5f)));

    N_GlowLine(px + 24, py + 206, px + pw - 24, Fade(winnerAccent, 0.4f));

    // Winner piece
    float pieceX = sw / 2.f - 60.f, pieceY = py + 220.f;
    bool isWinB = (gs.winner == 1);
    DrawCircle((int)pieceX + 3, (int)pieceY + 3, 22, NCA(0, 0, 0, 95));
    DrawCircle((int)pieceX, (int)pieceY, 22, isWinB ? NC(28, 28, 28) : NC(235, 235, 235));
    DrawCircle((int)pieceX - 6, (int)pieceY - 6, 7, isWinB ? NCA(88, 88, 88, 148) : NCA(255, 255, 255, 195));
    DrawCircleLines((int)pieceX, (int)pieceY, 22, isWinB ? NC(8, 8, 8) : NC(175, 175, 175));
    DrawTextEx(a.gameFont, isWinB ? "(Black Piece)" : "(White Piece)",
        { pieceX + 32.f,pieceY - 10.f }, 17, 1, Fade(winnerAccent, 0.72f));

    N_GlowLine(px + 24, py + 270, px + pw - 24, Fade(winnerAccent, 0.35f));



    const char* bLabels[] = { "PLAY AGAIN","MAIN MENU" };
    Color bAccents[] = { NC(80,200,120),NC(255,140,60) };
    float btnW = 200.f, btnH = 52.f, btnGap = 28.f;
    float btnSX = sw / 2.f - (btnW * 2 + btnGap) / 2.f, btnY = py + 288.f;
    for (int i = 0; i < 2; i++) {
        bool sel = (i == gs.winDisplaySel);
        float bx = btnSX + i * (btnW + btnGap);
        Color bc = bAccents[i];
        if (sel) for (int j = 3; j >= 1; j--)
            DrawRectangleRounded({ bx - j * 2.5f,btnY - j * 2.5f,btnW + j * 5.f,btnH + j * 5.f }, 0.2f, 8, Fade(bc, 0.07f * j));
        DrawRectangleRounded({ bx,btnY,btnW,btnH }, 0.2f, 8, sel ? NCA(bc.r / 3, bc.g / 3, bc.b / 3, 215) : NCA(18, 9, 1, 175));
        if (sel) {
            DrawRectangleRounded({ bx,btnY,6,btnH }, 0.8f, 4, bc);
            DrawRectangleRoundedLines({ bx - 2,btnY - 2,btnW + 4,btnH + 4 }, 0.2f, 8, Fade(bc, 0.5f));
        }
        DrawRectangleRoundedLines({ bx,btnY,btnW,btnH }, 0.2f, 8, sel ? bc : NCA(100, 58, 10, 155));
        Vector2 bsz = MeasureTextEx(a.gameFont, bLabels[i], 22, 1);
        DrawTextEx(a.gameFont, bLabels[i], { bx + btnW / 2.f - bsz.x / 2.f,btnY + btnH / 2.f - bsz.y / 2.f },
            22, 1, sel ? NC(255, 235, 80) : Fade(bc, 0.55f));
    }
    float ha = 0.35f + 0.2f * sinf(t * 2.f);
    DrawTextEx(a.gameFont, "A/D  Select      ENTER  Confirm",
        { sw / 2.f - MeasureTextEx(a.gameFont,"A/D  Select      ENTER  Confirm",16,1).x / 2.f,py + ph + 14.f },
        16, 1, Fade(NCA(205, 162, 75, 255), ha));
}






static void DrawPlaying(int sw, int sh, const GameAssets& a, bool& backToMenu) {
    float t = (float)GetTime(), dt = GetFrameTime();

    // PAUSE
    if (gs.paused) {
        DrawRectangle(0, 0, sw, sh, NCA(0, 0, 0, 152));
        float pw2 = 370.f, ph2 = 255.f, px2 = sw / 2.f - pw2 / 2.f, py2 = sh / 2.f - ph2 / 2.f;
        float g2 = 0.038f + 0.022f * sinf(t * 2.f);
        for (int i = 4; i >= 1; i--)
            DrawRectangleRounded({ px2 - i * 3.f,py2 - i * 3.f,pw2 + i * 6.f,ph2 + i * 6.f }, 0.1f, 8, Fade(NC(200, 130, 20), g2 * i));
        DrawRectangleRounded({ px2,py2,pw2,ph2 }, 0.1f, 8, NCA(18, 9, 2, 248));
        N_OrnateFrame(px2, py2, pw2, ph2, NC(200, 130, 20));
        DrawRectangleRounded({ px2,py2,pw2,55 }, 0.1f, 8, NCA(80, 40, 8, 225));
        N_GlowLine(px2 + 16, py2 + 55, px2 + pw2 - 16, NC(195, 125, 18));
        Vector2 psz = MeasureTextEx(a.gameFont, "PAUSE", 38, 3);
        DrawTextEx(a.gameFont, "PAUSE", { sw / 2.f - psz.x / 2.f + 3,py2 + 10 + 3 }, 38, 3, NCA(0, 0, 0, 98));
        DrawTextEx(a.gameFont, "PAUSE", { sw / 2.f - psz.x / 2.f,py2 + 10 }, 38, 3, NC(252, 208, 48));
        const char* opts[] = { "RESUME","QUIT TO MENU" };
        for (int i = 0; i < 2; i++) {
            bool sel = (i == gs.pauseSel);
            float oy = py2 + 66.f + i * 75.f;
            Rectangle r = { px2 + 26.f,oy,pw2 - 52.f,56.f };
            DrawRectangleRounded(r, 0.15f, 6, sel ? NCA(88, 46, 7, 205) : NCA(26, 13, 2, 172));
            if (sel) {
                DrawRectangleRounded({ r.x,r.y,5,r.height }, 0.8f, 4, NC(252, 155, 0));
                DrawRectangleRoundedLines({ r.x - 2,r.y - 2,r.width + 4,r.height + 4 }, 0.15f, 6, NCA(252, 175, 0, 50));
            }
            DrawRectangleRoundedLines(r, 0.15f, 6, sel ? NC(252, 165, 18) : NCA(102, 58, 12, 160));
            Vector2 osz = MeasureTextEx(a.gameFont, opts[i], 22, 1);
            DrawTextEx(a.gameFont, opts[i], { r.x + r.width / 2.f - osz.x / 2.f,r.y + r.height / 2.f - osz.y / 2.f },
                22, 1, sel ? NC(252, 218, 68) : NC(175, 125, 44));
        }
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_S)) gs.pauseSel ^= 1;
        if (IsKeyPressed(KEY_ENTER)) {
            if (gs.pauseSel == 0) gs.paused = false;
            else { backToMenu = true; gs = GameSession{}; s_needReset = true; }
            if (a.clickSound.stream.buffer)PlaySound(a.clickSound);
        }
        if (IsKeyPressed(KEY_ESCAPE)) gs.paused = false;
        return;
    }

    //  INPUT
    gs.turnTimer -= dt;
    if (gs.turnTimer <= 0.f) { gs.curP = (gs.curP == 1) ? 2 : 1; gs.turnTimer = TURN_TIME; }
    if (IsKeyPressed(KEY_W) && gs.curY > 0) { gs.curY--; if (a.moveSound.stream.buffer)PlaySound(a.moveSound); }
    if (IsKeyPressed(KEY_S) && gs.curY < BOARD_SIZE - 1) { gs.curY++; if (a.moveSound.stream.buffer)PlaySound(a.moveSound); }
    if (IsKeyPressed(KEY_A) && gs.curX > 0) { gs.curX--; if (a.moveSound.stream.buffer)PlaySound(a.moveSound); }
    if (IsKeyPressed(KEY_D) && gs.curX < BOARD_SIZE - 1) { gs.curX++; if (a.moveSound.stream.buffer)PlaySound(a.moveSound); }
    if (IsKeyPressed(KEY_ENTER)) {
        if (gs.board[gs.curX][gs.curY] == 0) {
            gs.board[gs.curX][gs.curY] = gs.curP;
            if (a.moveSound.stream.buffer)PlaySound(a.moveSound);
            WinLine wl;
            if (CheckWin5(gs.board, gs.curP, gs.curX, gs.curY, wl)) {
                gs.winner = gs.curP; gs.winLine = wl;
                gs.blinkTimer = 0.f; gs.sub = SUB_WIN_BLINK;
                if (a.winSound.stream.buffer)PlaySound(a.winSound);
            }
            else { gs.curP = (gs.curP == 1) ? 2 : 1; gs.turnTimer = TURN_TIME; }
        }
    }
    if (IsKeyPressed(KEY_ESCAPE)) { gs.paused = true; gs.pauseSel = 0; }

    //  DRAW
    Rectangle src = { 0,0,(float)a.bgMainMenu.width,(float)a.bgMainMenu.height };
    DrawTexturePro(a.bgMainMenu, src, { 0,0,(float)sw,(float)sh }, { 0,0 }, 0.f, WHITE);
    DrawRectangle(0, 0, sw, sh, NCA(8, 4, 0, 128));

    float bpX = sw / 2.f - (BOARD_SIZE * CELL) / 2.f - 55.f;
    float bpY = sh / 2.f - (BOARD_SIZE * CELL) / 2.f;
    float bpW = (float)BOARD_SIZE * CELL + 58.f, bpH = (float)BOARD_SIZE * CELL + 58.f;
    DrawRectangleRounded({ bpX,bpY,bpW,bpH }, 0.04f, 6, NCA(196, 155, 86, 240));
    DrawRectangleRoundedLines({ bpX,bpY,bpW,bpH }, 0.04f, 6, NC(135, 85, 26));
    float oX = bpX + 29.f, oY = bpY + 29.f;
    DrawBoard(oX, oY);

    // Cursor + ghost
    float cx2 = oX + gs.curX * CELL, cy2 = oY + gs.curY * CELL;
    float blink = 0.56f + 0.38f * sinf(t * 5.f);
    Color cc = (gs.curP == 1) ? NC(80, 180, 255) : NC(255, 130, 60);
    DrawRectangleLinesEx({ cx2 - 15,cy2 - 15,30,30 }, 2.2f, Fade(cc, blink));
    DrawCircleLines((int)cx2, (int)cy2, 14, Fade(cc, blink * 0.52f));
    bool isB2 = (gs.curP == 1);
    DrawCircle((int)cx2, (int)cy2, 13, isB2 ? NCA(26, 26, 26, 50) : NCA(236, 236, 236, 50));

    //  INFO PANEL 
    float ipX = bpX + bpW + 12.f, ipY = bpY, ipW = (float)sw - ipX - 14.f, ipH = bpH;
    DrawRectangleRounded({ ipX,ipY,ipW,ipH }, 0.08f, 8, NCA(14, 7, 1, 230));
    N_OrnateFrame(ipX, ipY, ipW, ipH, NC(172, 105, 14));

    const char* mL[] = { "STANDARD","BLOCK BOTH","FREE BLOCK" };
    const char* oL[] = { "VS CPU(E)","VS CPU(H)","PVP" };
    char mstr[48]; sprintf_s(mstr, sizeof(mstr), "%s | %s", mL[(int)gs.mode], oL[(int)gs.opp]);
    Vector2 msz2 = MeasureTextEx(a.gameFont, mstr, 13, 1);
    DrawRectangleRounded({ ipX + 8,ipY + 10,ipW - 16,25 }, 0.4f, 6, NCA(55, 26, 4, 172));
    DrawTextEx(a.gameFont, mstr, { ipX + ipW / 2.f - msz2.x / 2.f,ipY + 14 }, 13, 1, NCA(192, 150, 46, 185));

    for (int p = 0; p < 2; p++) {
        bool active = (gs.curP == p + 1);
        float py3 = ipY + 44.f + p * 108.f;
        Color pc = (p == 0) ? NC(80, 200, 255) : NC(255, 160, 80);
        DrawRectangleRounded({ ipX + 8,py3,ipW - 16,98 }, 0.12f, 6,
            active ? NCA(pc.r / 4, pc.g / 4, pc.b / 4, 195) : NCA(16, 8, 1, 172));
        if (active) {
            DrawRectangleRounded({ ipX + 8,py3,5,98 }, 0.8f, 4, pc);
            DrawRectangleRoundedLines({ ipX + 6,py3 - 2,ipW - 12,102 }, 0.12f, 6, Fade(pc, 0.45f));
        }
        DrawRectangleRoundedLines({ ipX + 8,py3,ipW - 16,98 }, 0.12f, 6, active ? pc : NCA(95, 55, 10, 152));
        DrawCircle((int)(ipX + 30), (int)(py3 + 49), 14, p == 0 ? NC(26, 26, 26) : NC(225, 225, 225));
        DrawCircle((int)(ipX + 30 - 3), (int)(py3 + 49 - 3), 4, p == 0 ? NCA(86, 86, 86, 145) : NCA(255, 255, 255, 192));
        DrawCircleLines((int)(ipX + 30), (int)(py3 + 49), 14, p == 0 ? NC(6, 6, 6) : NC(165, 165, 165));
        const char* pn = (p == 0) ? gs.name1 : gs.name2;
        DrawTextEx(a.gameFont, pn, { ipX + 50.f,py3 + 11 }, 20, 1, active ? pc : Fade(pc, 0.46f));
        if (active) {
            float ba = 0.58f + 0.36f * sinf(t * 3.2f);
            DrawRectangleRounded({ ipX + 50.f,py3 + 38,70,22 }, 0.4f, 6, Fade(pc, 0.20f));
            DrawTextEx(a.gameFont, "YOUR TURN", { ipX + 56.f,py3 + 41 }, 14, 1, Fade(pc, ba));
            float tr = gs.turnTimer / TURN_TIME;
            Color tc = (gs.turnTimer > 10.f) ? NC(80, 220, 100) : (gs.turnTimer > 5.f) ? NC(255, 200, 40) : NC(255, 80, 60);
            float tbX = ipX + 8.f, tbY = py3 + 70.f, tbW = ipW - 16.f, tbH = 13.f;
            DrawRectangleRounded({ tbX,tbY,tbW,tbH }, 1.f, 4, NCA(28, 14, 2, 175));
            DrawRectangleRounded({ tbX,tbY,tbW * tr,tbH }, 1.f, 4, tc);
            DrawRectangleRoundedLines({ tbX,tbY,tbW,tbH }, 1.f, 4, NCA(75, 46, 8, 135));
            char timerTxt[8]; sprintf_s(timerTxt, sizeof(timerTxt), "%.0fs", gs.turnTimer);
            DrawTextEx(a.gameFont, timerTxt, { tbX + tbW - 34.f,py3 + 84.f }, 13, 1, tc);
        }
        else {
            DrawTextEx(a.gameFont, "waiting...", { ipX + 50.f,py3 + 38 }, 14, 1, NCA(142, 102, 36, 108));
        }
    }

    N_GlowLine(ipX + 12, ipY + 272.f, ipX + ipW - 12, NCA(152, 95, 14, 112));
    const char* ctrls[] = { "WASD  Move cursor","ENTER  Place piece","ESC    Pause" };
    for (int i = 0; i < 3; i++)
        DrawTextEx(a.gameFont, ctrls[i], { ipX + 15.f,ipY + 282.f + i * 25.f }, 14, 1, NCA(175, 135, 55, 150));

    int c1 = 0, c2 = 0;
    for (int x = 0; x < BOARD_SIZE; x++) for (int y = 0; y < BOARD_SIZE; y++) { if (gs.board[x][y] == 1)c1++; else if (gs.board[x][y] == 2)c2++; }
    N_GlowLine(ipX + 12, ipY + 366.f, ipX + ipW - 12, NCA(152, 95, 14, 112));
    char cs2[48]; sprintf_s(cs2, sizeof(cs2), "%s:%d  %s:%d", gs.name1, c1, gs.name2, c2);
    Vector2 css = MeasureTextEx(a.gameFont, cs2, 13, 1);
    DrawTextEx(a.gameFont, cs2, { ipX + ipW / 2.f - css.x / 2.f,ipY + 372.f }, 13, 1, NCA(195, 155, 55, 160));

    DrawRectangleRounded({ ipX + 8,ipY + ipH - 44,ipW - 16,34 }, 0.3f, 6, NCA(46, 22, 3, 165));
    DrawRectangleRoundedLines({ ipX + 8,ipY + ipH - 44,ipW - 16,34 }, 0.3f, 6, NCA(152, 95, 14, 140));
    Vector2 phsz = MeasureTextEx(a.gameFont, "ESC  Pause", 16, 1);
    DrawTextEx(a.gameFont, "ESC  Pause", { ipX + ipW / 2.f - phsz.x / 2.f,ipY + ipH - 35 }, 16, 1, NCA(195, 152, 46, 172));
}

// PUBLIC ENTRY
void DrawNewGameFlow(int screenWidth, int screenHeight,
    const GameAssets& assets, bool& backToMenu)
{
    // Reset session whenever we arrive fresh from main menu
    if (s_needReset) {
        gs = GameSession{};
        s_needReset = false;
        s_entryFrame = true;  // block ENTER that triggered NEW GAME
    }

    switch (gs.sub) {
    case SUB_MODE_SELECT:     DrawModeSelect(screenWidth, screenHeight, assets, backToMenu); break;
    case SUB_OPPONENT_SELECT: DrawOpponentSelect(screenWidth, screenHeight, assets); break;
    case SUB_NAME_INPUT:      DrawNameInput(screenWidth, screenHeight, assets); break;
    case SUB_READY:           DrawReadyScreen(screenWidth, screenHeight, assets); break;
    case SUB_PLAYING:         DrawPlaying(screenWidth, screenHeight, assets, backToMenu); break;
    case SUB_WIN_BLINK:       DrawWinBlink(screenWidth, screenHeight, assets); break;
    case SUB_WIN:             DrawWinScreen(screenWidth, screenHeight, assets, backToMenu); break;
    }

    s_entryFrame = false;  // clear after each frame

    if (backToMenu) { gs = GameSession{}; s_needReset = true; }
}

// Call this from main.cpp when entering STATE_PLAYING from menu
void ResetNewGame() { s_needReset = true; }