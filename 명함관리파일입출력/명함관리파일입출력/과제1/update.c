#include "a.h"

void update(NAME* ary, char* name) {
    FILE* fp = fopen("data.txt", "r");
    FILE* tempfp = fopen("temp.txt", "w");
    int choice = 0;
    int found = 0;
  

    if (!fp || !tempfp) {
        printf("���� ���� ����\n");
        return;
    }
   
    printf("���� �Ǵ� ������ ������ �̸��� �Է�: ");
    scanf("%s", name); 
    if (strcmp(ary->name, name) == 0) {
        printf("1. ����\n");
        printf("2. ����\n");
        printf("����: ");
        scanf("%d", &choice);
    }
    else {
        printf("�̸��� �������� �ʽ��ϴ�.");
    }


    while (fscanf(fp, "%s %s %s %s", ary->name, ary->mobile, ary->email, ary->address) != EOF) {
        if (strcmp(ary->name, name) == 0) { 
            found = 1; 

            if (choice == 1) {
                printf("�� �޴��� ��ȣ �Է�: ");
                scanf("%s", ary->mobile);
                printf("�� �̸��� �Է�: ");
                scanf("%s", ary->email);
                printf("�� �ּ� �Է�: ");
                scanf("%s", ary->address);
               
                fprintf(tempfp, "%s %s %s %s\n", ary->name, ary->mobile, ary->email, ary->address);
                printf("���� ���� �Ϸ�\n");
                remove("data.txt");
                rename("temp.txt", "data.txt");
            }
            else if (choice == 2) { 
                
                printf("���� ���� �Ϸ�\n");
            }
            else { 
                printf("�߸��� �����Դϴ�. ���� ������ �����˴ϴ�.\n");
                
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
        printf("'%s' �̸��� ������ ã�� �� �����ϴ�.\n", name);
    }
}