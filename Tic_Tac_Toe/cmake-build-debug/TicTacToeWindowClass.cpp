#include <windows.h>
#include <string>

const int GRID_SIZE = 3; // 3x3 grid for Tic Tac Toe
char board[GRID_SIZE][GRID_SIZE]; // Tic Tac Toe board
bool playerX = true; // True for X, false for O
bool gameEnded = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ResetBoard();
void DrawBoard(HDC hdc);
void DrawSymbol(HDC hdc, int row, int col, char symbol);
bool CheckWinner(char symbol);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const char CLASS_NAME[] = "TicTacToeWindowClass";
    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
            0,
            CLASS_NAME,
            "Tic Tac Toe",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
            NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_LBUTTONDOWN: {
            if (gameEnded) {
                ResetBoard();
                InvalidateRect(hwnd, NULL, TRUE);
                gameEnded = false;
                return 0;
            }

            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);
            int col = xPos / 100;
            int row = yPos / 100;

            if (col < GRID_SIZE && row < GRID_SIZE && board[row][col] == '\0') {
                board[row][col] = playerX ? 'X' : 'O';
                playerX = !playerX;

                InvalidateRect(hwnd, NULL, TRUE);

                if (CheckWinner('X')) {
                    gameEnded = true;
                    MessageBox(hwnd, "Player X Wins!", "Game Over", MB_OK);
                } else if (CheckWinner('O')) {
                    gameEnded = true;
                    MessageBox(hwnd, "Player O Wins!", "Game Over", MB_OK);
                }
            }
            return 0;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            DrawBoard(hdc);

            for (int row = 0; row < GRID_SIZE; row++) {
                for (int col = 0; col < GRID_SIZE; col++) {
                    if (board[row][col] != '\0') {
                        DrawSymbol(hdc, row, col, board[row][col]);
                    }
                }
            }

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void DrawBoard(HDC hdc) {
    int cellSize = 100;
    for (int i = 1; i < GRID_SIZE; i++) {
        MoveToEx(hdc, i * cellSize, 0, NULL);
        LineTo(hdc, i * cellSize, 300);
        MoveToEx(hdc, 0, i * cellSize, NULL);
        LineTo(hdc, 300, i * cellSize);
    }
}

void DrawSymbol(HDC hdc, int row, int col, char symbol) {
    int cellSize = 100;
    int x = col * cellSize + cellSize / 2;
    int y = row * cellSize + cellSize / 2;

    if (symbol == 'X') {
        MoveToEx(hdc, x - 30, y - 30, NULL);
        LineTo(hdc, x + 30, y + 30);
        MoveToEx(hdc, x + 30, y - 30, NULL);
        LineTo(hdc, x - 30, y + 30);
    } else if (symbol == 'O') {
        Ellipse(hdc, x - 30, y - 30, x + 30, y + 30);
    }
}

bool CheckWinner(char symbol) {
    // Check rows and columns
    for (int i = 0; i < GRID_SIZE; i++) {
        if ((board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) ||
            (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol)) {
            return true;
        }
    }
    if ((board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) ||
        (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol)) {
        return true;
    }
    return false;
}

void ResetBoard() {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            board[row][col] = '\0';
        }
    }
    playerX = true;
}
