#include <windows.h>
#include <stdio.h>
#include <conio.h>

// â �Ӽ� ����ü
typedef struct {
    int width;       // â �ʺ�
    int height;      // â ����
    int location_x;  // â ���� X
    int location_y;  // â ���� Y
    char name[50];   // â ����
    int color;       // ANSI ���� (0~7)
} WindowProperties;

// �ܼ� Ŀ���� (x,y)�� �̵�
void gotoxy(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main() {
    const int CON_W = 80, CON_H = 25;
    // ASCII ��Ʈ ������
    const char* catFrames[2] = { "(=^.^=)", "(=^o^=)" };
    int catFrame = 0, catCounter = 0;

    // â 3�� ����
    WindowProperties wins[3] = {
        {30, 10, 10,  3, "���� ���� ������1", 3},
        {25,  8, 30,  8, "�� ��° ������",     2},
        {20, 12, 50,  5, "�� ��° ������",     5}
    };
    int winCount = 3;
    int prevLeftDown = 0;

    while (1) {
        // 1) ���� �۾�ǥ���� �׸���
        // ����1, �¿�2 ����, �߰� �Ķ���, �ϴ� ȸ��(����2 ����)
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

        // 2) â �׸���
        for (int i = 0; i < winCount; i++) {
            int sx = wins[i].location_x;
            int sy = wins[i].location_y;
            int w = wins[i].width;
            int h = wins[i].height;
            // ����
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
            // ����
            gotoxy(sx + 1, sy + 1);
            printf("%s", wins[i].name);
            // X ��ư (width-4)
            gotoxy(sx + w - 4, sy + 1);
            printf("\033[41m\033[37mX\033[0m");
            printf("\033[0m");
        }

        // 3) ���콺 ��ġ �� �ܼ� ��ǥ
        POINT p; GetCursorPos(&p);
        int cx = p.x / 10;
        int cy = p.y / 18;
        if (cx < 2)         cx = 2;
        if (cx > CON_W - 3) cx = CON_W - 3;
        if (cy < 1)         cy = 1;
        if (cy > CON_H - 2) cy = CON_H - 2;

        // 4) Ŀ���� ����� �׸���
        gotoxy(cx, cy);
        printf("<");
        // ����� �ִ� ���
        catCounter++;
        if (catCounter >= 5) {
            catCounter = 0;
            catFrame = 1 - catFrame;
        }
        gotoxy(cx + 1, cy);
        printf("%s", catFrames[catFrame]);

        // 5) Ŭ�� ó��
        int leftDown = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
        if (leftDown && !prevLeftDown) {
            int handled = 0;
            for (int i = winCount - 1; i >= 0 && !handled; i--) {
                int sx = wins[i].location_x;
                int sy = wins[i].location_y;
                int w = wins[i].width;
                int h = wins[i].height;
                // X Ŭ��
                if (cy == sy + 1 && cx >= sx + w - 4 && cx < sx + w - 3) {
                    for (int k = i; k < winCount - 1; k++)
                        wins[k] = wins[k + 1];
                    winCount--;
                    handled = 1;
                }
                // ���� Ŭ�� �� �ֻ��
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

        // 6) Q ����
        if (GetAsyncKeyState(0x51) & 0x8000) break;
        Sleep(30);
    }

    system("cls");
    return 0;
}