#include <stdio.h>

void human(int(*ip)[3]) {
	int row = 0, col = 0;

    while (1) {
        printf("�ప �Է� : ");
        scanf("%d", &row);
        printf("���� �Է� : ");
        scanf("%d", &col);

        if (row < 0 || row > 2 || col < 0 || col > 2) {
            printf("�߸��� ��ġ�Դϴ�. �ٽ� �Է��ϼ���.\n");
            continue;
        }
        if (ip[row][col] != 0) {
            printf("�̹� ������ �ڸ��Դϴ�! �ٸ� ���� �����ϼ���.\n");
            continue;
        }
        ip[row][col] = 2;
        break;
    }
}