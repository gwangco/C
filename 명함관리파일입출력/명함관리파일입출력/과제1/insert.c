#include "a.h"

void insert(NAME* ary) {
    FILE* fp = fopen("data.txt", "a");
 

    if (fp == NULL) {
        printf("파일 열기 실패\n");
        return;
    }

    printf("이름 입력: ");
    scanf("%s", ary->name);

    printf("휴대폰 번호 입력: ");
    scanf("%s", ary->mobile);

    printf("이메일 입력: ");
    scanf("%s", ary->email);

    printf("주소 입력: ");
    scanf("%s", ary->address);
    fprintf(fp, "%s %s %s %s\n", ary->name, ary->mobile, ary->email, ary->address);
    fclose(fp);

    printf("명함이 저장되었습니다.\n");
}
