#include <stdio.h>

int PassOrFail(int score) {
    if (score >= 50)
        return 1;
    else
        return 0;
}

int main(void) {
    // 1) �ڽ��� ���� ��� ������ ���⿡ �־����.
    int myScore = 65;

    // 2) PassOrFail ȣ�� �� ����� ���� �޽��� ���
    if (PassOrFail(myScore)) {
        printf("����� �����ϴ�!\n");
    }
    else {
        printf("�츮�� ���ߴ�...  ������̴�...\n");
    }

    return 0;
}
