#include "a.h"

// 메인 메뉴를 화면에 표시하는 함수
void display_main_menu(int width) {
    draw_border(width, '=');
    printf("##");
    int menu_padding = (width - 4 - strlen(" 나만의 일기장 메뉴 ")) / 2;
    for (int i = 0; i < menu_padding; i++) printf(" ");
    printf(" 나만의 일기장 메뉴 ");
    for (int i = 0; i < width - 4 - menu_padding - strlen(" 나만의 일기장 메뉴 "); i++) printf(" ");
    printf("##\n");
    draw_border(width, '=');

    printf("1. 새 일기 작성\n");
    printf("2. 일기 목록 보기\n");
    printf("3. 일기 보기\n");
    printf("4. 로그아웃\n");
    printf("5. 프로그램 종료\n");
}

// 사용자의 메인 메뉴 선택에 따라 각 기능 함수를 호출하는 함수
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
    case 4: // 로그아웃
        printf("\n로그아웃되었습니다. 다시 로그인/회원가입 화면으로 돌아갑니다.\n");
        *logged_in_status = 0; // 로그인 상태를 0(로그아웃)으로 변경
        break;
    case 5: // 프로그램 종료
        printf("\n일기장을 종료합니다. 안녕히 계세요!\n");
        *logged_in_status = -1; // 프로그램 종료를 의미하는 특수 값
        break;
    default:
        printf("잘못된 선택입니다. 다시 입력해주세요.\n");
        break;
    }
}

// 인증 메뉴(로그인/회원가입/찾기)를 화면에 표시하는 함수
void display_auth_menu() {
    printf("\n************************************\n");
    printf("* 나만의 일기장 - 인증         *\n");
    printf("************************************\n");
    printf("1. 로그인\n");
    printf("2. 회원가입\n");
    printf("3. 아이디/비밀번호 찾기\n"); 
    printf("4. 프로그램 종료\n"); 
    printf("선택: ");
}

// 사용자의 인증 메뉴 선택에 따라 각 기능 함수를 호출하는 함수
int handle_auth_choice(int choice, MYSQL* db_conn, int* logged_in_status) {
    int result = 0; // 기본적으로 로그인 실패 (0)

    switch (choice) {
    case 1: // 로그인
        result = login_user(db_conn); // 로그인 시도
        *logged_in_status = result; // 로그인 성공 여부 업데이트
        break;
    case 2: // 회원가입
        register_user(db_conn); // 회원가입 시도
        // 회원가입 성공해도 바로 로그인된 것은 아니므로 logged_in_status는 변경하지 않음
        break;
    case 3: // 아이디/비밀번호 찾기
        find_id_password(db_conn); // 아이디/비밀번호 찾기 함수 호출
        break;
    case 4: // 프로그램 종료
        printf("프로그램을 종료합니다.\n");
        result = -1; // 프로그램 종료를 의미
        *logged_in_status = -1; // main 루프가 종료되도록 상태 변경
        break;
    default:
        printf("잘못된 선택입니다. 다시 입력해주세요.\n");
        break;
    }
    return result; // 로그인 성공(1), 실패(0), 프로그램 종료(-1)
}