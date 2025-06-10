#include "a.h"

// 저장된 일기 파일들의 목록을 읽어와 화면에 표시하는 함수
void list_diaries() {
    printf("\n--- 저장된 일기 목록 ---\n");

    WIN32_FIND_DATAA findFileData; // 파일 검색 결과를 저장할 구조체
    HANDLE hFind = INVALID_HANDLE_VALUE; // 파일 검색 핸들
    char searchPath[MAX_PATH]; // 검색할 경로 버퍼 (Windows용 최대 경로 길이)

    // 'diary' 폴더 안의 모든 '.txt' 파일을 검색할 패턴 생성
    sprintf(searchPath, "diary\\*.txt");

    // 지정된 패턴으로 첫 번째 파일 검색 시작
    hFind = FindFirstFileA(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) { // 파일 검색 실패 (폴더 없거나 파일 없음)
        printf("오류: 'diary' 폴더를 찾을 수 없거나 파일이 없습니다.\n");
        printf("------------------------\n");
        return;
    }

    int count = 0; // 찾은 일기 파일의 개수
    do {
        // 현재 항목이 디렉토리가 아닌 일반 파일일 경우만 처리
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char* filename = findFileData.cFileName; // 찾은 파일 이름
            // 파일명이 "_diary_"를 포함하고 ".txt"로 끝나는지 확인
            if (strstr(filename, "_diary_") != NULL && strstr(filename, ".txt") != NULL) {
                char date_str[9]; // 날짜 부분 (YYYYMMDD)을 저장할 버퍼
                int sequence;     // 순번을 저장할 변수
                // 파일명에서 날짜와 순번을 파싱 (예: "20250527_diary_01.txt" -> "20250527", 1)
                sscanf(filename, "%8s_diary_%d.txt", date_str, &sequence);

                // 화면에 "날짜 - 일기 순번" 형식으로 출력
                printf("%s - 일기 %d\n", date_str, sequence);
                count++;
            }
        }
    } while (FindNextFileA(hFind, &findFileData) != 0); // 다음 파일이 없을 때까지 반복

    FindClose(hFind); // 파일 검색 핸들 닫기

    if (count == 0) { // 찾은 일기가 없으면
        printf("저장된 일기가 없습니다.\n");
    }
    printf("------------------------\n");
}