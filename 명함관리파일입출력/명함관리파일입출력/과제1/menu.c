#include <stdio.h>

int menu() {
    char input[100];
    int choice;

    printf("===== 명함 관리 프로그램 =====\n");
    printf("0. 종료\n");
    printf("1. 추가\n");
    printf("2. 출력\n");
    printf("3. 검색\n");
    printf("4. 수정/삭제\n");
    printf("선택: ");

    fgets(input, sizeof(input), stdin);

    if (sscanf(input, "%d", &choice) != 1) {
        printf("숫자를 입력하세요.\n");
        return -1;
    }

    return choice;
}