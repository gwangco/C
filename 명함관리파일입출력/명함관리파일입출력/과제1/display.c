#include "a.h"

void display(NAME* ary) {
    FILE* fp = fopen("data.txt", "r");
    int count = 0;

    if (fp == NULL) {
        printf("저장된 명함이 없습니다.\n");
        return;
    }

    while (fscanf(fp, "%s %s %s %s", ary->name, ary->mobile, ary->email, ary->address) != EOF) {
        printf("=== %d번 명함 ===\n", ++count);
        printf("이름: %s\n", ary->name);
        printf("휴대폰: %s\n", ary->mobile);
        printf("이메일: %s\n", ary->email);
        printf("주소: %s\n", ary->address);
    }

    if (count == 0) {
        printf("저장된 명함이 없습니다.\n");
    }

    fclose(fp);
}
