#include "a.h"

void insert(NAME* ary) {
    FILE* fp = fopen("data.txt", "a");
 

    if (fp == NULL) {
        printf("���� ���� ����\n");
        return;
    }

    printf("�̸� �Է�: ");
    scanf("%s", ary->name);

    printf("�޴��� ��ȣ �Է�: ");
    scanf("%s", ary->mobile);

    printf("�̸��� �Է�: ");
    scanf("%s", ary->email);

    printf("�ּ� �Է�: ");
    scanf("%s", ary->address);
    fprintf(fp, "%s %s %s %s\n", ary->name, ary->mobile, ary->email, ary->address);
    fclose(fp);

    printf("������ ����Ǿ����ϴ�.\n");
}
