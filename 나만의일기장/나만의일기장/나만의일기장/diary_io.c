#include "a.h"

// Ư�� ��¥�� �̹� �ۼ��� �ϱ��� ���� ������ ã�� �Լ�
// ��: 20250529_diary_01.txt, 20250529_diary_02.txt �� �ִٸ� 3�� ��ȯ
int get_next_diary_sequence(int year, int month, int day) {
    char search_pattern[MAX_PATH]; // �˻��� ���� ��� ���� ����
    // 'diary/YYYYMMDD_diary_*.txt' ������ ���� ���� (��: diary/20250529_diary_*.txt)
    sprintf(search_pattern, "diary\\%04d%02d%02d_diary_*.txt", year, month, day);

    int max_sequence = 0; // ã�� ���� ū ������ ������ ����

    WIN32_FIND_DATAA findFileData; // ���� �˻� ����� ������ ����ü (Windows��)
    HANDLE hFind = INVALID_HANDLE_VALUE; // ���� �˻� �ڵ�

    // ������ �������� ù ��° ���� �˻�
    hFind = FindFirstFileA(search_pattern, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return 1; // �ش� ��¥�� �ϱⰡ ������, ù ��° ������ 1
    }

    // ������ �ϳ��� �ִٸ�, ���� ���ϵ��� ��� �˻�
    do {
        // ���� �׸��� ���丮�� �ƴ� �Ϲ� ������ ��츸 ó��
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char* filename = findFileData.cFileName; // ã�� ���� �̸�
            char* suffix_ptr = strstr(filename, "_diary_"); // ���ϸ��� "_diary_" ���ڿ� ã��
            if (suffix_ptr != NULL) {
                // "_diary_" �ٷ� �ڿ� ���� ����(����)�� ������ ��ȯ
                int current_seq = atoi(suffix_ptr + strlen("_diary_"));
                // ���� ������ �ִ� �������� ũ�� ����
                if (current_seq > max_sequence) {
                    max_sequence = current_seq;
                }
            }
        }
    } while (FindNextFileA(hFind, &findFileData) != 0); // ���� ������ ���� ������ �ݺ�

    FindClose(hFind); // ���� �˻� �ڵ� �ݱ� (�޸� ���� ����)

    return max_sequence + 1; // ã�� �ִ� ������ 1�� ���� ���� ���� ��ȯ
}

// ���ο� �ϱ⸦ �ۼ��ϰ� ���Ͽ� �����ϴ� �Լ�
void write_diary() {
    time_t t = time(NULL);       // ���� �ð� ��������
    struct tm* tm = localtime(&t); // ���� �ð� ����ü�� ��ȯ

    int year = tm->tm_year + 1900; // �⵵ (1900 ���ؾ� ���� �⵵)
    int month = tm->tm_mon + 1;    // �� (0���� �����ϹǷ� 1 ���ؾ� ��)
    int day = tm->tm_mday;         // ��

    // ���� ��¥�� ���� �ϱ� ���� �������� (��: 1, 2, 3...)
    int sequence = get_next_diary_sequence(year, month, day);

    char filename[MAX_PATH]; // ������ ���ϸ� ����
    // ���ϸ� ����: "diary/YYYYMMDD_diary_XX.txt" (��: diary/20250529_diary_01.txt)
    sprintf(filename, "diary\\%04d%02d%02d_diary_%02d.txt",
        year, month, day, sequence);

    FILE* fp = fopen(filename, "w"); // ���� ���� (���� ���, ���� ������ ����)
    if (fp == NULL) { // ���� ���� ���� ��
        printf("����: ������ �� �� �����ϴ�. (���: %s)\n", filename);
        printf("�ϱ��� ����(diary)�� �ִ��� Ȯ�����ּ���.\n");
        return;
    }

    printf("\n--- ���ο� �ϱ� �ۼ� ---\n");
    printf("���� ��¥: %04d�� %02d�� %02d�� (�ϱ� %d)\n", year, month, day, sequence);
    printf("�ϱ� ������ �Է��ϼ��� (�Է��� ��ġ���� �� �ٿ��� Ctrl+Z �� Enter):\n");
    printf("--------------------------\n");

    char buffer[1024]; // ����� �Է��� �� �پ� ���� �ӽ� ����
    // EOF(End Of File) ���� (Ctrl+Z)�� �Էµ� ������ ���Ͽ� ���� ����
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        fputs(buffer, fp); // ���� ������ ���Ͽ� ����
    }

    fclose(fp); // ���� �ݱ� 
    printf("\n�ϱⰡ '%s' ���Ϸ� ����Ǿ����ϴ�.\n", filename);
    getchar(); // �Է� ���� ���� Ctrl+Z �Է� �� ���� ���� ���� ó��)
}

// Ư�� ��¥/������ �ϱ� ������ �о�� ȭ�鿡 ǥ���ϴ� �Լ�
void view_diary(int page_width) {
    char search_date_str[9]; // �˻��� ��¥ ���ڿ� (YYYYMMDD)
    int search_sequence;     // �˻��� �ϱ� ����
    char filename[MAX_PATH]; // �˻��� ���ϸ� ����
    char line_buffer[1024];  // ���Ͽ��� �� �پ� ���� �ӽ� ����
    char content_buffer[10000]; // �о�� �ϱ� ���� ��ü�� ������ ���� (�˳��ϰ�)
    content_buffer[0] = '\0'; // ���� �ʱ�ȭ (�� ���ڿ���)

    printf("\n--- �ϱ� ���� ---\n");
    printf("���� ���� �ϱ� ��¥�� ������ �Է��ϼ��� (��: 20250527 1): ");
    scanf("%8s %d", search_date_str, &search_sequence); // ��¥�� ������ �Բ� �Է¹���
    getchar(); // �Է� ���� ����

    // �˻��� ���ϸ� ���� (��: diary/20250527_diary_01.txt)
    sprintf(filename, "diary\\%s_diary_%02d.txt", search_date_str, search_sequence);

    FILE* fp = fopen(filename, "r"); // ���� ���� (�б� ���)
    if (fp == NULL) { // ���� ���� ���� (������ ���ų� ���� �� ����)
        printf("����: '%s' �ϱ⸦ ã�� �� �����ϴ�.\n", filename);
        printf("��¥�� ���� ������ Ȯ���ϰų� '�ϱ� ��� ����'�� ���� Ȯ�����ּ���.\n");
        return;
    }

    printf("\n");
    draw_border(page_width, '*'); // ��� '*' �׵θ� �׸���
    printf("********************************************************************************\n"); // �߰� ������ ����
    printf("##");
    char display_title[50]; // ȭ�鿡 ǥ���� ���� ����
    // ���� ����: "YYYYMMDD �ϱ� X" (��: 20250527 �ϱ� 1)
    sprintf(display_title, "%s �ϱ� %d", search_date_str, search_sequence);
    int title_len = strlen(display_title);
    int title_padding = (page_width - 4 - title_len) / 2; // ���� �߾� ������ ���� ���� ���
    for (int i = 0; i < title_padding; i++) printf(" ");
    printf(" %s ", display_title); // ���� ���
    for (int i = 0; i < page_width - 4 - title_padding - title_len; i++) printf(" ");
    printf("##\n");
    printf("********************************************************************************\n"); // �߰� ������ ����
    draw_border(page_width, '*'); // �� �ٸ� ��� '*' �׵θ� �׸���

    // ���� ������ �� �پ� �о� content_buffer�� ��� ����
    while (fgets(line_buffer, sizeof(line_buffer), fp) != NULL) {
        strcat(content_buffer, line_buffer); // ���� ���� ��ü ���� ���ۿ� �߰�
    }

    fclose(fp); // ���� �ݱ�

    // ����� ��ü �ϱ� ������ ȭ�鿡 ���ڰ� ���
    print_diary_block(display_title, content_buffer, page_width);

    printf("\n");
    draw_border(page_width, '*'); // �ϴ� '*' �׵θ� �׸���
    printf("\n�ϱ� ���Ⱑ �Ϸ�Ǿ����ϴ�.\n");
}