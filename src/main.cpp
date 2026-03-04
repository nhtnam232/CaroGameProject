#include "raylib.h"

int main() {
    // 1. Khởi tạo cửa sổ
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Test - Bouncing Box");

    SetTargetFPS(60); // Đảm bảo game chạy mượt 60 khung hình/giây

    // --- KHAI BÁO BIẾN CHO "CỤC" HÌNH VUÔNG ---
    float boxX = 350.0f;  // Tọa độ X ban đầu
    float boxY = 250.0f;  // Tọa độ Y ban đầu
    float boxSize = 60.0f; // Kích thước cục vuông (60x60 pixel)

    // Tốc độ di chuyển (số pixel mỗi khung hình)
    float speedX = 5.0f;
    float speedY = 4.0f;

    // 2. Vòng lặp game chính
    while (!WindowShouldClose()) {

        // --- PHẦN UPDATE: CẬP NHẬT TỌA ĐỘ TỪNG KHUNG HÌNH ---
        boxX += speedX;
        boxY += speedY;

        // Xử lý va chạm: Nếu đụng mép màn hình trái/phải -> Đảo ngược tốc độ X
        if ((boxX <= 0) || (boxX + boxSize >= screenWidth)) {
            speedX *= -1.0f;
        }
        // Xử lý va chạm: Nếu đụng mép màn hình trên/dưới -> Đảo ngược tốc độ Y
        if ((boxY <= 0) || (boxY + boxSize >= screenHeight)) {
            speedY *= -1.0f;
        }

        // --- PHẦN DRAW: VẼ RA MÀN HÌNH ---
        BeginDrawing();

        ClearBackground(RAYWHITE); // Xóa dấu vết cũ của khung hình trước

        // Vẽ cục hình vuông màu đỏ
        DrawRectangle((int)boxX, (int)boxY, (int)boxSize, (int)boxSize, MAROON);

        // In dòng chữ thông báo
        DrawText("Cuc mau do dang nhay qua nhay lai!", 200, 20, 20, LIGHTGRAY);

        EndDrawing();
    }

    // 3. Đóng cửa sổ, giải phóng bộ nhớ
    CloseWindow();
    return 0;
}