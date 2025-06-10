#include "a.h"
// 지정된 너비와 문자로 테두리를 그리는 함수
void draw_border(int width, char character) {
    for (int i = 0; i < width; i++) {
        printf("%c", character); // 지정된 문자를 width만큼 반복 출력
    }
    printf("\n"); // 줄 바꿈
}

// 일기 내용을 예쁜 박스 형태로 화면에 출력하는 함수
void print_diary_block(const char* title, const char* content, int width) {
    int content_len = strlen(content); // 일기 내용의 길이
    int title_len = strlen(title);     // 제목의 길이

    // 제목 중앙 정렬을 위한 양쪽 공백 계산
    // width - 4는 양쪽 '*' 문자와 그 옆의 공백을 제외한 실제 내용 영역
    int title_padding = (width - 4 - title_len) / 2;

    // 상단 빈 줄 출력
    printf("*");
    for (int i = 0; i < width - 2; i++) {
        printf(" ");
    }
    printf("*\n");

    // 제목 줄 출력 (중앙 정렬)
    printf("*");
    for (int i = 0; i < title_padding; i++) printf(" "); // 제목 앞 공백
    printf(" %s ", title); // 제목 출력
    // 제목 뒤 공백 (전체 너비에서 앞 공백, 제목, 양옆 한 칸 공백 제외)
    for (int i = 0; i < width - 4 - title_padding - title_len; i++) printf(" ");
    printf("*\n");

    // 제목 아래 빈 줄 출력
    printf("*");
    for (int i = 0; i < width - 2; i++) {
        printf(" ");
    }
    printf("*\n");

    // 내용 라인 출력 (줄 바꿈 처리 및 좌측 정렬)
    // 동적 메모리 할당: 내용 한 줄을 담을 버퍼 (width - 4는 내용 실제 영역)
    char* buffer = (char*)malloc(sizeof(char) * (width - 4 + 1));
    if (buffer == NULL) { // 메모리 할당 실패 시 오류 메시지 출력 후 종료
        perror("메모리 할당 실패");
        return;
    }

    int current_pos = 0; // 현재 읽고 있는 내용의 위치
    while (current_pos < content_len) {
        printf("* "); // 왼쪽 테두리 '*'와 공백
        int char_count = 0; // 버퍼에 담은 문자 수
        // 한 줄에 출력할 수 있는 최대 문자 수 (width - 4)만큼 내용을 버퍼에 담기
        for (int i = 0; i < width - 4 && current_pos < content_len; i++) {
            // 내용 중 줄 바꿈 문자('\n')를 만나면 강제로 줄 바꿈
            if (content[current_pos] == '\n') {
                buffer[char_count] = '\0'; // 버퍼 끝에 널 문자 추가
                current_pos++; // '\n' 문자 건너뛰기
                break; // 현재 줄 처리 완료
            }
            buffer[i] = content[current_pos]; // 문자를 버퍼에 복사
            current_pos++; // 다음 문자로 이동
            char_count++;  // 문자 수 증가
        }
        buffer[char_count] = '\0'; // 버퍼 끝에 널 문자 추가 (문자열 종료)

        // 버퍼 내용을 화면에 출력 (width - 4만큼 좌측 정렬)
        printf("%-*s", width - 4, buffer);
        printf(" *\n"); // 오른쪽 테두리 '*'와 줄 바꿈
    }

    free(buffer); // 동적으로 할당한 메모리 해제 (매우 중요!)

    // 하단 빈 줄 출력
    printf("*");
    for (int i = 0; i < width - 2; i++) {
        printf(" ");
    }
    printf("*\n");
}