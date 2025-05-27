#include "a.h"

void search(NAME* ary, char* name) {
    FILE* fp = fopen("data.txt", "r");
    int found = 0;

    if (fp == NULL) {
        printf("파일 열기 실패\n");
        return;
    }

    printf("검색할 이름 입력: ");
    scanf("%s", name);

    while (fscanf(fp, "%s %s %s %s", ary->name, ary->mobile, ary->email, ary->address) != EOF) {
        if (strcmp(ary->name, name) == 0) {
            printf("=== 검색 결과 ===\n");
            printf("이름: %s\n", ary->name);
            printf("휴대폰: %s\n", ary->mobile);
            printf("이메일: %s\n", ary->email);
            printf("주소: %s\n", ary->address);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("'%s' 이름의 명함을 찾을 수 없습니다.\n", name);
    }

    fclose(fp);
}
