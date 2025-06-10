#include "a.h" // ���� ��� ���� ����

int main() {
#ifdef _WIN32
    system("mkdir diary"); // 'diary' ���� ���� �õ� (Windows ����)
#endif

    MYSQL* db_conn = NULL; // MySQL ���� ������
    int page_width = 80;  // ȭ�� �ʺ�

    // ���� ���α׷� ����
    while (1) {
        // �����ͺ��̽� ����
        db_conn = init_db_connection();
        if (db_conn == NULL) {
            fprintf(stderr, "�����ͺ��̽� ���ῡ �����߽��ϴ�. ���α׷��� �����մϴ�.\n");
            return 1;
        }

        int logged_in = 0; // �α��� ���� (0:�α׾ƿ�, 1:�α���, -1:���α׷� ����)

        // �α���/ȸ������/ã�� ����
        while (!logged_in) {
            display_auth_menu(); // ���� �޴� ���
            int auth_choice;
            scanf("%d", &auth_choice);
            getchar(); // �Է� ���� ����

            // ���� �޴� ���� ó�� �Լ� ȣ��
            // �� �Լ��� logged_in ���¸� �����ϰ�, ���α׷� ���� ����(-1)�� ��ȯ�� �� ����
            int auth_result = handle_auth_choice(auth_choice, db_conn, &logged_in);

            if (auth_result == -1) { // ���α׷� ���� ���� ��
                logged_in = -1; // ���� ������ �����ϵ��� logged_in ���� ����
                break; // ���� ���� ����
            }
            system("pause");
            system("cls");
            // auth_result�� 1�̸� logged_in�� 1�� �����Ǿ� ���� ���� ���� (�α��� ����)
            // auth_result�� 0�̸� �α��� ����/ȸ������ ���̹Ƿ� ���� ���� ���
        }

        // ���� �������� ������ �� (�α��� ���� �Ǵ� ���α׷� ����)
        if (logged_in == -1) { // ���α׷� ���ᰡ ���õǾ��ٸ�
            close_db_connection(db_conn); // DB ���� �ݱ�
            break; // ���� ���α׷� ���� ����
        }

        // --- �α��� ���� �� �ϱ��� ���� �޴� ---
        int choice;
        while (logged_in == 1) { // �α��� ���°� �����Ǵ� ����
            display_main_menu(page_width); // ���� �ϱ��� �޴� ���
            printf("����: ");
            scanf("%d", &choice);
            getchar(); // �Է� ���� ����

            // �ϱ��� �޴� ���� ó�� �Լ� ȣ��
            // �� �Լ��� logged_in ���¸� 0(�α׾ƿ�)�̳� -1(���α׷� ����)�� ������ �� ����
            handle_menu_choice(choice, page_width, db_conn, &logged_in);
            system("pause");
            system("cls");
        }

        // �ϱ��� �޴� �������� ������ �� (�α׾ƿ� �Ǵ� ���α׷� ����)
        close_db_connection(db_conn); // DB ���� �ݱ�
        // logged_in�� 0�̸� ���� ������ ó������ ���ư� �ٽ� DB ���� �õ� �� ���� �޴� ǥ��
        // logged_in�� -1�̸� ���� ������ ����
        if (logged_in == -1) {
            break; // ���� ���α׷� ���� ����
        }
    }

    return 0; // ���α׷� ���� ����
}