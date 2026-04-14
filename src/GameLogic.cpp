#include "GameLogic.h"
#include "raylib.h"


//Doing later
bool IsValidMove_Standard(int board[15][15], int x, int y) {
    if (x < 0 || x >= 15 || y < 0 || y >= 15) {
        return false;
    }

    // A move is only valid if the cell is currently empty
    if (board[x][y] != 0) {
        return false;
    }

    return true;
}
bool IsValidMove_Block2Ends(int board[15][15], int x, int y) {
    if (x < 0 || x >= 15 || y < 0 || y >= 15) {
        return false;
    }

    // A move is only valid if the cell is currently empty
    if (board[x][y] != 0) {
        return false;
    }

    return true;
}
bool IsValidMove_Renju(int board[15][15], int x, int y, int player) {
    // Basic boundary and occupancy checks
    if (x < 0 || x >= 15 || y < 0 || y >= 15 || board[x][y] != 0) {
        return false;
    }

    // Renju restrictions only apply to the first player (Black / Player 1)
    // White (Player 2) can move anywhere and even create overlines
    if (player == 2) return true;

    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };
    int fourCount = 0;
    int threeCount = 0;

    for (int i = 0; i < 4; i++) {
        int count = 1;

        // Count consecutive pieces in both directions of the axis
        for (int step = 1; step < 7; step++) {
            int nx = x + dx[i] * step, ny = y + dy[i] * step;
            if (nx >= 0 && nx < 15 && ny >= 0 && ny < 15 && board[nx][ny] == player) count++;
            else break;
        }
        for (int step = 1; step < 7; step++) {
            int nx = x - dx[i] * step, ny = y - dy[i] * step;
            if (nx >= 0 && nx < 15 && ny >= 0 && ny < 15 && board[nx][ny] == player) count++;
            else break;
        }

        // Rule 1: Overline is forbidden for Black (more than 5 pieces)
        if (count > 5) return false;

        // Rule 2 & 3: Detecting potential 3s and 4s for double-move violations
        // Note: A real Renju engine needs complex check for "open" threes
        if (count == 4) fourCount++;
        if (count == 3) threeCount++;
    }

    // Rule 4: Double-4 violation
    if (fourCount >= 2) return false;

    // Rule 5: Double-3 violation (Simplified detection)
    if (threeCount >= 2) return false;

    return true;
}
bool CheckWin_Standard(int board[15][15], int x, int y, int player) {
    // Check 4 directions: Horizontal, Vertical, Diagonal, Anti-Diagonal
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    for (int i = 0; i < 4; i++) {
        int count = 1; // Start with the piece just placed

        // Count consecutive pieces in the positive direction
        for (int step = 1; step < 5; step++) {
            int nx = x + dx[i] * step;
            int ny = y + dy[i] * step;
            if (nx >= 0 && nx < 15 && ny >= 0 && ny < 15 && board[nx][ny] == player) {
                count++;
            }
            else {
                break;
            }
        }

        // Count consecutive pieces in the negative direction
        for (int step = 1; step < 5; step++) {
            int nx = x - dx[i] * step;
            int ny = y - dy[i] * step;
            if (nx >= 0 && nx < 15 && ny >= 0 && ny < 15 && board[nx][ny] == player) {
                count++;
            }
            else {
                break;
            }
        }

        // If at least 5 in a row, it's a win
        if (count >= 5) return true;
    }

    return false;
}
bool CheckWin_Block2Ends(int board[15][15], int x, int y, int player) {
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };
    int opponent = (player == 1) ? 2 : 1;

    for (int i = 0; i < 4; i++) {
        int count = 1;

        // Track the exact distance moved to identify boundary cells later
        int fStep = 0;
        while (true) {
            int nx = x + dx[i] * (fStep + 1);
            int ny = y + dy[i] * (fStep + 1);
            if (nx >= 0 && nx < 15 && ny >= 0 && ny < 15 && board[nx][ny] == player) {
                count++;
                fStep++;
            }
            else break;
        }

        int bStep = 0;
        while (true) {
            int nx = x - dx[i] * (bStep + 1);
            int ny = y - dy[i] * (bStep + 1);
            if (nx >= 0 && nx < 15 && ny >= 0 && ny < 15 && board[nx][ny] == player) {
                count++;
                bStep++;
            }
            else break;
        }

        // Standard win requires at least 5 in a row
        if (count >= 5) {
            // Check the cell immediately following the forward sequence
            int fx = x + dx[i] * (fStep + 1);
            int fy = y + dy[i] * (fStep + 1);
            bool blockedForward = (fx < 0 || fx >= 15 || fy < 0 || fy >= 15 || board[fx][fy] == opponent);

            // Check the cell immediately before the backward sequence
            int bx = x - dx[i] * (bStep + 1);
            int by = y - dy[i] * (bStep + 1);
            bool blockedBackward = (bx < 0 || bx >= 15 || by < 0 || by >= 15 || board[bx][by] == opponent);

            // Win is invalid only if BOTH ends are blocked by opponent or board edge
            if (!(blockedForward && blockedBackward)) return true;
        }
    }
    return false;
}
bool CheckWin_Renju(int board[15][15], int x, int y, int player) {
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    for (int i = 0; i < 4; i++) {
        int count = 1;

        // Explore the positive side of the current axis
        for (int step = 1; step < 6; step++) {
            int nx = x + dx[i] * step;
            int ny = y + dy[i] * step;
            if (nx >= 0 && nx < 15 && ny >= 0 && ny < 15 && board[nx][ny] == player) {
                count++;
            }
            else break;
        }

        // Explore the negative side of the current axis
        for (int step = 1; step < 6; step++) {
            int nx = x - dx[i] * step;
            int ny = y - dy[i] * step;
            if (nx >= 0 && nx < 15 && ny >= 0 && ny < 15 && board[nx][ny] == player) {
                count++;
            }
            else break;
        }

        // Win logic varies by player in Renju
        // Black (Player 1) MUST have exactly 5 to trigger a win
        if (player == 1) {
            if (count == 5) return true;
        }
        // White (Player 2) wins with 5 or any overline (6, 7, etc.)
        else {
            if (count >= 5) return true;
        }
    }

    return false;
}

//General function to handle logic
bool IsValidMove(int mode, int board[15][15], int x, int y, int player) {
    // Route to the specific validation logic based on the active game mode
    // Standard mode only checks boundaries and empty cells
    switch (mode) {
    case 1:  return IsValidMove_Block2Ends(board, x, y);
    case 2:  return IsValidMove_Renju(board, x, y, player);
    default: return IsValidMove_Standard(board, x, y);
    }
}

bool CheckWinCondition(int mode, int board[15][15], int x, int y, int player) {
    // Determine the winner by calling the win-check algorithm for the current mode
    // Returns true if the last move created a winning line
    switch (mode) {
    case 1:  return CheckWin_Block2Ends(board, x, y, player);
    case 2:  return CheckWin_Renju(board, x, y, player);
    default: return CheckWin_Standard(board, x, y, player);
    }
}
void UpdateMatchLogic(MatchManager& match, bool & backToMenu) {
    // Stop processing if the game is already finished
    if (match.isMatchOver) {
        return;
    }
    if (IsKeyReleased(KEY_ESCAPE)) {
        match.isPaused = !match.isPaused;
        match.pauseSelected = 0; 
    }
    if (match.isPaused) {
        if (IsKeyReleased(KEY_W) || IsKeyReleased(KEY_UP)) match.pauseSelected = (match.pauseSelected - 1 + 3) % 3;
        if (IsKeyReleased(KEY_S) || IsKeyReleased(KEY_DOWN)) match.pauseSelected = (match.pauseSelected + 1) % 3;

        if (IsKeyReleased(KEY_ENTER)) {
            if (match.pauseSelected == 0) {
                match.isPaused = false;
            }
            else if (match.pauseSelected == 1) {
                match.isPaused = false;
            }
            else if (match.pauseSelected == 2) {
                match.isPaused = false;
                backToMenu = true;
            }
        }
        return;
    }
    // Handle cursor navigation with arrow keys or WASD
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))    match.cursorY--;
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))  match.cursorY++;
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))  match.cursorX--;
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) match.cursorX++;

    // Keep the cursor within board boundaries (0 to 14)
    if (match.cursorX < 0) match.cursorX = 0;
    if (match.cursorX >= 15) match.cursorX = 14;
    if (match.cursorY < 0) match.cursorY = 0;
    if (match.cursorY >= 15) match.cursorY = 14;

    // Place a piece when pressing Space or Enter
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        if (IsValidMove(match.gameMode, match.board, match.cursorX, match.cursorY, match.currentPlayer)) {
            match.board[match.cursorX][match.cursorY] = match.currentPlayer;

            if (CheckWinCondition(match.gameMode, match.board, match.cursorX, match.cursorY, match.currentPlayer)) {
                match.isMatchOver = true;
                match.winner = match.currentPlayer;
                match.winTimer = 0.0f;
            }
            else {
                match.currentPlayer = (match.currentPlayer == 1) ? 2 : 1;
            }

            // Add sound place Piece later
        }
        else {
            // Add sound if the move is not valid
        }
    }

    // Quick debug trigger for testing the victory UI
    if (IsKeyPressed(KEY_T)) {
        match.isMatchOver = true;
        match.winner = 1;
        match.winTimer = 0.0f;
    }
}