#include "a.h"

void display(NAME* ary) {
    FILE* fp = fopen("data.txt", "r");
    int count = 0;

    if (fp == NULL) {
        printf("����� ������ �����ϴ�.\n");
        return;
    }

    while (fscanf(fp, "%s %s %s %s", ary->name, ary->mobile, ary->email, ary->address) != EOF) {
        printf("=== %d�� ���� ===\n", ++count);
        printf("�̸�: %s\n", ary->name);
        printf("�޴���: %s\n", ary->mobile);
        printf("�̸���: %s\n", ary->email);
        printf("�ּ�: %s\n", ary->address);
    }

    if (count == 0) {
        printf("����� ������ �����ϴ�.\n");
    }

    fclose(fp);
}
