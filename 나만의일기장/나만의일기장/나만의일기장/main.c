#include "a.h" // 통합 헤더 파일 포함

int main() {
#ifdef _WIN32
    system("mkdir diary"); // 'diary' 폴더 생성 시도 (Windows 전용)
#endif

    MYSQL* db_conn = NULL; // MySQL 연결 포인터
    int page_width = 80;  // 화면 너비

    // 메인 프로그램 루프
    while (1) {
        // 데이터베이스 연결
        db_conn = init_db_connection();
        if (db_conn == NULL) {
            fprintf(stderr, "데이터베이스 연결에 실패했습니다. 프로그램을 종료합니다.\n");
            return 1;
        }

        int logged_in = 0; // 로그인 상태 (0:로그아웃, 1:로그인, -1:프로그램 종료)

        // 로그인/회원가입/찾기 루프
        while (!logged_in) {
            display_auth_menu(); // 인증 메뉴 출력
            int auth_choice;
            scanf("%d", &auth_choice);
            getchar(); // 입력 버퍼 비우기

            // 인증 메뉴 선택 처리 함수 호출
            // 이 함수는 logged_in 상태를 변경하고, 프로그램 종료 여부(-1)를 반환할 수 있음
            int auth_result = handle_auth_choice(auth_choice, db_conn, &logged_in);

            if (auth_result == -1) { // 프로그램 종료 선택 시
                logged_in = -1; // 메인 루프도 종료하도록 logged_in 상태 변경
                break; // 인증 루프 종료
            }
            system("pause");
            system("cls");
            // auth_result가 1이면 logged_in이 1로 설정되어 인증 루프 종료 (로그인 성공)
            // auth_result가 0이면 로그인 실패/회원가입 후이므로 인증 루프 계속
        }

        // 인증 루프에서 나왔을 때 (로그인 성공 또는 프로그램 종료)
        if (logged_in == -1) { // 프로그램 종료가 선택되었다면
            close_db_connection(db_conn); // DB 연결 닫기
            break; // 메인 프로그램 루프 종료
        }

        // --- 로그인 성공 후 일기장 메인 메뉴 ---
        int choice;
        while (logged_in == 1) { // 로그인 상태가 유지되는 동안
            display_main_menu(page_width); // 메인 일기장 메뉴 출력
            printf("선택: ");
            scanf("%d", &choice);
            getchar(); // 입력 버퍼 비우기

            // 일기장 메뉴 선택 처리 함수 호출
            // 이 함수는 logged_in 상태를 0(로그아웃)이나 -1(프로그램 종료)로 변경할 수 있음
            handle_menu_choice(choice, page_width, db_conn, &logged_in);
            system("pause");
            system("cls");
        }

        // 일기장 메뉴 루프에서 나왔을 때 (로그아웃 또는 프로그램 종료)
        close_db_connection(db_conn); // DB 연결 닫기
        // logged_in이 0이면 메인 루프의 처음으로 돌아가 다시 DB 연결 시도 및 인증 메뉴 표시
        // logged_in이 -1이면 메인 루프도 종료
        if (logged_in == -1) {
            break; // 메인 프로그램 루프 종료
        }
    }

    return 0; // 프로그램 정상 종료
}