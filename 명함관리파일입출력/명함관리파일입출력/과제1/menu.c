#include <stdio.h>

int menu() {
    char input[100];
    int choice;

    printf("===== ���� ���� ���α׷� =====\n");
    printf("0. ����\n");
    printf("1. �߰�\n");
    printf("2. ���\n");
    printf("3. �˻�\n");
    printf("4. ����/����\n");
    printf("����: ");

    fgets(input, sizeof(input), stdin);

    if (sscanf(input, "%d", &choice) != 1) {
        printf("���ڸ� �Է��ϼ���.\n");
        return -1;
    }

    return choice;
}