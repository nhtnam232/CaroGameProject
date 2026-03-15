#include"UI.h"


void DrawKeycap(Font font, float x, float y, float width, const char* text, Color baseColor) {
    float height = 36.0f; // Height of the top face
    float depth = 6.0f;   // The 3D thickness

    // Draw the botthem part
    DrawRectangleRounded({ x, y + depth, width, height }, 0.2f, 4, baseColor);
    DrawRectangleRounded({ x, y + depth, width, height }, 0.2f, 4, COLORA(0, 0, 0, 80));

    // Draw the face
    DrawRectangleRounded({ x, y, width, height }, 0.2f, 4, baseColor);

    // Draw the edge
    DrawRectangleRoundedLines({ x, y, width, height }, 0.2f, 4, BLACK);

    // Center the text
    float textW = MeasureTextEx(font, text, 20, 1).x;
    DrawTextEx(font, text, { x + (width - textW) / 2.0f, y + 8.0f }, 20, 1, BLACK);
}

void DrawHelpMenu(int screenWidth, int screenHeight, const GameAssets& assets, int selectedIndex) // selectedIndex = 0: Controls, 1: Rules, 2: Tactics
{

    const char* title;
    if (selectedIndex == 0) title = "CONTROLS";
    else if (selectedIndex == 1) title = "RULES";
    else title = "TIPS AND TRICKS";
    Rectangle panel = DrawBaseMenu(screenWidth, screenHeight, assets, title);


    //get positions
    float px = panel.x;
    float py = panel.y;
    float pw = panel.width;
    float ph = panel.height;
    float tabW = 200.0f;
    float tabH = 45.0f;
    float gap = 15.0f;
    float totalTabsWidth = (tabW * 3) + (gap * 2);
    float startX = px + (pw - totalTabsWidth) / 2.0f; // Center all 3 tabs
    float tabY = py - tabH + 5.0f;
    int hh = 68.0f;

    // Hint Bar
    const char* hint = "A/D: Navigate  ESC: Return to Menu";
    float hintY = py + ph + 14.0f;
    float hintW = 300.0f;
    float hintX = screenWidth / 2.0f - hintW / 2.0f;
    DrawHint(screenWidth, screenHeight, hintX, hintY, hintW, hint, assets);

    // Tab Colors
    Color tabActiveFill = ColorBrownRust;
    Color tabInactiveFill = ColorBrownMedium;
    Color textActive = ColorGoldSolid;
    Color textInactive = GRAY;

    const char* tabNames[3] = { "CONTROLS", "RULES", "TIPS AND TRICKS" };

    // Drawing 3 tabs
    for (int i = 0; i < 3; i++) {
        float tx = startX + i * (tabW + gap);
        bool isActive = (selectedIndex == i);

        // Draw Tab Background & Border
        DrawRectangleRounded({ tx, tabY, tabW, tabH }, 0.2f, 4, isActive ? tabActiveFill : tabInactiveFill);
        DrawRectangleRoundedLines({ tx, tabY, tabW, tabH }, 0.2f, 4, ColorBronzeSolid);

        // Center text inside each tab
        float textW = MeasureTextEx(assets.gameFont, tabNames[i], 24, 1).x;
        DrawTextEx(assets.gameFont, tabNames[i], { tx + (tabW - textW) / 2.0f, tabY + 12.0f }, 24, 1, isActive ? textActive : textInactive);
    }


    if (selectedIndex == 0) {
        // TAB1: Controls
        Color keyColor = ColorGoldSolid;
        int fontSizeContent = 24;
        float contentY = py + hh + 80.0f;
        float keyX = px + 60.0f;
        float contentX = px + 330.0f;
        // Draw keycap
        DrawKeycap(assets.gameFont, keyX, contentY, 36.0f, "W", keyColor);
        DrawKeycap(assets.gameFont, keyX + 42.0f, contentY, 36.0f, "A", keyColor);
        DrawKeycap(assets.gameFont, keyX + 84.0f, contentY, 36.0f, "S", keyColor);
        DrawKeycap(assets.gameFont, keyX + 126.0f, contentY, 36.0f, "D", keyColor);

        DrawTextEx(assets.gameFont, "Move up/left/down/right", { contentX, contentY + 8.0f }, fontSizeContent, 1, ColorCreamSolid);
        contentY += 60.0f;

        DrawKeycap(assets.gameFont, keyX, contentY, 162.0f, "ENTER", keyColor);
        DrawTextEx(assets.gameFont, "Place piece/Select", { contentX, contentY + 8.0f }, fontSizeContent, 1, ColorCreamSolid);
        contentY += 60.0f;

        DrawKeycap(assets.gameFont, keyX, contentY, 162.0f, "ESC", keyColor);
        DrawTextEx(assets.gameFont, "Return to previous menu", { contentX, contentY + 8.0f }, fontSizeContent, 1, ColorCreamSolid);
        contentY += 60.0f;

        DrawKeycap(assets.gameFont, keyX, contentY, 162.0f, "L", keyColor);
        DrawTextEx(assets.gameFont, "Save game", { contentX, contentY + 8.0f }, fontSizeContent, 1, ColorCreamSolid);

    }
    else if (selectedIndex == 1) {
        // TAB2: RULES
        float contentY = py + hh + 40.0f;
        float contentX = px + 240.0f;
        float keyX = contentX - 180.0f;
        float centerX = px + pw / 2.0f;
        float ruleY = contentY - 20.0f;
        float ruleX = contentX - 180.0f;
        float lineSpacing = 28.0f;
        int fontSizeSubTitle = 35;
        int fontSizeContent = 24;
        //Objective
        const char* subTitle1 = "General Rules";
        float t1Width = MeasureTextEx(assets.gameFont, subTitle1, fontSizeSubTitle, 1).x;
        DrawTextEx(assets.gameFont, subTitle1, { centerX - t1Width / 2.0f, ruleY }, fontSizeSubTitle, 1, ColorGoldSolid);
        ruleY += lineSpacing;
        DrawTextEx(assets.gameFont, "1. Places 5 pieces in a continuous row (horizontal, vertical, or diagonal) to victory.", { ruleX, ruleY }, fontSizeContent, 1, ColorCreamSolid);
        ruleY += lineSpacing;
        DrawTextEx(assets.gameFont, "2. Player (X) plays first move.", { ruleX, ruleY }, fontSizeContent, 1, ColorCreamSolid);
        ruleY += lineSpacing;
        DrawTextEx(assets.gameFont, "3. Players take turns placing one piece on an empty square.", { ruleX, ruleY }, fontSizeContent, 1, ColorCreamSolid);
        ruleY += lineSpacing;

        //Special Modes
        const char* subTitle2 = "Special Rules (Modes)";
        float t2Width = MeasureTextEx(assets.gameFont, subTitle2, fontSizeSubTitle, 1).x;
        DrawTextEx(assets.gameFont, subTitle2, { centerX - t2Width / 2.0f, ruleY }, fontSizeSubTitle, 1, ColorGoldSolid);
        ruleY += lineSpacing;

        //Standard Caro Mode
        const char* mode1 = "[ Standard Gomoku ]";
        DrawTextEx(assets.gameFont, mode1, { ruleX, ruleY }, fontSizeContent, 1, ColorBlueNeon);
        ruleY += lineSpacing;
        DrawTextEx(assets.gameFont, "Get 5 pieces in a row to win. No restrictions. 6+ pieces also win.", { ruleX, ruleY }, fontSizeContent, 1, ColorCreamSolid);
        ruleY += lineSpacing;

        //Block 2 ends mode
        const char* mode2 = "[ Block 2 Ends ]";
        DrawTextEx(assets.gameFont, mode2, { ruleX, ruleY }, fontSizeContent, 1, ColorPinkNeon);
        ruleY += lineSpacing;

        // Split long sentence into 2 lines
        DrawTextEx(assets.gameFont, "A line of 5 pieces blocked at BOTH ends (O X X X X X O) is NOT a win. ", { ruleX, ruleY }, fontSizeContent, 1, ColorCreamSolid);
        ruleY += lineSpacing;
        DrawTextEx(assets.gameFont, "You must have at least one open end.", { ruleX, ruleY }, fontSizeContent, 1, ColorCreamSolid);

        ruleY += lineSpacing;
        //Renju
        const char* mode3 = "[ Renju Mode ]";
        DrawTextEx(assets.gameFont, mode3, { ruleX, ruleY }, fontSizeContent, 1, ColorLightGreenSolid);
        ruleY += lineSpacing;

        // Split long sentence into 2 lines explaining Renju fouls
        DrawTextEx(assets.gameFont, "Player (X) is restricted: NO double-threes, NO double-fours, and NO 6+ pieces", { ruleX, ruleY }, fontSizeContent, 1, ColorCreamSolid);
        ruleY += lineSpacing;
        DrawTextEx(assets.gameFont, "Player (O) has no restrictions.", { ruleX, ruleY }, fontSizeContent, 1, ColorCreamSolid);
    }
    else {
        // TAB 3: TIPS AND TRICKS
        float contentY = py + hh + 40.0f;
        float contentX = px + 240.0f;
        float keyX = contentX - 180.0f;
        float centerX = px + pw / 2.0f;
        float tacticY = contentY - 20.0f;
        float tacticX = contentX - 180.0f;
        float lineSpacing = 40.0f;
        int fontSizeTitle = 35;
        int fontSizeContent = 24;

        // Title
        const char* title3 = "Tips and Tricks";
        float t3Width = MeasureTextEx(assets.gameFont, title3, fontSizeTitle, 1).x;
        DrawTextEx(assets.gameFont, title3, { centerX - t3Width / 2.0f, tacticY }, fontSizeTitle, 1, ColorGoldSolid);
        tacticY += 60.0f;

        //Setup 2-column layout
        float labelX = tacticX;
        float descX = tacticX + 230.0f;

        // Tip 1
        DrawTextEx(assets.gameFont, "[1] Control Center:", { labelX, tacticY }, fontSizeContent, 1, ColorBlueNeon); // Blue
        DrawTextEx(assets.gameFont, "Start in the middle to create more opportunities.", { descX, tacticY + 2.0f }, fontSizeContent, 1, ColorCreamSolid);
        tacticY += lineSpacing;

        // Tip 2
        DrawTextEx(assets.gameFont, "[2] Open Fours:", { labelX, tacticY }, fontSizeContent, 1, ColorGoldSolid); // Gold
        DrawTextEx(assets.gameFont, "4 pieces with both ends empty = Guaranteed win.", { descX, tacticY + 2.0f }, fontSizeContent, 1, ColorCreamSolid);
        tacticY += lineSpacing;

        // Tip 3
        DrawTextEx(assets.gameFont, "[3] Double Threat:", { labelX, tacticY }, fontSizeContent, 1, ColorPinkNeon); // Pink
        DrawTextEx(assets.gameFont, "Create two winning paths simultaneously.", { descX, tacticY + 2.0f }, fontSizeContent, 1, ColorCreamSolid);
        tacticY += lineSpacing;

        // Tip 4
        DrawTextEx(assets.gameFont, "[4] Block Early:", { labelX, tacticY }, fontSizeContent, 1, ColorCyanSolid); // Cyan
        DrawTextEx(assets.gameFont, "Always block the opponent's open 3s immediately.", { descX, tacticY + 2.0f }, fontSizeContent, 1, ColorCreamSolid);
        tacticY += lineSpacing;

        // Tip 5
        DrawTextEx(assets.gameFont, "[5] Plan Ahead:", { labelX, tacticY }, fontSizeContent, 1, ColorLightGreenSolid); // Light Green
        DrawTextEx(assets.gameFont, "Think 2-3 moves ahead, don't just react to them.", { descX, tacticY + 2.0f }, fontSizeContent, 1, ColorCreamSolid);
    }
}