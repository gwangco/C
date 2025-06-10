#include "a.h"

// 특정 날짜에 이미 작성된 일기의 다음 순번을 찾는 함수
// 예: 20250529_diary_01.txt, 20250529_diary_02.txt 가 있다면 3을 반환
int get_next_diary_sequence(int year, int month, int day) {
    char search_pattern[MAX_PATH]; // 검색할 파일 경로 패턴 버퍼
    // 'diary/YYYYMMDD_diary_*.txt' 형태의 패턴 생성 (예: diary/20250529_diary_*.txt)
    sprintf(search_pattern, "diary\\%04d%02d%02d_diary_*.txt", year, month, day);

    int max_sequence = 0; // 찾은 가장 큰 순번을 저장할 변수

    WIN32_FIND_DATAA findFileData; // 파일 검색 결과를 저장할 구조체 (Windows용)
    HANDLE hFind = INVALID_HANDLE_VALUE; // 파일 검색 핸들

    // 지정된 패턴으로 첫 번째 파일 검색
    hFind = FindFirstFileA(search_pattern, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return 1; // 해당 날짜의 일기가 없으면, 첫 번째 순번은 1
    }

    // 파일이 하나라도 있다면, 다음 파일들을 계속 검색
    do {
        // 현재 항목이 디렉토리가 아닌 일반 파일일 경우만 처리
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char* filename = findFileData.cFileName; // 찾은 파일 이름
            char* suffix_ptr = strstr(filename, "_diary_"); // 파일명에서 "_diary_" 문자열 찾기
            if (suffix_ptr != NULL) {
                // "_diary_" 바로 뒤에 오는 숫자(순번)를 정수로 변환
                int current_seq = atoi(suffix_ptr + strlen("_diary_"));
                // 현재 순번이 최대 순번보다 크면 갱신
                if (current_seq > max_sequence) {
                    max_sequence = current_seq;
                }
            }
        }
    } while (FindNextFileA(hFind, &findFileData) != 0); // 다음 파일이 없을 때까지 반복

    FindClose(hFind); // 파일 검색 핸들 닫기 (메모리 누수 방지)

    return max_sequence + 1; // 찾은 최대 순번에 1을 더해 다음 순번 반환
}

// 새로운 일기를 작성하고 파일에 저장하는 함수
void write_diary() {
    time_t t = time(NULL);       // 현재 시간 가져오기
    struct tm* tm = localtime(&t); // 현지 시간 구조체로 변환

    int year = tm->tm_year + 1900; // 년도 (1900 더해야 실제 년도)
    int month = tm->tm_mon + 1;    // 월 (0부터 시작하므로 1 더해야 함)
    int day = tm->tm_mday;         // 일

    // 현재 날짜의 다음 일기 순번 가져오기 (예: 1, 2, 3...)
    int sequence = get_next_diary_sequence(year, month, day);

    char filename[MAX_PATH]; // 저장할 파일명 버퍼
    // 파일명 형식: "diary/YYYYMMDD_diary_XX.txt" (예: diary/20250529_diary_01.txt)
    sprintf(filename, "diary\\%04d%02d%02d_diary_%02d.txt",
        year, month, day, sequence);

    FILE* fp = fopen(filename, "w"); // 파일 열기 (쓰기 모드, 파일 없으면 생성)
    if (fp == NULL) { // 파일 열기 실패 시
        printf("오류: 파일을 열 수 없습니다. (경로: %s)\n", filename);
        printf("일기장 폴더(diary)가 있는지 확인해주세요.\n");
        return;
    }

    printf("\n--- 새로운 일기 작성 ---\n");
    printf("오늘 날짜: %04d년 %02d월 %02d일 (일기 %d)\n", year, month, day, sequence);
    printf("일기 내용을 입력하세요 (입력을 마치려면 새 줄에서 Ctrl+Z 후 Enter):\n");
    printf("--------------------------\n");

    char buffer[1024]; // 사용자 입력을 한 줄씩 읽을 임시 버퍼
    // EOF(End Of File) 문자 (Ctrl+Z)가 입력될 때까지 파일에 내용 쓰기
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        fputs(buffer, fp); // 읽은 내용을 파일에 쓰기
    }

    fclose(fp); // 파일 닫기 
    printf("\n일기가 '%s' 파일로 저장되었습니다.\n", filename);
    getchar(); // 입력 버퍼 비우기 Ctrl+Z 입력 후 남은 개행 문자 처리)
}

// 특정 날짜/순번의 일기 내용을 읽어와 화면에 표시하는 함수
void view_diary(int page_width) {
    char search_date_str[9]; // 검색할 날짜 문자열 (YYYYMMDD)
    int search_sequence;     // 검색할 일기 순번
    char filename[MAX_PATH]; // 검색할 파일명 버퍼
    char line_buffer[1024];  // 파일에서 한 줄씩 읽을 임시 버퍼
    char content_buffer[10000]; // 읽어온 일기 내용 전체를 저장할 버퍼 (넉넉하게)
    content_buffer[0] = '\0'; // 버퍼 초기화 (빈 문자열로)

    printf("\n--- 일기 보기 ---\n");
    printf("보고 싶은 일기 날짜와 순번을 입력하세요 (예: 20250527 1): ");
    scanf("%8s %d", search_date_str, &search_sequence); // 날짜와 순번을 함께 입력받음
    getchar(); // 입력 버퍼 비우기

    // 검색할 파일명 생성 (예: diary/20250527_diary_01.txt)
    sprintf(filename, "diary\\%s_diary_%02d.txt", search_date_str, search_sequence);

    FILE* fp = fopen(filename, "r"); // 파일 열기 (읽기 모드)
    if (fp == NULL) { // 파일 열기 실패 (파일이 없거나 읽을 수 없음)
        printf("오류: '%s' 일기를 찾을 수 없습니다.\n", filename);
        printf("날짜와 순번 형식을 확인하거나 '일기 목록 보기'를 통해 확인해주세요.\n");
        return;
    }

    printf("\n");
    draw_border(page_width, '*'); // 상단 '*' 테두리 그리기
    printf("********************************************************************************\n"); // 추가 디자인 라인
    printf("##");
    char display_title[50]; // 화면에 표시할 제목 버퍼
    // 제목 생성: "YYYYMMDD 일기 X" (예: 20250527 일기 1)
    sprintf(display_title, "%s 일기 %d", search_date_str, search_sequence);
    int title_len = strlen(display_title);
    int title_padding = (page_width - 4 - title_len) / 2; // 제목 중앙 정렬을 위한 공백 계산
    for (int i = 0; i < title_padding; i++) printf(" ");
    printf(" %s ", display_title); // 제목 출력
    for (int i = 0; i < page_width - 4 - title_padding - title_len; i++) printf(" ");
    printf("##\n");
    printf("********************************************************************************\n"); // 추가 디자인 라인
    draw_border(page_width, '*'); // 또 다른 상단 '*' 테두리 그리기

    // 파일 내용을 한 줄씩 읽어 content_buffer에 모두 저장
    while (fgets(line_buffer, sizeof(line_buffer), fp) != NULL) {
        strcat(content_buffer, line_buffer); // 읽은 줄을 전체 내용 버퍼에 추가
    }

    fclose(fp); // 파일 닫기

    // 저장된 전체 일기 내용을 화면에 예쁘게 출력
    print_diary_block(display_title, content_buffer, page_width);

    printf("\n");
    draw_border(page_width, '*'); // 하단 '*' 테두리 그리기
    printf("\n일기 보기가 완료되었습니다.\n");
}