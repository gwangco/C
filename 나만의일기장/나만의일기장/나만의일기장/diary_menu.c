#include "a.h"

// ���� �޴��� ȭ�鿡 ǥ���ϴ� �Լ�
void display_main_menu(int width) {
    draw_border(width, '=');
    printf("##");
    int menu_padding = (width - 4 - strlen(" ������ �ϱ��� �޴� ")) / 2;
    for (int i = 0; i < menu_padding; i++) printf(" ");
    printf(" ������ �ϱ��� �޴� ");
    for (int i = 0; i < width - 4 - menu_padding - strlen(" ������ �ϱ��� �޴� "); i++) printf(" ");
    printf("##\n");
    draw_border(width, '=');

    printf("1. �� �ϱ� �ۼ�\n");
    printf("2. �ϱ� ��� ����\n");
    printf("3. �ϱ� ����\n");
    printf("4. �α׾ƿ�\n");
    printf("5. ���α׷� ����\n");
}

// ������� ���� �޴� ���ÿ� ���� �� ��� �Լ��� ȣ���ϴ� �Լ�
void handle_menu_choice(int choice, int page_width, MYSQL* db_conn, int* logged_in_status) {
    switch (choice) {
    case 1:
        write_diary();
        break;
    case 2:
        list_diaries();
        break;
    case 3:
        view_diary(page_width);
        break;
    case 4: // �α׾ƿ�
        printf("\n�α׾ƿ��Ǿ����ϴ�. �ٽ� �α���/ȸ������ ȭ������ ���ư��ϴ�.\n");
        *logged_in_status = 0; // �α��� ���¸� 0(�α׾ƿ�)���� ����
        break;
    case 5: // ���α׷� ����
        printf("\n�ϱ����� �����մϴ�. �ȳ��� �輼��!\n");
        *logged_in_status = -1; // ���α׷� ���Ḧ �ǹ��ϴ� Ư�� ��
        break;
    default:
        printf("�߸��� �����Դϴ�. �ٽ� �Է����ּ���.\n");
        break;
    }
}

// ���� �޴�(�α���/ȸ������/ã��)�� ȭ�鿡 ǥ���ϴ� �Լ�
void display_auth_menu() {
    printf("\n************************************\n");
    printf("* ������ �ϱ��� - ����         *\n");
    printf("************************************\n");
    printf("1. �α���\n");
    printf("2. ȸ������\n");
    printf("3. ���̵�/��й�ȣ ã��\n"); 
    printf("4. ���α׷� ����\n"); 
    printf("����: ");
}

// ������� ���� �޴� ���ÿ� ���� �� ��� �Լ��� ȣ���ϴ� �Լ�
int handle_auth_choice(int choice, MYSQL* db_conn, int* logged_in_status) {
    int result = 0; // �⺻������ �α��� ���� (0)

    switch (choice) {
    case 1: // �α���
        result = login_user(db_conn); // �α��� �õ�
        *logged_in_status = result; // �α��� ���� ���� ������Ʈ
        break;
    case 2: // ȸ������
        register_user(db_conn); // ȸ������ �õ�
        // ȸ������ �����ص� �ٷ� �α��ε� ���� �ƴϹǷ� logged_in_status�� �������� ����
        break;
    case 3: // ���̵�/��й�ȣ ã��
        find_id_password(db_conn); // ���̵�/��й�ȣ ã�� �Լ� ȣ��
        break;
    case 4: // ���α׷� ����
        printf("���α׷��� �����մϴ�.\n");
        result = -1; // ���α׷� ���Ḧ �ǹ�
        *logged_in_status = -1; // main ������ ����ǵ��� ���� ����
        break;
    default:
        printf("�߸��� �����Դϴ�. �ٽ� �Է����ּ���.\n");
        break;
    }
    return result; // �α��� ����(1), ����(0), ���α׷� ����(-1)
}