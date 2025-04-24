#include <windows.h>
#include <stdio.h>
#include <conio.h>

// 창 속성 구조체
typedef struct {
    int width;       // 창 너비
    int height;      // 창 높이
    int location_x;  // 창 시작 X
    int location_y;  // 창 시작 Y
    char name[50];   // 창 제목
    int color;       // ANSI 배경색 (0~7)
} WindowProperties;

// 콘솔 커서를 (x,y)로 이동
void gotoxy(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main() {
    const int CON_W = 80, CON_H = 25;
    // ASCII 아트 프레임
    const char* catFrames[2] = { "(=^.^=)", "(=^o^=)" };
    int catFrame = 0, catCounter = 0;

    // 창 3개 설정
    WindowProperties wins[3] = {
        {30, 10, 10,  3, "나의 멋진 윈도우1", 3},
        {25,  8, 30,  8, "두 번째 윈도우",     2},
        {20, 12, 50,  5, "세 번째 윈도우",     5}
    };
    int winCount = 3;
    int prevLeftDown = 0;

    while (1) {
        // 1) 배경과 작업표시줄 그리기
        // 상하1, 좌우2 여백, 중간 파란색, 하단 회색(여백2 포함)
        system("cls");
        int x, y;
        for (y = 1; y < CON_H - 1; y++) {
            gotoxy(0, y);
            printf("  ");
            for (x = 2; x < CON_W - 2; x++) printf("\033[44m \033[0m");
            printf("  ");
        }
        gotoxy(0, CON_H - 1);
        printf("  ");
        for (x = 2; x < CON_W - 2; x++) printf("\033[47m \033[0m");
        printf("  ");

        // 2) 창 그리기
        for (int i = 0; i < winCount; i++) {
            int sx = wins[i].location_x;
            int sy = wins[i].location_y;
            int w = wins[i].width;
            int h = wins[i].height;
            // 배경색
            printf("\033[%dm", 40 + wins[i].color);
            for (int i2 = 0; i2 < h; i2++) {
                for (int j = 0; j < w; j++) {
                    gotoxy(sx + j, sy + i2);
                    if (i2 == 0 || i2 == 2 || i2 == h - 1) {
                        if (j == 0 || j == w - 1) printf("|");
                        else                       printf("-");
                    }
                    else {
                        if (j == 0 || j == w - 1) printf("|");
                        else                       printf(" ");
                    }
                }
            }
            // 제목
            gotoxy(sx + 1, sy + 1);
            printf("%s", wins[i].name);
            // X 버튼 (width-4)
            gotoxy(sx + w - 4, sy + 1);
            printf("\033[41m\033[37mX\033[0m");
            printf("\033[0m");
        }

        // 3) 마우스 위치 → 콘솔 좌표
        POINT p; GetCursorPos(&p);
        int cx = p.x / 10;
        int cy = p.y / 18;
        if (cx < 2)         cx = 2;
        if (cx > CON_W - 3) cx = CON_W - 3;
        if (cy < 1)         cy = 1;
        if (cy > CON_H - 2) cy = CON_H - 2;

        // 4) 커서와 고양이 그리기
        gotoxy(cx, cy);
        printf("<");
        // 고양이 애니 토글
        catCounter++;
        if (catCounter >= 5) {
            catCounter = 0;
            catFrame = 1 - catFrame;
        }
        gotoxy(cx + 1, cy);
        printf("%s", catFrames[catFrame]);

        // 5) 클릭 처리
        int leftDown = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
        if (leftDown && !prevLeftDown) {
            int handled = 0;
            for (int i = winCount - 1; i >= 0 && !handled; i--) {
                int sx = wins[i].location_x;
                int sy = wins[i].location_y;
                int w = wins[i].width;
                int h = wins[i].height;
                // X 클릭
                if (cy == sy + 1 && cx >= sx + w - 4 && cx < sx + w - 3) {
                    for (int k = i; k < winCount - 1; k++)
                        wins[k] = wins[k + 1];
                    winCount--;
                    handled = 1;
                }
                // 내부 클릭 → 최상단
                else if (cx >= sx && cx < sx + w && cy >= sy && cy < sy + h) {
                    WindowProperties tmp = wins[i];
                    for (int k = i; k < winCount - 1; k++)
                        wins[k] = wins[k + 1];
                    wins[winCount - 1] = tmp;
                    handled = 1;
                }
            }
        }
        prevLeftDown = leftDown;

        // 6) Q 종료
        if (GetAsyncKeyState(0x51) & 0x8000) break;
        Sleep(30);
    }

    system("cls");
    return 0;
}