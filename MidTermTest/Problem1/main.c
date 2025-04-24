#include <stdio.h>

int PassOrFail(int score) {
    if (score >= 50)
        return 1;
    else
        return 0;
}

int main(void) {
    // 1) 자신의 예상 평균 점수를 여기에 넣어보세요.
    int myScore = 65;

    // 2) PassOrFail 호출 후 결과에 따라 메시지 출력
    if (PassOrFail(myScore)) {
        printf("재시험 없습니다!\n");
    }
    else {
        printf("우리는 망했다...  재시험이다...\n");
    }

    return 0;
}
