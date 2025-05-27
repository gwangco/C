#include "a.h"

void update(NAME* ary, char* name) {
    FILE* fp = fopen("data.txt", "r");
    FILE* tempfp = fopen("temp.txt", "w");
    int choice = 0;
    int found = 0;
  

    if (!fp || !tempfp) {
        printf("파일 열기 실패\n");
        return;
    }
   
    printf("수정 또는 삭제할 명함의 이름을 입력: ");
    scanf("%s", name); 
    if (strcmp(ary->name, name) == 0) {
        printf("1. 수정\n");
        printf("2. 삭제\n");
        printf("선택: ");
        scanf("%d", &choice);
    }
    else {
        printf("이름이 존재하지 않습니다.");
    }


    while (fscanf(fp, "%s %s %s %s", ary->name, ary->mobile, ary->email, ary->address) != EOF) {
        if (strcmp(ary->name, name) == 0) { 
            found = 1; 

            if (choice == 1) {
                printf("새 휴대폰 번호 입력: ");
                scanf("%s", ary->mobile);
                printf("새 이메일 입력: ");
                scanf("%s", ary->email);
                printf("새 주소 입력: ");
                scanf("%s", ary->address);
               
                fprintf(tempfp, "%s %s %s %s\n", ary->name, ary->mobile, ary->email, ary->address);
                printf("명함 수정 완료\n");
                remove("data.txt");
                rename("temp.txt", "data.txt");
            }
            else if (choice == 2) { 
                
                printf("명함 삭제 완료\n");
            }
            else { 
                printf("잘못된 선택입니다. 기존 정보는 유지됩니다.\n");
                
                fprintf(tempfp, "%s %s %s %s\n", ary->name, ary->mobile, ary->email, ary->address);
            }
        }
        else { 
            fprintf(tempfp, "%s %s %s %s\n", ary->name, ary->mobile, ary->email, ary->address);
        }
    }

    fclose(fp);
    fclose(tempfp);

    remove("data.txt");
    rename("temp.txt", "data.txt");

    if (!found) {
        printf("'%s' 이름의 명함을 찾을 수 없습니다.\n", name);
    }
}