#include "a.h"

void search(NAME* ary, char* name) {
    FILE* fp = fopen("data.txt", "r");
    int found = 0;

    if (fp == NULL) {
        printf("���� ���� ����\n");
        return;
    }

    printf("�˻��� �̸� �Է�: ");
    scanf("%s", name);

    while (fscanf(fp, "%s %s %s %s", ary->name, ary->mobile, ary->email, ary->address) != EOF) {
        if (strcmp(ary->name, name) == 0) {
            printf("=== �˻� ��� ===\n");
            printf("�̸�: %s\n", ary->name);
            printf("�޴���: %s\n", ary->mobile);
            printf("�̸���: %s\n", ary->email);
            printf("�ּ�: %s\n", ary->address);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("'%s' �̸��� ������ ã�� �� �����ϴ�.\n", name);
    }

    fclose(fp);
}
