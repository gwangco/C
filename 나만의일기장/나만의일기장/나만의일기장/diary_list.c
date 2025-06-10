#include "a.h"

// ����� �ϱ� ���ϵ��� ����� �о�� ȭ�鿡 ǥ���ϴ� �Լ�
void list_diaries() {
    printf("\n--- ����� �ϱ� ��� ---\n");

    WIN32_FIND_DATAA findFileData; // ���� �˻� ����� ������ ����ü
    HANDLE hFind = INVALID_HANDLE_VALUE; // ���� �˻� �ڵ�
    char searchPath[MAX_PATH]; // �˻��� ��� ���� (Windows�� �ִ� ��� ����)

    // 'diary' ���� ���� ��� '.txt' ������ �˻��� ���� ����
    sprintf(searchPath, "diary\\*.txt");

    // ������ �������� ù ��° ���� �˻� ����
    hFind = FindFirstFileA(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) { // ���� �˻� ���� (���� ���ų� ���� ����)
        printf("����: 'diary' ������ ã�� �� ���ų� ������ �����ϴ�.\n");
        printf("------------------------\n");
        return;
    }

    int count = 0; // ã�� �ϱ� ������ ����
    do {
        // ���� �׸��� ���丮�� �ƴ� �Ϲ� ������ ��츸 ó��
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char* filename = findFileData.cFileName; // ã�� ���� �̸�
            // ���ϸ��� "_diary_"�� �����ϰ� ".txt"�� �������� Ȯ��
            if (strstr(filename, "_diary_") != NULL && strstr(filename, ".txt") != NULL) {
                char date_str[9]; // ��¥ �κ� (YYYYMMDD)�� ������ ����
                int sequence;     // ������ ������ ����
                // ���ϸ��� ��¥�� ������ �Ľ� (��: "20250527_diary_01.txt" -> "20250527", 1)
                sscanf(filename, "%8s_diary_%d.txt", date_str, &sequence);

                // ȭ�鿡 "��¥ - �ϱ� ����" �������� ���
                printf("%s - �ϱ� %d\n", date_str, sequence);
                count++;
            }
        }
    } while (FindNextFileA(hFind, &findFileData) != 0); // ���� ������ ���� ������ �ݺ�

    FindClose(hFind); // ���� �˻� �ڵ� �ݱ�

    if (count == 0) { // ã�� �ϱⰡ ������
        printf("����� �ϱⰡ �����ϴ�.\n");
    }
    printf("------------------------\n");
}