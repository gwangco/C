#include <stdio.h>

void human(int(*ip)[3]) {
	int row = 0, col = 0;

    while (1) {
        printf("행값 입력 : ");
        scanf("%d", &row);
        printf("열값 입력 : ");
        scanf("%d", &col);

        if (row < 0 || row > 2 || col < 0 || col > 2) {
            printf("잘못된 위치입니다. 다시 입력하세요.\n");
            continue;
        }
        if (ip[row][col] != 0) {
            printf("이미 착수한 자리입니다! 다른 곳을 선택하세요.\n");
            continue;
        }
        ip[row][col] = 2;
        break;
    }
}