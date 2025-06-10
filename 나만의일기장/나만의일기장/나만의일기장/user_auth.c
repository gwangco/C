#include "a.h"

// MySQL �����ͺ��̽��� �����ϰ� 'login' ���̺��� ������ �����ϴ� �Լ�
// ��ȯ��: ���� �� MySQL ���� ������ (MYSQL*), ���� �� NULL
MYSQL* init_db_connection() {
    MYSQL* conn; // MySQL ������ ������ ����ü ������

    // MySQL ���� ��ü �ʱ�ȭ
    conn = mysql_init(NULL);
    if (conn == NULL) { // �ʱ�ȭ ���� ��
        fprintf(stderr, "mysql_init ���� !\n"); // ���� �޽��� ���
        return NULL; // NULL ��ȯ�ϸ� �Լ� ����
    }

    // MySQL �ɼ� ����: �ѱ� ó���� ���� ���ڼ��� EUC-KR�� ����
    mysql_options(conn, MYSQL_SET_CHARSET_NAME, "euckr");
    mysql_options(conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");

    // MySQL �����ͺ��̽��� ���� ���� �õ�
    // (���� ��ü, ȣ��Ʈ, �����, ��й�ȣ, DB�̸�, ��Ʈ, ����, Ŭ���̾�Ʈ �÷���)
    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, NULL, 0) == NULL) {
        fprintf(stderr, "MySQL ���� ���� !: %s\n", mysql_error(conn)); // ���� ���� �� ���� �޽���
        mysql_close(conn); // �����ִ� ���� �ݱ�
        return NULL; // NULL ��ȯ�ϸ� �Լ� ����
    }

    printf("MySQL �����ͺ��̽��� ���������� ����Ǿ����ϴ�.\n");

    // 'login' ���̺��� �������� ������ �����ϴ� SQL ����
    // id: VARCHAR(20)���� �⺻ Ű (�ߺ� �Ұ�)
    // paswwd: VARCHAR(30)���� NULL ��� ����
    char* sql_create_table = "CREATE TABLE IF NOT EXISTS login (id VARCHAR(20) PRIMARY KEY, paswwd VARCHAR(30) NOT NULL);";
    // SQL ���� ����
    if (mysql_query(conn, sql_create_table)) { // ���� ���� ���� ��
        fprintf(stderr, "Failed to create table: %s\n", mysql_error(conn)); // ���� �޽���
        mysql_close(conn); // ���� �ݱ�
        return NULL; // NULL ��ȯ�ϸ� �Լ� ����
    }
    printf("'login' ���̺� Ȯ��/���� �Ϸ�.\n");

    return conn; // ���������� ����� MySQL ������ ��ȯ
}

// ����� ID�� �����ͺ��̽��� �̹� �����ϴ��� Ȯ���ϴ� �Լ�
// conn: MySQL ���� ������
// id: Ȯ���� ����� ID ���ڿ�
// ��ȯ��: 1 (ID ����), 0 (ID ����), 1 (���� ���� �� - �����ϰ� �����Ѵٰ� ó��)
int check_id_exists(MYSQL* conn, const char* id) {
    char query[200];      // SQL ���� ���ڿ��� ������ ����
    MYSQL_RES* result;    // SQL ���� ����� ������ ����ü ������

    // Ư�� ID�� ��ȸ�ϴ� SQL ���� ����
    sprintf(query, "SELECT id FROM login WHERE id = '%s'", id);
    // SQL ���� ����
    if (mysql_query(conn, query)) { // ���� ���� ���� ��
        fprintf(stderr, "SELECT query failed: %s\n", mysql_error(conn));
        return 1; // ���� �� �ߺ����� �����Ͽ� ȸ������ ����
    }

    // ���� ����� Ŭ���̾�Ʈ(C ���α׷�)�� ������
    result = mysql_store_result(conn);
    if (result == NULL) { // ��� �������� ���� ��
        fprintf(stderr, "mysql_store_result failed: %s\n", mysql_error(conn));
        return 1; // ���� �� �ߺ����� ����
    }

    int exists = 0; // ID ���� ���θ� ������ ���� (�ʱⰪ: ����)
    // ������ ��� ���� ���� 0���� ũ�� (��, ID�� �����ϸ�)
    if (mysql_num_rows(result) > 0) {
        exists = 1; // �����Ѵٰ� ǥ��
    }

    mysql_free_result(result); // ����� ��� �� �޸� ���� (�ſ� �߿�!)
    return exists; // ID ���� ���� ��ȯ
}

// ���ο� ����ڸ� �����ͺ��̽��� ���(ȸ������)�ϴ� �Լ�
// conn: MySQL ���� ������
// ��ȯ��: 1 (ȸ������ ����), 0 (ȸ������ ����)
int register_user(MYSQL* conn) {
    char id[21];     // ����� ID�� ������ ���� (VARCHAR(20) + �ι���)
    char paswwd[31]; // ��й�ȣ�� ������ ���� (VARCHAR(30) + �ι���)
    char query[200]; // SQL ���� ���ڿ��� ������ ����

    printf("\n--- ȸ������ ---\n");
    printf("����� ID�� �Է��ϼ���: ");
    // ����ڷκ��� ID �Է� �ޱ� (fgets�� ���� ���ڱ��� �о��)
    if (fgets(id, sizeof(id), stdin) == NULL) return 0; // �Է� ���� �� 0 ��ȯ
    CHOP(id); // �Էµ� ID ���ڿ� ���� ���� ���� ����

    // �Է¹��� ID�� �̹� �����ϴ��� Ȯ��
    if (check_id_exists(conn, id)) {
        printf("����: '%s'�� �̹� �����ϴ� ID�Դϴ�. �ٸ� ID�� ������ּ���.\n", id);
        return 0; // ȸ������ ����
    }

    printf("����� ��й�ȣ�� �Է��ϼ���: ");
    // ����ڷκ��� ��й�ȣ �Է� �ޱ�
    if (fgets(paswwd, sizeof(paswwd), stdin) == NULL) return 0;
    CHOP(paswwd); // �Էµ� ��й�ȣ ���ڿ� ���� ���� ���� ����

    // ����� ������ 'login' ���̺� �����ϴ� SQL ���� ����
    sprintf(query, "INSERT INTO login(id, paswwd) VALUES ('%s', '%s')", id, paswwd);
    // SQL ���� ����
    if (mysql_query(conn, query)) { // ���� ���� ���� ��
        fprintf(stderr, "INSERT query failed: %s\n", mysql_error(conn));
        return 0; // ����
    }

    printf("ȸ�������� ���������� �Ϸ�Ǿ����ϴ�.\n");
    return 1; // ����
}

// ����� �α��� �õ� �Լ�
// conn: MySQL ���� ������
// ��ȯ��: 1 (�α��� ����), 0 (�α��� ����)
int login_user(MYSQL* conn) {
    char id[21];     // ����� ID�� ������ ����
    char paswwd[31]; // ��й�ȣ�� ������ ����
    char query[200]; // SQL ���� ���ڿ��� ������ ����
    MYSQL_RES* result; // SQL ���� ����� ������ ����ü ������

    printf("\n--- �α��� ---\n");
    printf("ID�� �Է��ϼ���: ");
    if (fgets(id, sizeof(id), stdin) == NULL) return 0;
    CHOP(id);

    printf("��й�ȣ�� �Է��ϼ���: ");
    if (fgets(paswwd, sizeof(paswwd), stdin) == NULL) return 0;
    CHOP(paswwd);

    // �Է¹��� ID�� ��й�ȣ�� ��ġ�ϴ��� Ȯ���ϴ� SQL ���� ����
    sprintf(query, "SELECT id FROM login WHERE id = '%s' AND paswwd = '%s'", id, paswwd);
    // SQL ���� ����
    if (mysql_query(conn, query)) { // ���� ���� ���� ��
        fprintf(stderr, "SELECT query failed: %s\n", mysql_error(conn));
        return 0; // ����
    }

    // ���� ����� Ŭ���̾�Ʈ�� ������
    result = mysql_store_result(conn);
    if (result == NULL) { // ��� �������� ���� ��
        fprintf(stderr, "mysql_store_result failed: %s\n", mysql_error(conn));
        return 0; // ����
    }

    int success = 0; // �α��� ���� ���θ� ������ ���� (�ʱⰪ: ����)
    // ��� ���� ���� 0���� ũ�� (��, ��ġ�ϴ� ID/��й�ȣ ������ �����ϸ�)
    if (mysql_num_rows(result) > 0) {
        printf("�α��� ����! ȯ���մϴ�, %s��!\n", id);
        success = 1; // ����
    }
    else { // ��ġ�ϴ� ������ ������
        printf("ID �Ǵ� ��й�ȣ�� �߸��Ǿ����ϴ�.\n");
    }

    mysql_free_result(result); // ����� ��� �� �޸� ����
    return success; // �α��� ���� ���� ��ȯ
}

// ���̵�/��й�ȣ ã�� �Լ� (����� �������� ��� ����� ���� ���)
// conn: MySQL ���� ������
void find_id_password(MYSQL* conn) {
    MYSQL_RES* result; // SQL ���� ����� ������ ����ü ������
    MYSQL_ROW row;     // ����� �� ��(row)�� ������ ����ü ������

    printf("\n--- ���̵�/��й�ȣ ã�� (��� ����� ���� ���) ---\n");
    printf("--- (����: ���Ȼ� ���� ���񽺿����� ������ �ʴ� ����Դϴ�.) ---\n");

    // 'login' ���̺��� ��� ID�� ��й�ȣ�� ��ȸ�ϴ� SQL ���� ����
    if (mysql_query(conn, "SELECT id, paswwd FROM login")) {
        fprintf(stderr, "��ȸ ���� !: %s\n", mysql_error(conn));
        return; // ���� ���� �� �Լ� ����
    }

    // ���� ����� Ŭ���̾�Ʈ�� ������
    result = mysql_store_result(conn);
    if (result == NULL) { // ��� �������� ���� ��
        fprintf(stderr, "��� �������� ���� !: %s\n", mysql_error(conn));
        return; // �Լ� ����
    }

    printf("\n   ID           PASSWORD\n"); // ��� ���
    printf("-------------------------------\n");
    // ��� ��� ���� �ϳ��� �����ͼ� ���
    while ((row = mysql_fetch_row(result)) != NULL) {
        // row[0]�� ù ��° �÷� (ID), row[1]�� �� ��° �÷� (��й�ȣ)
        // NULL üũ: ���� DB�� ���� NULL�� ����Ǿ� �ִٸ� "NULL" ���ڿ� ���
        printf("%-15s %s\n", row[0] ? row[0] : "NULL", row[1] ? row[1] : "NULL");
    }
    printf("-------------------------------\n");

    mysql_free_result(result); // ����� ��� �� �޸� ���� (�ʼ�!)
    printf("��� ����� ���� ����� �Ϸ�Ǿ����ϴ�.\n");
    
}

// MySQL �����ͺ��̽� ������ �����ϴ� �Լ�
// conn: ������ MySQL ���� ������
void close_db_connection(MYSQL* conn) {
    if (conn != NULL) { // ���� �����Ͱ� ��ȿ�� ��쿡��
        mysql_close(conn); // MySQL ���� �ݱ�
        printf("MySQL ������ �����Ǿ����ϴ�.\n");
    }
}