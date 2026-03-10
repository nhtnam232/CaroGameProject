#define _CRT_SECURE_NO_WARNINGS
#include "UI.h"
#include <algorithm>
#include <cmath>

// Định nghĩa hàm COLOR
Color COLOR(unsigned char r, unsigned char g, unsigned char b) {
    Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = 255;
    return color;
}

void UpdateSettings(GameSettings& settings, int selectedOption, Vector2 mousePoint, bool mousePressed) {
    // Update volume
    if ((selectedOption == 0 || selectedOption == 1) && mousePressed) {
        float minX = 600.0f;
        float maxX = 950.0f;
        float newX = mousePoint.x;
        newX = std::max(minX, std::min(newX, maxX));

        float volume = (newX - minX) / (maxX - minX);

        if (selectedOption == 0) {
            settings.musicVolume = volume;
        }
        else {
            settings.sfxVolume = volume;
        }
    }
}

void DrawSettingsMenu(int screenWidth, int screenHeight, GameSettings& settings, const GameAssets& assets, int& selectedOption, bool& backToMenu) {
    Vector2 mousePoint = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    // Drawing background
    Rectangle sourceRec = { 0.0f, 0.0f, (float)assets.bgMainMenu.width, (float)assets.bgMainMenu.height };
    Rectangle destRec = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
    Vector2 origin = { 0.0f, 0.0f };
    DrawTexturePro(assets.bgMainMenu, sourceRec, destRec, origin, 0.0f, WHITE);

    // Begin Drawing
    Rectangle vintageFrame = {
        screenWidth / 2.0f - 450.0f,
        screenHeight / 2.0f - 250.0f,
        900.0f,
        500.0f
    };

    // Vẽ khung nền mờ với viền
    DrawRectangleRec(vintageFrame, Fade(COLOR(139, 69, 19), 0.25f));
    DrawRectangleLinesEx(vintageFrame, 4.0f, Fade(COLOR(205, 133, 63), 0.9f));

    // Trang trí góc khung
    float cornerSize = 30.0f;
    float lineThickness = 4.0f;
    Color cornerColor = COLOR(218, 165, 32);

    // Góc trên bên trái
    DrawLineEx({ vintageFrame.x, vintageFrame.y + cornerSize },
        { vintageFrame.x, vintageFrame.y }, lineThickness, cornerColor);
    DrawLineEx({ vintageFrame.x, vintageFrame.y },
        { vintageFrame.x + cornerSize, vintageFrame.y }, lineThickness, cornerColor);

    // Góc trên bên phải
    DrawLineEx({ vintageFrame.x + vintageFrame.width - cornerSize, vintageFrame.y },
        { vintageFrame.x + vintageFrame.width, vintageFrame.y }, lineThickness, cornerColor);
    DrawLineEx({ vintageFrame.x + vintageFrame.width, vintageFrame.y },
        { vintageFrame.x + vintageFrame.width, vintageFrame.y + cornerSize }, lineThickness, cornerColor);

    // Góc dưới bên trái
    DrawLineEx({ vintageFrame.x, vintageFrame.y + vintageFrame.height - cornerSize },
        { vintageFrame.x, vintageFrame.y + vintageFrame.height }, lineThickness, cornerColor);
    DrawLineEx({ vintageFrame.x, vintageFrame.y + vintageFrame.height },
        { vintageFrame.x + cornerSize, vintageFrame.y + vintageFrame.height }, lineThickness, cornerColor);

    // Góc dưới bên phải
    DrawLineEx({ vintageFrame.x + vintageFrame.width - cornerSize, vintageFrame.y + vintageFrame.height },
        { vintageFrame.x + vintageFrame.width, vintageFrame.y + vintageFrame.height }, lineThickness, cornerColor);
    DrawLineEx({ vintageFrame.x + vintageFrame.width, vintageFrame.y + vintageFrame.height },
        { vintageFrame.x + vintageFrame.width, vintageFrame.y + vintageFrame.height - cornerSize }, lineThickness, cornerColor);

    // Tiêu đề SETTINGS
    Vector2 titlePos = { screenWidth / 2.0f - 150.0f, vintageFrame.y + 40.0f };
    DrawTextEx(assets.gameFont, "SETTINGS", titlePos, 70, 2, COLOR(255, 215, 0));

    // MUSIC VOLUME với icon nốt nhạc
    float startY = vintageFrame.y + 140.0f;

    // Vẽ icon nốt nhạc
    Color iconColor = (selectedOption == 0) ? COLOR(255, 215, 0) : COLOR(218, 165, 32);

    // Nốt nhạc chính (hình tròn)
    DrawCircle((int)(vintageFrame.x + 80.0f), (int)(startY + 15.0f), 10.0f, iconColor);
    // Thân nốt nhạc (hình chữ nhật đứng)
    DrawRectangle((int)(vintageFrame.x + 75.0f), (int)(startY), 10, 25, iconColor);

    // 3 nốt nhạc nhỏ bay lên
    for (int j = 0; j < 3; j++) {
        DrawCircle((int)(vintageFrame.x + 95.0f + j * 10), (int)(startY - 10.0f - j * 6), 4.0f,
            Fade(iconColor, 0.6f - j * 0.15f));
    }

    // Text MUSIC VOLUME
    Vector2 musicTextPos = { vintageFrame.x + 140.0f, startY };
    DrawTextEx(assets.gameFont, "MUSIC VOLUME", musicTextPos, 32, 1, COLOR(101, 67, 33));

    // Slider cho Music
    float sliderX = vintageFrame.x + 400.0f;
    float sliderY = startY + 15.0f;

    Rectangle sliderBg = { sliderX, sliderY, 350.0f, 18.0f };
    DrawRectangleRec(sliderBg, COLOR(139, 90, 43));
    DrawRectangleLinesEx(sliderBg, 3.0f, COLOR(101, 67, 33));

    Rectangle sliderFill = { sliderX, sliderY, settings.musicVolume * 350.0f, 18.0f };
    DrawRectangleRec(sliderFill, COLOR(218, 165, 32));

    float knobX = sliderX + (settings.musicVolume * 350.0f);
    float knobY = sliderY + 9.0f;

    Color musicKnobColor = (selectedOption == 0) ? COLOR(255, 140, 0) : COLOR(205, 133, 63);
    DrawCircle((int)knobX, (int)knobY, 16.0f, musicKnobColor);
    DrawCircleLines((int)knobX, (int)knobY, 16.0f, COLOR(101, 67, 33));

    // Phần trăm
    char musicVolText[10];
    sprintf_s(musicVolText, sizeof(musicVolText), "%d%%", (int)(settings.musicVolume * 100));

    Rectangle musicPercentBg = { sliderX + 370.0f, startY - 5.0f, 70.0f, 45.0f };
    DrawRectangleRec(musicPercentBg, Fade(COLOR(222, 184, 135), 0.5f));
    DrawRectangleLinesEx(musicPercentBg, 2.0f, COLOR(139, 69, 19));

    Vector2 musicPercentPos = { sliderX + 380.0f, startY };
    DrawTextEx(assets.gameFont, musicVolText, musicPercentPos, 30, 1, COLOR(101, 67, 33));

    // SFX VOLUME với icon loa
    float sfxY = vintageFrame.y + 240.0f;

    // Vẽ icon loa
    Color sfxIconColor = (selectedOption == 1) ? COLOR(255, 215, 0) : COLOR(218, 165, 32);

    // Thân loa (hình chữ nhật bo tròn)
    Rectangle speakerBody = { vintageFrame.x + 65.0f, sfxY, 30.0f, 30.0f };
    DrawRectangleRounded(speakerBody, 0.3f, 8, sfxIconColor);

    // SỬA: Vẽ viền bằng cách vẽ một hình chữ nhật bo tròn khác với màu viền
    // Cách 1: Dùng DrawRectangleRoundedLines với tham số màu
    DrawRectangleRoundedLines(speakerBody, 0.3f, 8, COLOR(101, 67, 33));

    // Miệng loa (hình loa kèn)
    Vector2 speakerMouth[3] = {
        { vintageFrame.x + 95.0f, sfxY + 5.0f },
        { vintageFrame.x + 115.0f, sfxY + 15.0f },
        { vintageFrame.x + 95.0f, sfxY + 25.0f }
    };
    DrawTriangle(speakerMouth[0], speakerMouth[1], speakerMouth[2], sfxIconColor);
    DrawTriangleLines(speakerMouth[0], speakerMouth[1], speakerMouth[2], COLOR(101, 67, 33));

    // Sóng âm thanh
    for (int j = 0; j < 3; j++) {
        float radius = 15.0f + j * 8.0f;
        DrawCircleLines((int)(vintageFrame.x + 125.0f + j * 5), (int)(sfxY + 15.0f), (int)radius,
            Fade(sfxIconColor, 0.5f - j * 0.1f));
    }

    // Text SFX VOLUME
    Vector2 sfxTextPos = { vintageFrame.x + 140.0f, sfxY };
    DrawTextEx(assets.gameFont, "SFX VOLUME", sfxTextPos, 32, 1, COLOR(101, 67, 33));

    // Slider cho SFX
    sliderY = sfxY + 15.0f;

    sliderBg = { sliderX, sliderY, 350.0f, 18.0f };
    DrawRectangleRec(sliderBg, COLOR(139, 90, 43));
    DrawRectangleLinesEx(sliderBg, 3.0f, COLOR(101, 67, 33));

    sliderFill = { sliderX, sliderY, settings.sfxVolume * 350.0f, 18.0f };
    DrawRectangleRec(sliderFill, COLOR(218, 165, 32));

    knobX = sliderX + (settings.sfxVolume * 350.0f);
    knobY = sliderY + 9.0f;

    Color sfxKnobColor = (selectedOption == 1) ? COLOR(255, 140, 0) : COLOR(205, 133, 63);
    DrawCircle((int)knobX, (int)knobY, 16.0f, sfxKnobColor);
    DrawCircleLines((int)knobX, (int)knobY, 16.0f, COLOR(101, 67, 33));

    // Phần trăm
    char sfxVolText[10];
    sprintf_s(sfxVolText, sizeof(sfxVolText), "%d%%", (int)(settings.sfxVolume * 100));

    Rectangle sfxPercentBg = { sliderX + 370.0f, sfxY - 5.0f, 70.0f, 45.0f };
    DrawRectangleRec(sfxPercentBg, Fade(COLOR(222, 184, 135), 0.5f));
    DrawRectangleLinesEx(sfxPercentBg, 2.0f, COLOR(139, 69, 19));

    Vector2 sfxPercentPos = { sliderX + 380.0f, sfxY };
    DrawTextEx(assets.gameFont, sfxVolText, sfxPercentPos, 30, 1, COLOR(101, 67, 33));

    // Vẽ nút BACK
    float backBtnWidth = 250.0f;
    float backBtnHeight = 70.0f;
    float backBtnX = screenWidth / 2.0f - backBtnWidth / 2.0f;
    float backBtnY = vintageFrame.y + vintageFrame.height - 80.0f;

    Rectangle backBtnRect = { backBtnX, backBtnY, backBtnWidth, backBtnHeight };

    bool isBackHovered = CheckCollisionPointRec(mousePoint, backBtnRect);

    Color backBtnColor = isBackHovered ? COLOR(218, 165, 32) : COLOR(160, 82, 45);
    DrawRectangleRec(backBtnRect, backBtnColor);
    DrawRectangleLinesEx(backBtnRect, 4.0f, COLOR(101, 67, 33));

    DrawLineEx({ backBtnX + 15, backBtnY + backBtnHeight - 15 },
        { backBtnX + backBtnWidth - 15, backBtnY + backBtnHeight - 15 }, 3.0f, COLOR(255, 215, 0));

    Vector2 backTextPos = { backBtnX + 90.0f, backBtnY + 20.0f };
    DrawTextEx(assets.gameFont, "BACK", backTextPos, 35, 1, WHITE);

    if (isBackHovered && mousePressed) {
        backToMenu = true;
        if (assets.clickSound.stream.buffer != NULL) {
            PlaySound(assets.clickSound);
        }
    }

    // Xử lý kéo thả slider
    if (selectedOption == 0) {
        UpdateSettings(settings, 0, mousePoint, mousePressed);
    }
    else if (selectedOption == 1) {
        UpdateSettings(settings, 1, mousePoint, mousePressed);
    }

    // Hướng dẫn
    Vector2 helpPos = { screenWidth / 2.0f - 220.0f, vintageFrame.y + vintageFrame.height + 20.0f };
    DrawTextEx(assets.gameFont, "Use W/S to switch - Click and drag slider",
        helpPos, 18, 1, Fade(WHITE, 0.7f));
}