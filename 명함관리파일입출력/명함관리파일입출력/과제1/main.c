#include "a.h"

int main(void) {
    int choice;
    NAME ary[SIZE] = { 0 };
    char name[20] = { 0 };

    while (1) {
        choice = menu();

        if (choice == 0) {
            printf("프로그램 종료\n");
            exit(0);
        }
        else if (choice == 1) {
            insert(ary);
        }
        else if (choice == 2) {
            display(ary);
        }
        else if (choice == 3) {
            search(ary,name);
        }
        else if (choice == 4) {
            update(ary,name);
        }
     

        system("pause");
        system("cls");
    }

    return 0;
}
