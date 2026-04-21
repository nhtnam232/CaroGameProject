#include"UI.h"

enum NewGameStep {
    STEP_CHOOSE_MODE,
    STEP_CHOOSE_OPPONENT,
    STEP_CHOOSE_BOT_DIFFICULTY,
    STEP_ENTER_NAME,
    STEP_READY, 
    STEP_VICTORY
};


void DrawFlipCard(float x, float y, float width, float height, const char* title, const char* description, Color accentColor, const GameAssets& assets, bool isSelected, bool isFlipped, float scaleX)
{
    // Counting width and x-axis based on scaleX
    float currentWidth = width * scaleX;
    float offsetX = x + (width - currentWidth) / 2.0f;

    // Setup retro color
    Color shadow = { (unsigned char)(accentColor.r * 0.4f), (unsigned char)(accentColor.g * 0.4f), (unsigned char)(accentColor.b * 0.4f), 255 };
    Color bgFill = { (unsigned char)(accentColor.r * 0.15f), (unsigned char)(accentColor.g * 0.15f), (unsigned char)(accentColor.b * 0.15f), 255 };

    // Draw the card body and shadow, auto resize according to currentWidth
    DrawRectangle((int)offsetX + 6, (int)y + 6, (int)currentWidth, (int)height, shadow); // Shadow
    DrawRectangle((int)offsetX, (int)y, (int)currentWidth, (int)height, bgFill); // Fill
    DrawRectangleLinesEx({ offsetX, y, currentWidth, height }, 4.0f, accentColor); // Border

    // Draw the content, only draw if the card is wide enough to see the text
    if (scaleX > 0.15f) {
        if (!isFlipped) {
            // Front
            float fontSize = 40.0f * scaleX;
            Vector2 titleSize = MeasureTextEx(assets.gameFont, title, fontSize, 2);
            DrawTextEx(assets.gameFont, title, { offsetX + currentWidth / 2.0f - titleSize.x / 2.0f, y + height / 2.0f - titleSize.y / 2.0f }, fontSize, 2, accentColor);
        }
        else {
            // Back
            float descFS = 25.0f * scaleX;
            Vector2 descSize = MeasureTextEx(assets.gameFont, description, descFS, 1);
            DrawTextEx(assets.gameFont, description, { offsetX + currentWidth / 2.0f - descSize.x / 2.0f, y + height / 2.0f - descSize.y / 2.0f }, descFS, 1, WHITE);
        }
    }

    // Draw 4 squares for decoration, also scale according to scale X
    float cornerSize = 10.0f * scaleX;
    DrawRectangle((int)offsetX, (int)y, (int)cornerSize, (int)cornerSize, accentColor); //top-left
    DrawRectangle((int)(offsetX + currentWidth - cornerSize), (int)y, (int)cornerSize, (int)cornerSize, accentColor); //top-right
    DrawRectangle((int)offsetX, (int)(y + height - cornerSize), (int)cornerSize, (int)cornerSize, accentColor); // bottom-left
    DrawRectangle((int)(offsetX + currentWidth - cornerSize), (int)(y + height - cornerSize), (int)cornerSize, (int)cornerSize, accentColor); // bottom-right

    // Highlight when selected, also scale according to scale X
    if (isSelected) {
        DrawRectangle((int)offsetX - 8, (int)y - 8, (int)currentWidth + 16, (int)height + 16, Fade(accentColor, 0.2f));
        DrawRectangleLinesEx({ offsetX - 4, y - 4, currentWidth + 8, height + 8 }, 3.0f, WHITE);
    }
}


static int currentStep = 0;

void DrawStepMode(int screenWidth, int screenHeight, const GameAssets& assets, int& selectedSlot, bool& backToMenu)
{
    // Init management variables
    static bool isFlipped = false;
    static float flipRotation = 0.0f;
    static bool isAnimating = false;

    // Draw base menu
    Rectangle panel = DrawBaseMenu(screenWidth, screenHeight, assets, "Select Game Mode");

    // Moving
    if (IsKeyReleased(KEY_D) || IsKeyReleased(KEY_RIGHT) || IsKeyReleased(KEY_A) || IsKeyReleased(KEY_LEFT)) {
        if (IsKeyReleased(KEY_D) || IsKeyReleased(KEY_RIGHT)) selectedSlot = (selectedSlot + 1) % 3;
        else selectedSlot = (selectedSlot - 1 + 3) % 3;

        // Reset when selected new card
        isFlipped = false;
        flipRotation = 0.0f;
        isAnimating = false;
        if (assets.clickSound.stream.buffer) PlaySound(assets.clickSound);
    }

    // Starting flip card
    if (IsKeyReleased(KEY_SPACE) && !isAnimating) {
        isAnimating = true;
    }

    if (isAnimating) {
        float speed = 500.0f;
        float prevRotation = flipRotation;
        flipRotation += speed * GetFrameTime();

        //The moment the card is 90 degrees -> Change the content side.
        if (prevRotation < 90.0f && flipRotation >= 90.0f) {
            isFlipped = !isFlipped;
        }

        // Stop animation
        if (flipRotation >= 180.0f) {
            flipRotation = 0.0f;
            isAnimating = false;
        }
    }

    // Couting scaleX from the flip angle
    float scaleX = fabsf(cosf(flipRotation * DEG2RAD));

    // Counting position
    float cardWidth = 210.0f;
    float cardHeight = 300.0f;
    float gap = 30.0f;
    float totalCardsWidth = (cardWidth * 3) + (gap * 2);
    float startX = panel.x + (panel.width - totalCardsWidth) / 2.0f;
    float startY = panel.y + (panel.height - cardHeight) / 2.0f + 30.0f;

    // Set up data of 3 cards
    const char* titles[3] = { "STANDARD", "BLOCK 2 ENDS", "RENJU MODE" };
    const char* descriptions[3] = {
        // Card 1: Standard
        "Get 5 in a row\nto win.\n6+ pieces\nalso win.",

        // Card 2: Block 2 Ends
        "5-in-a-row blocked\nat BOTH ends\nis NOT a win.\n(Need 1 open end)",

        // Card 3: Renju
        "Player (X) restricted:\nNO double-3s,\nNO double-4s,\nNO 6+ pieces."
    };
    Color accents[3] = { ColorBlueNeon, ColorLightYellow ,ColorPinkNeon };

    // Draw 3 cards
    for (int i = 0; i < 3; i++) {
        //Counting position for each card
        float currentX = startX + i * (cardWidth + gap); 

        // Only selected card can flip
        float s = (i == selectedSlot) ? scaleX : 1.0f;
        bool f = (i == selectedSlot) ? isFlipped : false;

        DrawFlipCard(currentX, startY, cardWidth, cardHeight, titles[i], descriptions[i], accents[i], assets, (i == selectedSlot), f, s);
    }
}
//Draw page choose opponent
void DrawStepOpponent(int screenWidth, int screenHeight, const GameAssets& assets, int& selectedSlot, bool& backToMenu)
{
    // Draw Base menu
    DrawBaseMenu(screenWidth, screenHeight, assets, "Select Opponent");

    // Moving
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
        selectedSlot = (selectedSlot == 0) ? 1 : 0;
        if (assets.clickSound.stream.buffer) PlaySound(assets.clickSound);
    }

    // Couting position for cards
    float cardWidth = 210.0f;
    float cardHeight = 320.0f;
    float gap = 150.0f;
    float totalWidth = (cardWidth * 2) + gap;
    float startX = screenWidth / 2.0f - totalWidth / 2.0f;
    float startY = screenHeight / 2.0f - cardHeight / 2.0f + 20.0f;

    const char* titles[2] = { "1 PLAYER", "2 PLAYERS" };
    const char* descriptions[2] = { "Play against\nthe CPU.", "Local PvP\nwith a friend." };
    Color accents[2] = { ColorOrangeSolid, ColorOrangeSolid };

    // Draw 2 cards, use again DrawFlipCard() and in this function, do not allow cards can flip
    for (int i = 0; i < 2; i++) {
        float x = startX + i * (cardWidth + gap);
        bool isSelected = (i == selectedSlot);
        DrawFlipCard(x, startY, cardWidth, cardHeight, titles[i], descriptions[i], accents[i], assets, isSelected, false, 1.0f);
    }
}
//Choose difficulty of bot
void DrawStepBotDifficulty(int screenWidth, int screenHeight, const GameAssets& assets, int& selectedSlot, bool& backToMenu)
{
    DrawBaseMenu(screenWidth, screenHeight, assets, "Bot Difficulty");

    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
        selectedSlot = (selectedSlot + 1) % 3;
        if (assets.clickSound.stream.buffer) PlaySound(assets.clickSound);
    }
    else if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
        selectedSlot = (selectedSlot - 1 + 3) % 3;
        if (assets.clickSound.stream.buffer) PlaySound(assets.clickSound);
    }

    float cardWidth = 210.0f;
    float cardHeight = 300.0f;
    float gap = 30.0f;
    float totalWidth = (cardWidth * 3) + (gap * 2);
    float startX = screenWidth / 2.0f - totalWidth / 2.0f;
    float startY = screenHeight / 2.0f - cardHeight / 2.0f + 20.0f;

    const char* titles[3] = { "EASY", "MEDIUM", "HARD" };
    const char* descriptions[3] = { "Bot moves\nrandomly.", "Standard\nAI skill.", "Very tough\nopponent." };

    Color accents[3] = { ColorLightGreen, ColorLightYellow, ColorRedNeon };

    for (int i = 0; i < 3; i++) {
        float x = startX + i * (cardWidth + gap);
        bool isSelected = (i == selectedSlot);

        DrawFlipCard(x, startY, cardWidth, cardHeight, titles[i], descriptions[i], accents[i], assets, isSelected, false, 1.0f);
    }

}
//Util Function for input player name
void DrawInputBox(float x, float y, float w, float h, const char* text, bool isActive) {
    Rectangle rect = { x, y, w, h };
    DrawRectangleRec(rect, Fade(BLACK, 0.6f));
    DrawRectangleLinesEx(rect, isActive ? 3.0f : 1.0f, isActive ? WHITE : GRAY);

    DrawText(text, (int)x + 15, (int)y + 15, 30, isActive ? WHITE : GRAY);

    if (isActive && (((int)(GetTime() * 2)) % 2 == 0)) {
        int textW = MeasureText(text, 30);
        DrawRectangle((int)x + 18 + textW, (int)y + 10, 15, 30, WHITE);
    }
}
void DrawStepEnterName(int screenWidth, int screenHeight, const GameAssets& assets, int opponentMode, char* p1Name, int& count1, char* p2Name, int& count2, int& activeField)
{

    //Draw Base Menu with smaller size
    const char* title = "ENTER NAME";

    Rectangle srcBg = { 0, 0, (float)assets.bgMainMenu.width, (float)assets.bgMainMenu.height };
    Rectangle dstBg = { 0, 0, (float)screenWidth, (float)screenHeight };
    DrawTexturePro(assets.bgMainMenu, srcBg, dstBg, { 0, 0 }, 0.0f, WHITE);
    DrawRectangle(0, 0, screenWidth, screenHeight, ColorShadow);

    DrawRectangleLinesEx({ 6, 6, (float)screenWidth - 12, (float)screenHeight - 12 }, 2.0f, ColorBronzeAlpha);
    DrawRectangleLinesEx({ 12, 12, (float)screenWidth - 24, (float)screenHeight - 24 }, 1.0f, ColorBronzeAlpha);
    DrawCornerOrnament(18, 18, 1, 1, ColorBronzeAlpha);
    DrawCornerOrnament(screenWidth - 18, 18, -1, 1, ColorBronzeAlpha);
    DrawCornerOrnament(18, screenHeight - 18, 1, -1, ColorBronzeAlpha);
    DrawCornerOrnament(screenWidth - 18, screenHeight - 18, -1, -1, ColorBronzeAlpha);

    float pw = 560.0f, ph = 380.0f;
    float px = screenWidth / 2.0f - pw / 2.0f;
    float py = screenHeight / 2.0f - ph / 2.0f;

    float bannerX1 = px - 55.0f;
    float bannerX2 = px + pw + 55.0f;
    float bTop = py + 12.0f, bBot = py + ph - 12.0f;
    DrawSideBanner(bannerX1, bTop, bBot, assets);
    DrawSideBanner(bannerX2, bTop, bBot, assets);

    DrawRectangleRounded({ px, py, pw, ph }, 0.07f, 8, ColorBrownDark);

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

    float hh = 68.0f;
    DrawRectangleRounded({ px, py, pw, hh }, 0.07f, 8, ColorBrownRust);
    DrawGlowLine(px + 22, py + hh, px + pw - 22, ColorBronzeSolid);
    DrawDiamond(px + pw / 2, py + hh, 8, ColorGoldSolid);
    DrawDiamond(px + pw / 2 - 110, py + hh, 4, ColorGoldAlpha);
    DrawDiamond(px + pw / 2 + 110, py + hh, 4, ColorGoldAlpha);

    DrawOrnateFrame(px, py, pw, ph, ColorBronzeSolid, ColorGoldSolid);

    Vector2 tsz = MeasureTextEx(assets.gameFont, title, 56, 4);
    float ttx = px + pw / 2 - tsz.x / 2;
    float tty = py + hh / 2 - tsz.y / 2;

    DrawTextEx(assets.gameFont, "<<", { ttx - 56, tty + 10 }, 30, 1, ColorGoldAlpha);
    DrawTextEx(assets.gameFont, ">>", { ttx + tsz.x + 16, tty + 10 }, 30, 1, ColorGoldAlpha);
    DrawTextEx(assets.gameFont, title, { ttx + 4, tty + 4 }, 56, 4, ColorShadow);
    DrawTextEx(assets.gameFont, title, { ttx + 2, tty + 2 }, 56, 4, ColorBronzeAlpha);
    DrawTextEx(assets.gameFont, title, { ttx, tty }, 56, 4, ColorGoldSolid);

    // Begin handle user input
    // Auto-fill BOT name if mode is BOT
    if (opponentMode == 0) {
        if (count2 == 0) {
            strcpy_s(p2Name, 16, "BOT");
            count2 = 3;
            activeField = 0;
        }
        if (activeField == 1) activeField = 0;
    }
    else {
        if (strcmp(p2Name, "BOT") == 0) {
            memset(p2Name, 0, 16);
            count2 = 0;
        }
    }
    char* currentName = (activeField == 0) ? p1Name : p2Name;
    int* currentCount = (activeField == 0) ? &count1 : &count2;

    int key = GetCharPressed();
    while (key > 0) {
        bool isLetter = (key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z');
        bool isNumber = (key >= '0' && key <= '9');
        bool isSpace = (key == 32);

        if ((isLetter || isNumber || isSpace) && (*currentCount < 15)) {
            currentName[*currentCount] = (char)key;
            currentName[*currentCount + 1] = '\0';
            (*currentCount)++;
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && (*currentCount) > 0) {
        (*currentCount)--;
        currentName[*currentCount] = '\0';
    }

    if (IsKeyPressed(KEY_TAB)) {
        if (opponentMode != 0) {
            activeField = (activeField + 1) % 2;
        }
    }

    // Draw input box
    float boxW = 380.0f;
    float boxH = 50.0f;
    float boxX = px + (pw / 2.0f) - (boxW / 2.0f);
    float startY = py + hh + 30.0f;

    DrawTextEx(assets.gameFont, "PLAYER 1", { boxX, startY }, 20, 1, (activeField == 0) ? ColorCyanSolid : GRAY);
    DrawInputBox(boxX, startY + 25, boxW, boxH, p1Name, (activeField == 0));

    float box2Y = startY + 105.0f;

    const char* p2Title = (opponentMode == 0) ? "BOT NAME" : "PLAYER 2";
    DrawTextEx(assets.gameFont, p2Title, { boxX, box2Y }, 20, 1, (activeField == 1) ? ColorPinkNeon : GRAY);

    DrawInputBox(boxX, box2Y + 25, boxW, boxH, p2Name, (activeField == 1));
    if (opponentMode != 0) {
        DrawTextEx(assets.gameFont, "[TAB] Switch Player", { boxX, box2Y + 90 }, 16, 1, ColorGoldAlpha);
    }
    DrawTextEx(assets.gameFont, "Maximum number of characters: 15", { boxX, box2Y + 110 }, 16, 1, ColorGoldAlpha);
}
void DrawCaroBoard(const MatchManager& match, float boardX, float boardY)
{
    const int (*board)[15] = match.board;
    // Calculate the total grid size (15 cells * cell size)
    float gridWidth = (float)BOARD_SIZE * CELL;
    float gridHeight = (float)BOARD_SIZE * CELL;

    // Wooden board container (padding adjusted)
    float bpW = gridWidth + 58.0f;
    float bpH = gridHeight + 58.0f;

    DrawRectangleRounded({ boardX, boardY, bpW, bpH }, 0.04f, 6, Color{ 196, 155, 86, 240 });
    DrawRectangleRoundedLines({ boardX, boardY, bpW, bpH }, 0.04f, 6, Color{ 135, 85, 26, 255 });

    // Inner grid origin
    float oX = boardX + 29.0f;
    float oY = boardY + 29.0f;

    // Draw grid lines (Note: i <= BOARD_SIZE to draw the closing lines)
    for (int i = 0; i <= BOARD_SIZE; i++) {
        // Vertical lines
        DrawLineEx({ oX + i * CELL, oY }, { oX + i * CELL, oY + gridHeight }, 1.2f, Color{ 96, 60, 16, 195 });
        // Horizontal lines
        DrawLineEx({ oX, oY + i * CELL }, { oX + gridWidth, oY + i * CELL }, 1.2f, Color{ 96, 60, 16, 195 });
    }

    // Draw placed pieces in the CENTER of each cell
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (board[x][y] == 0) continue;

            float ppx = oX + (float)x * CELL + (CELL / 2.0f);
            float ppy = oY + (float)y * CELL + (CELL / 2.0f);
            bool isBlack = (match.board[x][y] == 1);

            // Flashing logic for winning pieces
            float alpha = 1.0f;
            if (match.isMatchOver) {
                for (int k = 0; k < 5; k++) {
                    if ((int)match.flashingPieces[k].x == x && (int)match.flashingPieces[k].y == y) {
                        alpha = 0.5f + 0.5f * sinf(match.winTimer * 12.0f);
                        break;
                    }
                }
            }

            // Piece shadow 
            DrawCircle((int)ppx + 2, (int)ppy + 2, 13, Fade(Color{ 0, 0, 0, 75 }, alpha));

            // Piece body 
            Color bodyColor = isBlack ? Color{ 26, 26, 26, 255 } : Color{ 236, 236, 236, 255 };
            DrawCircle((int)ppx, (int)ppy, 13, Fade(bodyColor, alpha));

            // Highlight shine
            Color shineColor = isBlack ? Color{ 86, 86, 86, 148 } : Color{ 255, 255, 255, 195 };
            DrawCircle((int)ppx - 4, (int)ppy - 4, 4, Fade(shineColor, alpha));

            // Outline 
            Color outlineColor = isBlack ? Color{ 6, 6, 6, 255 } : Color{ 175, 175, 175, 255 };
            DrawCircleLines((int)ppx, (int)ppy, 13, Fade(outlineColor, alpha));
        }
    }
}
void DrawPlayerInfoPanel(float ipX, float ipY, float ipW, float ipH, const MatchManager& match, const GameAssets& assets, const std::vector<PlayerInfo>& players) {
    float t = (float)GetTime();

    // Draw main panel background
    DrawRectangleRounded({ ipX, ipY, ipW, ipH }, 0.08f, 8, Color{ 14, 7, 1, 230 });
    DrawRectangleRoundedLines({ ipX, ipY, ipW, ipH }, 0.08f, 8, Color{ 172, 105, 14, 255 });

    // Draw Mode Info Header
    const char* modeLabels[] = { "STANDARD", "BLOCK 2 ENDS", "RENJU MODE" };
    char headerText[64];
    snprintf(headerText, sizeof(headerText), "%s | MATCH", modeLabels[match.gameMode]);

    Vector2 headerSize = MeasureTextEx(assets.gameFont, headerText, 13, 1);
    DrawRectangleRounded({ ipX + 8, ipY + 10, ipW - 16, 25 }, 0.4f, 6, Color{ 55, 26, 4, 172 });
    DrawTextEx(assets.gameFont, headerText, { ipX + ipW / 2.0f - headerSize.x / 2.0f, ipY + 14 }, 13, 1, Color{ 192, 150, 46, 185 });

    // Draw Player 1 & Player 2 Cards
    for (int p = 0; p < 2; p++) {
        bool isActive = (match.currentPlayer == p + 1);
        float cardY = ipY + 44.0f + p * 108.0f;

        // Get data from struct PlayerInfo
        const PlayerInfo& currentInfo = players[p];

        Color playerColor = (p == 0) ? Color{ 80, 200, 255, 255 } : Color{ 255, 160, 80, 255 };

        // Card background (brighter if active)
        Color bgColor = isActive ? Color{ (unsigned char)(playerColor.r / 4), (unsigned char)(playerColor.g / 4), (unsigned char)(playerColor.b / 4), 195 }
        : Color{ 16, 8, 1, 172 };
        DrawRectangleRounded({ ipX + 8, cardY, ipW - 16, 98 }, 0.12f, 6, bgColor);

        // Highlight border and left accent line if active
        if (isActive) {
            DrawRectangleRounded({ ipX + 8, cardY, 5, 98 }, 0.8f, 4, playerColor);
            DrawRectangleRoundedLines({ ipX + 6, cardY - 2, ipW - 12, 102 }, 0.12f, 6, Fade(playerColor, 0.45f));
        }
        DrawRectangleRoundedLines({ ipX + 8, cardY, ipW - 16, 98 }, 0.12f, 6, isActive ? playerColor : Color{ 95, 55, 10, 152 });

        // Draw Player Piece Icon (Black for P1, White for P2)
        DrawCircle((int)(ipX + 30), (int)(cardY + 49), 14, p == 0 ? Color{ 26, 26, 26, 255 } : Color{ 225, 225, 225, 255 });
        DrawCircle((int)(ipX + 27), (int)(cardY + 46), 4, p == 0 ? Color{ 86, 86, 86, 145 } : Color{ 255, 255, 255, 192 });
        DrawCircleLines((int)(ipX + 30), (int)(cardY + 49), 14, p == 0 ? Color{ 6, 6, 6, 255 } : Color{ 165, 165, 165, 255 });

        // Handle user name 
        std::string displayName = currentInfo.name;
        // Default name
        if (displayName.empty()) {
            displayName = (p == 0) ? "PLAYER 1" : (currentInfo.isBot ? "BOT" : "PLAYER 2");
        }
        // If playing with bot, player 2's name is BOT
        if (currentInfo.isBot && displayName != "BOT") {
            displayName += " [CPU]";
        }

        DrawTextEx(assets.gameFont, displayName.c_str(), { ipX + 50.0f, cardY + 11 }, 20, 1, isActive ? playerColor : Fade(playerColor, 0.46f));

        // Active Turn Indicator
        if (isActive) {
            float blinkAlpha = 0.58f + 0.36f * sinf(t * 3.2f);
            DrawRectangleRounded({ ipX + 50.0f, cardY + 38, 70, 22 }, 0.4f, 6, Fade(playerColor, 0.20f));
            DrawTextEx(assets.gameFont, "YOUR TURN", { ipX + 56.0f, cardY + 41 }, 14, 1, Fade(playerColor, blinkAlpha));
        }
        else {
            DrawTextEx(assets.gameFont, "waiting...", { ipX + 50.0f, cardY + 38 }, 14, 1, Color{ 142, 102, 36, 108 });
        }
    }

    // Count Pieces on Board
    int p1Count = 0, p2Count = 0;
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (match.board[x][y] == 1) p1Count++;
            else if (match.board[x][y] == 2) p2Count++;
        }
    }

    // Separator line
    DrawLineEx({ ipX + 12, ipY + 366.0f }, { ipX + ipW - 12, ipY + 366.0f }, 2.0f, Color{ 152, 95, 14, 112 });

    char scoreText[64];
    std::string p1ShortName = (players[0].name[0] != '\0') ? players[0].name : "P1";
    std::string p2ShortName = (players[1].name[0] != '\0') ? players[1].name : "P2";

    snprintf(scoreText, sizeof(scoreText), "%s: %d   %s: %d", p1ShortName.c_str(), p1Count, p2ShortName.c_str(), p2Count);
    Vector2 scoreSize = MeasureTextEx(assets.gameFont, scoreText, 15, 1);
    DrawTextEx(assets.gameFont, scoreText, { ipX + ipW / 2.0f - scoreSize.x / 2.0f, ipY + 372.0f }, 15, 1, Color{ 195, 155, 55, 160 });

    // Help Footer
    DrawRectangleRounded({ ipX + 8, ipY + ipH - 44, ipW - 16, 34 }, 0.3f, 6, Color{ 46, 22, 3, 165 });
    DrawRectangleRoundedLines({ ipX + 8, ipY + ipH - 44, ipW - 16, 34 }, 0.3f, 6, Color{ 152, 95, 14, 140 });
    Vector2 helpSize = MeasureTextEx(assets.gameFont, "ESC  Pause", 16, 1);
    DrawTextEx(assets.gameFont, "ESC  Pause", { ipX + ipW / 2.0f - helpSize.x / 2.0f, ipY + ipH - 35 }, 16, 1, Color{ 195, 152, 46, 172 });
}
void DrawMatchScreen(int screenWidth, int screenHeight, const MatchManager& match, const GameAssets& assets, const std::vector<PlayerInfo>& players) {
    // Screen background and vignette effect
    Rectangle srcBg = { 0, 0, (float)assets.bgMainMenu.width, (float)assets.bgMainMenu.height };
    Rectangle dstBg = { 0, 0, (float)screenWidth, (float)screenHeight };
    DrawTexturePro(assets.bgMainMenu, srcBg, dstBg, { 0, 0 }, 0.0f, WHITE);
    DrawRectangle(0, 0, screenWidth, screenHeight, ColorShadow);

    // Draw borders and corner ornaments
    DrawRectangleLinesEx({ 6, 6, (float)screenWidth - 12, (float)screenHeight - 12 }, 2.0f, ColorBronzeAlpha);
    DrawRectangleLinesEx({ 12, 12, (float)screenWidth - 24, (float)screenHeight - 24 }, 1.0f, ColorBronzeAlpha);
    DrawCornerOrnament(18, 18, 1, 1, ColorBronzeAlpha);
    DrawCornerOrnament(screenWidth - 18, 18, -1, 1, ColorBronzeAlpha);
    DrawCornerOrnament(18, screenHeight - 18, 1, -1, ColorBronzeAlpha);
    DrawCornerOrnament(screenWidth - 18, screenHeight - 18, -1, -1, ColorBronzeAlpha);

    // Calculate position and render the whole Caro board
    float bpW = (float)BOARD_SIZE * CELL + 58.0f;
    float bpH = (float)BOARD_SIZE * CELL + 58.0f;
    float bpX = screenWidth / 2.0f - bpW / 2.0f;
    float bpY = screenHeight / 2.0f - bpH / 2.0f;

    DrawCaroBoard(match, bpX, bpY);

    // Render the active player's blinking cursor
    // Need to recalculate grid origin (oX, oY) to position the cursor correctly
    float oX = bpX + 29.0f;
    float oY = bpY + 29.0f;

    float cx = oX + match.cursorX * CELL;
    float cy = oY + match.cursorY * CELL;

    float blink = 0.5f + 0.5f * sinf(GetTime() * 8.0f);
    Color cursorColor = (match.currentPlayer == 1) ? Fade(ColorBlueNeon, blink) : Fade(ColorRedNeon, blink);

    DrawRectangleLinesEx({ cx, cy, (float)CELL, (float)CELL }, 3.0f, cursorColor);

    float ipX = bpX + bpW + 12.0f;
    float ipY = bpY;
    float ipW = screenWidth - ipX - 14.0f;
    float ipH = bpH;

    // Check if enough space for drawing
    if (ipW > 100.0f) {
        DrawPlayerInfoPanel(ipX, ipY, ipW, ipH, match, assets, players);
    }
    if (match.isPaused) {
        DrawPauseOverlay(screenWidth, screenHeight, match, assets);
    }
}
//Draw notification of winner
void DrawWinnerDisplay(int screenWidth, float panelY, int winnerIdx, const std::vector<PlayerInfo>& players, const GameAssets& assets, float winTimer) {
    
    PlayerInfo winner;
    bool isDraw = (winnerIdx == 0);
    if (!isDraw) {
         winner = players[winnerIdx - 1];
    }
    else {
        winner = players[winnerIdx];
    }
    // Setup colors from winner data or fallbacks

    Color accent = isDraw ? GOLD : winner.color;
    if (accent.a == 0) accent = BLUE;
    const char* displayName = isDraw ? "DRAW" : winner.name;


    // Main panel dimensions and positioning
    float panelW = 580.0f;
    float panelH = 300.0f;
    float px = (float)screenWidth / 2.0f - panelW / 2.0f;
    float py = panelY;

    // Header settings for the integrated top section
    float hh = 60.0f;

    // Background glow with pulsing animation
    float glowAlpha = 0.06f + 0.04f * sinf(winTimer * 2.8f);
    for (int i = 5; i >= 1; i--) {
        DrawRectangleRounded({ px - i * 4.0f, py - i * 4.0f, panelW + i * 8.0f, panelH + i * 8.0f }, 0.12f, 8, Fade(accent, glowAlpha * i));
    }

    // Main panel fill
    DrawRectangleRounded({ px, py, panelW, panelH }, 0.07f, 8, ColorBrownDark);

    // Integrated header block (Visual style from DrawBaseMenu)
    DrawRectangleRounded({ px, py, panelW, hh }, 0.07f, 8, Fade(accent, 0.4f));
    DrawGlowLine(px + 22, py + hh, px + panelW - 22, accent);
    DrawDiamond(px + panelW / 2.0f, py + hh, 8, accent);
    DrawDiamond(px + panelW / 2.0f - 110.0f, py + hh, 4, accent);
    DrawDiamond(px + panelW / 2.0f + 110.0f, py + hh, 4, accent);

    // Header title inside the integrated top block
    const char* headerText = "MATCH FINISHED";
    Vector2 headerSize = MeasureTextEx(assets.gameFont, headerText, 24, 2);
    DrawTextEx(assets.gameFont, headerText, { (float)screenWidth / 2.0f - headerSize.x / 2.0f, py + hh / 2.0f - headerSize.y / 2.0f }, 35, 2, WHITE);

    // Main ornate border frame
    DrawOrnateFrame(px, py, panelW, panelH, accent, accent);

    // Inner decorative lines (Player color accent)
    DrawRectangleRoundedLines({ px + 10, py + 10, panelW - 20, panelH - 20 }, 0.12f, 8, Fade(accent, 0.35f));

    // Dynamic name scaling logic
    float namePulse = 52.0f * (1.0f + 0.05f * sinf(winTimer * 3.5f));
    Vector2 nameSize = MeasureTextEx(assets.gameFont, displayName, namePulse, 2);
    float tx = (float)screenWidth / 2.0f - nameSize.x / 2.0f;
    float ty = py + hh + 25.0f;

    // Render winner name with depth layers
    DrawTextEx(assets.gameFont, displayName, { tx + 4, ty + 4 }, namePulse, 2, Color{ 0, 0, 0, 150 });
    DrawTextEx(assets.gameFont, displayName, { tx + 1.5f, ty + 1.5f }, namePulse, 2, Color{ (unsigned char)(accent.r / 2), (unsigned char)(accent.g / 2), (unsigned char)(accent.b / 2), 200 });
    DrawTextEx(assets.gameFont, displayName, { tx, ty }, namePulse, 2, accent);

    // Victory sub-text announcement
    float winFS = 32.0f * (1.0f + 0.06f * sinf(winTimer * 4.5f));
    const char* subText = isDraw ? "NO WINNER!" : "WINS!"; 
    Vector2 winSize = MeasureTextEx(assets.gameFont, subText, winFS, 2);
    DrawTextEx(assets.gameFont, subText, { (float)screenWidth / 2.0f - winSize.x / 2.0f, ty + 70.0f }, winFS, 2, Color{ 252, 215, 45, 255 });

    // Winning piece preview icon
    float circleY = ty + 130.0f;
    if (isDraw) {
        DrawCircle((int)(screenWidth / 2.0f - 25), (int)circleY, 18, Color{ 30, 30, 30, 255 });
        DrawCircle((int)(screenWidth / 2.0f + 25), (int)circleY, 18, Color{ 240, 240, 240, 255 });
    }
    else {
        DrawCircle((int)(screenWidth / 2.0f), (int)circleY, 22, (winnerIdx == 1) ? Color{ 30, 30, 30, 255 } : Color{ 240, 240, 240, 255 });
        DrawCircleLines((int)(screenWidth / 2.0f), (int)circleY, 22, accent);
    }

    // Interaction prompts at the bottom
    float promptAlpha = 0.4f + 0.4f * sinf(winTimer * 2.0f);
    Color promptColor = Fade(WHITE, promptAlpha);

    // Define the two instruction strings
    const char* msgPlayAgain = "PRESS [ENTER] TO PLAY AGAIN";
    const char* msgMenu = "PRESS [ESC] TO RETURN TO MENU";

    // Measure both for horizontal centering
    Vector2 sizePlayAgain = MeasureTextEx(assets.gameFont, msgPlayAgain, 17, 1);
    Vector2 sizeMenu = MeasureTextEx(assets.gameFont, msgMenu, 17, 1);

    // Draw the first line (Play Again)
    DrawTextEx(assets.gameFont, msgPlayAgain,
        { (float)screenWidth / 2.0f - sizePlayAgain.x / 2.0f, py + panelH - 55.0f },
        17, 1, promptColor);

    // Draw the second line (Return to Menu)
    DrawTextEx(assets.gameFont, msgMenu,
        { (float)screenWidth / 2.0f - sizeMenu.x / 2.0f, py + panelH - 32.0f },
        17, 1, promptColor);
}
void DrawPauseOverlay(int sw, int sh, const MatchManager& match, const GameAssets& assets) {
    // Backdrop dimming effect
    DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.7f));

    // Upsized panel dimensions for better visibility
    float pw = 420.0f;
    float ph = 400.0f;
    float px = sw / 2.0f - pw / 2.0f;
    float py = sh / 2.0f - ph / 2.0f;

    // Main background container
    DrawRectangleRounded({ px, py, pw, ph }, 0.06f, 8, ColorBrownDark);

    // Dedicated Header Banner styling
    float hh = 70.0f; // Header height
    DrawRectangleRounded({ px, py, pw, hh }, 0.06f, 8, ColorBrownRust);
    DrawGlowLine(px + 20, py + hh, px + pw - 20, ColorGoldSolid); 

    // Header ornaments
    DrawDiamond(px + pw / 2.0f, py + hh, 8, ColorGoldSolid);
    DrawDiamond(px + 40, py + hh / 2.0f, 5, ColorGoldAlpha);
    DrawDiamond(px + pw - 40, py + hh / 2.0f, 5, ColorGoldAlpha);

    // Centered header title
    Vector2 tSize = MeasureTextEx(assets.gameFont, "PAUSED", 42, 3);
    DrawTextEx(assets.gameFont, "PAUSED", { sw / 2.0f - tSize.x / 2.0f, py + hh / 2.0f - tSize.y / 2.0f }, 42, 3, ColorGoldSolid);

    // Main ornate border for the entire panel
    DrawOrnateFrame(px, py, pw, ph, ColorBronzeSolid, ColorGoldSolid);

    // Vertical spacing for menu entries
    const char* options[] = { "RESUME GAME", "SAVE GAME", "QUIT TO MENU" };
    float startY = py + hh + 40.0f;

    for (int i = 0; i < 3; i++) {
        bool isSelected = (match.pauseSelected == i);
        float bx = sw / 2.0f - 130.0f;
        float by = startY + i * 65.0f;

        if (isSelected) {
            // Selection highlight box
            DrawRectangleRounded({ bx - 20, by - 8, 300, 45 }, 0.5f, 6, Fade(ColorGoldSolid, 0.12f));

            // Pulsing indicator arrows
            float shift = sinf(GetTime() * 12.0f) * 4.0f;
            DrawTextEx(assets.gameFont, ">>", { bx - 55 + shift, by + 4 }, 22, 2, ColorGoldSolid);
            DrawTextEx(assets.gameFont, "<<", { bx + 225 - shift, by + 4 }, 22, 2, ColorGoldSolid);
        }

        // Option label rendering
        Color textColor = isSelected ? ColorGoldSolid : ColorBronzeAlpha;
        DrawTextEx(assets.gameFont, options[i], { bx, by }, 28, 2, textColor);
    }

    // Navigation hint at the bottom
    const char* controls = "[W/S] Navigate  -  [ENTER] Select";
    Vector2 cSize = MeasureTextEx(assets.gameFont, controls, 16, 1);
    DrawTextEx(assets.gameFont, controls, { sw / 2.0f - cSize.x / 2.0f, py + ph - 30 }, 16, 1, ColorGoldAlpha);
}
// Handle NewGame Page flow
void DrawNewGameFlow(int screenWidth, int screenHeight, const GameAssets& assets, bool& backToMenu, bool& startMatch, std::vector<PlayerInfo> &players, MatchManager& match)
{
    static NewGameStep currentStep = STEP_CHOOSE_MODE;
    NewGameStep nextStep = currentStep;
    static int selectedModeSlot = 0;
    static int selectedOpponentSlot = 0;
    static int selectedBotDiffSlot = 0;

    static int letterCount1 = 0;
    static int letterCount2 = 0;
    static int activeField = 0;
    
    switch (currentStep) {

    case STEP_CHOOSE_MODE:
        DrawStepMode(screenWidth, screenHeight, assets, selectedModeSlot, backToMenu);
        if (IsKeyPressed(KEY_ENTER)) {
            // 0: Standard, 1: Block 2 Ends, 2: Renju
            match.gameMode = selectedModeSlot;
            nextStep = STEP_CHOOSE_OPPONENT;
            if (assets.clickSound.stream.buffer) PlaySound(assets.clickSound);
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            backToMenu = true;
            if (assets.clickSound.stream.buffer) PlaySound(assets.clickSound);
        }
        break;
    case STEP_CHOOSE_OPPONENT:
        DrawStepOpponent(screenWidth, screenHeight, assets, selectedOpponentSlot, backToMenu);

        if (IsKeyPressed(KEY_ENTER)) {
            if (selectedOpponentSlot == 0) {
                players[1].isBot = true;
                nextStep = STEP_CHOOSE_BOT_DIFFICULTY;
            }
            else {
                players[1].isBot = false;
                nextStep = STEP_ENTER_NAME;
            }
            if (assets.clickSound.stream.buffer) PlaySound(assets.clickSound);
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            nextStep = STEP_CHOOSE_MODE;
            if (assets.clickSound.stream.buffer) PlaySound(assets.clickSound);
        }
        break;
    case STEP_CHOOSE_BOT_DIFFICULTY:
        DrawStepBotDifficulty(screenWidth, screenHeight, assets, selectedBotDiffSlot, backToMenu);

        if (IsKeyPressed(KEY_ESCAPE)) {
            nextStep = STEP_CHOOSE_OPPONENT;
            backToMenu = false;
            if (assets.clickSound.stream.buffer) PlaySound(assets.clickSound);
        }
        if (IsKeyPressed(KEY_ENTER)) {
            nextStep = STEP_ENTER_NAME;
            if (assets.clickSound.stream.buffer) PlaySound(assets.clickSound);
        }
        break;
    case STEP_ENTER_NAME:
        DrawStepEnterName(screenWidth, screenHeight, assets, selectedOpponentSlot, players[0].name, letterCount1, players[1].name, letterCount2, activeField);
        if (IsKeyPressed(KEY_ENTER)) {
            if (letterCount1 > 0) {
                if (selectedOpponentSlot == 0 || (selectedOpponentSlot == 1 && letterCount2 > 0)) {
                    startMatch = true;
                }
            }
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (selectedOpponentSlot == 0) nextStep = STEP_CHOOSE_BOT_DIFFICULTY;
            else nextStep = STEP_CHOOSE_OPPONENT;
        }
        break;
    case STEP_READY:
        startMatch = true;
        nextStep = STEP_CHOOSE_MODE;
        break;
    }
    currentStep = nextStep;

    if (startMatch) {
        currentStep = STEP_CHOOSE_MODE;
        selectedModeSlot = 0;
        selectedOpponentSlot = 0;
        selectedBotDiffSlot = 0;
        letterCount1 = 0;
        letterCount2 = 0;
        activeField = 0;
    }
    if (backToMenu) {
        currentStep = STEP_CHOOSE_MODE;
        selectedModeSlot = 0;
        selectedOpponentSlot = 0;
        selectedBotDiffSlot = 0;
        letterCount1 = 0;
        letterCount2 = 0;
        activeField = 0;
        memset(players[0].name, 0, 16);
        memset(players[1].name, 0, 16);
        return;
    }
}

