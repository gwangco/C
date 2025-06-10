#include "a.h"

// MySQL 데이터베이스에 연결하고 'login' 테이블이 없으면 생성하는 함수
// 반환값: 성공 시 MySQL 연결 포인터 (MYSQL*), 실패 시 NULL
MYSQL* init_db_connection() {
    MYSQL* conn; // MySQL 연결을 관리할 구조체 포인터

    // MySQL 연결 객체 초기화
    conn = mysql_init(NULL);
    if (conn == NULL) { // 초기화 실패 시
        fprintf(stderr, "mysql_init 실패 !\n"); // 에러 메시지 출력
        return NULL; // NULL 반환하며 함수 종료
    }

    // MySQL 옵션 설정: 한글 처리를 위해 문자셋을 EUC-KR로 설정
    mysql_options(conn, MYSQL_SET_CHARSET_NAME, "euckr");
    mysql_options(conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");

    // MySQL 데이터베이스에 실제 연결 시도
    // (연결 객체, 호스트, 사용자, 비밀번호, DB이름, 포트, 소켓, 클라이언트 플래그)
    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, NULL, 0) == NULL) {
        fprintf(stderr, "MySQL 연결 실패 !: %s\n", mysql_error(conn)); // 연결 실패 시 에러 메시지
        mysql_close(conn); // 열려있던 연결 닫기
        return NULL; // NULL 반환하며 함수 종료
    }

    printf("MySQL 데이터베이스에 성공적으로 연결되었습니다.\n");

    // 'login' 테이블이 존재하지 않으면 생성하는 SQL 쿼리
    // id: VARCHAR(20)으로 기본 키 (중복 불가)
    // paswwd: VARCHAR(30)으로 NULL 허용 안함
    char* sql_create_table = "CREATE TABLE IF NOT EXISTS login (id VARCHAR(20) PRIMARY KEY, paswwd VARCHAR(30) NOT NULL);";
    // SQL 쿼리 실행
    if (mysql_query(conn, sql_create_table)) { // 쿼리 실행 실패 시
        fprintf(stderr, "Failed to create table: %s\n", mysql_error(conn)); // 에러 메시지
        mysql_close(conn); // 연결 닫기
        return NULL; // NULL 반환하며 함수 종료
    }
    printf("'login' 테이블 확인/생성 완료.\n");

    return conn; // 성공적으로 연결된 MySQL 포인터 반환
}

// 사용자 ID가 데이터베이스에 이미 존재하는지 확인하는 함수
// conn: MySQL 연결 포인터
// id: 확인할 사용자 ID 문자열
// 반환값: 1 (ID 존재), 0 (ID 없음), 1 (쿼리 실패 시 - 안전하게 존재한다고 처리)
int check_id_exists(MYSQL* conn, const char* id) {
    char query[200];      // SQL 쿼리 문자열을 저장할 버퍼
    MYSQL_RES* result;    // SQL 쿼리 결과를 저장할 구조체 포인터

    // 특정 ID를 조회하는 SQL 쿼리 생성
    sprintf(query, "SELECT id FROM login WHERE id = '%s'", id);
    // SQL 쿼리 실행
    if (mysql_query(conn, query)) { // 쿼리 실행 실패 시
        fprintf(stderr, "SELECT query failed: %s\n", mysql_error(conn));
        return 1; // 오류 시 중복으로 간주하여 회원가입 방지
    }

    // 쿼리 결과를 클라이언트(C 프로그램)로 가져옴
    result = mysql_store_result(conn);
    if (result == NULL) { // 결과 가져오기 실패 시
        fprintf(stderr, "mysql_store_result failed: %s\n", mysql_error(conn));
        return 1; // 오류 시 중복으로 간주
    }

    int exists = 0; // ID 존재 여부를 저장할 변수 (초기값: 없음)
    // 가져온 결과 행의 수가 0보다 크면 (즉, ID가 존재하면)
    if (mysql_num_rows(result) > 0) {
        exists = 1; // 존재한다고 표시
    }

    mysql_free_result(result); // 사용한 결과 셋 메모리 해제 (매우 중요!)
    return exists; // ID 존재 여부 반환
}

// 새로운 사용자를 데이터베이스에 등록(회원가입)하는 함수
// conn: MySQL 연결 포인터
// 반환값: 1 (회원가입 성공), 0 (회원가입 실패)
int register_user(MYSQL* conn) {
    char id[21];     // 사용자 ID를 저장할 버퍼 (VARCHAR(20) + 널문자)
    char paswwd[31]; // 비밀번호를 저장할 버퍼 (VARCHAR(30) + 널문자)
    char query[200]; // SQL 쿼리 문자열을 저장할 버퍼

    printf("\n--- 회원가입 ---\n");
    printf("사용할 ID를 입력하세요: ");
    // 사용자로부터 ID 입력 받기 (fgets는 개행 문자까지 읽어옴)
    if (fgets(id, sizeof(id), stdin) == NULL) return 0; // 입력 실패 시 0 반환
    CHOP(id); // 입력된 ID 문자열 끝의 개행 문자 제거

    // 입력받은 ID가 이미 존재하는지 확인
    if (check_id_exists(conn, id)) {
        printf("오류: '%s'는 이미 존재하는 ID입니다. 다른 ID를 사용해주세요.\n", id);
        return 0; // 회원가입 실패
    }

    printf("사용할 비밀번호를 입력하세요: ");
    // 사용자로부터 비밀번호 입력 받기
    if (fgets(paswwd, sizeof(paswwd), stdin) == NULL) return 0;
    CHOP(paswwd); // 입력된 비밀번호 문자열 끝의 개행 문자 제거

    // 사용자 정보를 'login' 테이블에 삽입하는 SQL 쿼리 생성
    sprintf(query, "INSERT INTO login(id, paswwd) VALUES ('%s', '%s')", id, paswwd);
    // SQL 쿼리 실행
    if (mysql_query(conn, query)) { // 쿼리 실행 실패 시
        fprintf(stderr, "INSERT query failed: %s\n", mysql_error(conn));
        return 0; // 실패
    }

    printf("회원가입이 성공적으로 완료되었습니다.\n");
    return 1; // 성공
}

// 사용자 로그인 시도 함수
// conn: MySQL 연결 포인터
// 반환값: 1 (로그인 성공), 0 (로그인 실패)
int login_user(MYSQL* conn) {
    char id[21];     // 사용자 ID를 저장할 버퍼
    char paswwd[31]; // 비밀번호를 저장할 버퍼
    char query[200]; // SQL 쿼리 문자열을 저장할 버퍼
    MYSQL_RES* result; // SQL 쿼리 결과를 저장할 구조체 포인터

    printf("\n--- 로그인 ---\n");
    printf("ID를 입력하세요: ");
    if (fgets(id, sizeof(id), stdin) == NULL) return 0;
    CHOP(id);

    printf("비밀번호를 입력하세요: ");
    if (fgets(paswwd, sizeof(paswwd), stdin) == NULL) return 0;
    CHOP(paswwd);

    // 입력받은 ID와 비밀번호가 일치하는지 확인하는 SQL 쿼리 생성
    sprintf(query, "SELECT id FROM login WHERE id = '%s' AND paswwd = '%s'", id, paswwd);
    // SQL 쿼리 실행
    if (mysql_query(conn, query)) { // 쿼리 실행 실패 시
        fprintf(stderr, "SELECT query failed: %s\n", mysql_error(conn));
        return 0; // 실패
    }

    // 쿼리 결과를 클라이언트로 가져옴
    result = mysql_store_result(conn);
    if (result == NULL) { // 결과 가져오기 실패 시
        fprintf(stderr, "mysql_store_result failed: %s\n", mysql_error(conn));
        return 0; // 실패
    }

    int success = 0; // 로그인 성공 여부를 저장할 변수 (초기값: 실패)
    // 결과 행의 수가 0보다 크면 (즉, 일치하는 ID/비밀번호 조합이 존재하면)
    if (mysql_num_rows(result) > 0) {
        printf("로그인 성공! 환영합니다, %s님!\n", id);
        success = 1; // 성공
    }
    else { // 일치하는 조합이 없으면
        printf("ID 또는 비밀번호가 잘못되었습니다.\n");
    }

    mysql_free_result(result); // 사용한 결과 셋 메모리 해제
    return success; // 로그인 성공 여부 반환
}

// 아이디/비밀번호 찾기 함수 (디버깅 목적으로 모든 사용자 정보 출력)
// conn: MySQL 연결 포인터
void find_id_password(MYSQL* conn) {
    MYSQL_RES* result; // SQL 쿼리 결과를 저장할 구조체 포인터
    MYSQL_ROW row;     // 결과의 각 행(row)을 저장할 구조체 포인터

    printf("\n--- 아이디/비밀번호 찾기 (모든 사용자 정보 출력) ---\n");
    printf("--- (주의: 보안상 실제 서비스에서는 사용되지 않는 방식입니다.) ---\n");

    // 'login' 테이블의 모든 ID와 비밀번호를 조회하는 SQL 쿼리 실행
    if (mysql_query(conn, "SELECT id, paswwd FROM login")) {
        fprintf(stderr, "조회 실패 !: %s\n", mysql_error(conn));
        return; // 쿼리 실패 시 함수 종료
    }

    // 쿼리 결과를 클라이언트로 가져옴
    result = mysql_store_result(conn);
    if (result == NULL) { // 결과 가져오기 실패 시
        fprintf(stderr, "결과 가져오기 실패 !: %s\n", mysql_error(conn));
        return; // 함수 종료
    }

    printf("\n   ID           PASSWORD\n"); // 출력 헤더
    printf("-------------------------------\n");
    // 모든 결과 행을 하나씩 가져와서 출력
    while ((row = mysql_fetch_row(result)) != NULL) {
        // row[0]은 첫 번째 컬럼 (ID), row[1]은 두 번째 컬럼 (비밀번호)
        // NULL 체크: 만약 DB에 값이 NULL로 저장되어 있다면 "NULL" 문자열 출력
        printf("%-15s %s\n", row[0] ? row[0] : "NULL", row[1] ? row[1] : "NULL");
    }
    printf("-------------------------------\n");

    mysql_free_result(result); // 사용한 결과 셋 메모리 해제 (필수!)
    printf("모든 사용자 정보 출력이 완료되었습니다.\n");
    
}

// MySQL 데이터베이스 연결을 해제하는 함수
// conn: 해제할 MySQL 연결 포인터
void close_db_connection(MYSQL* conn) {
    if (conn != NULL) { // 연결 포인터가 유효할 경우에만
        mysql_close(conn); // MySQL 연결 닫기
        printf("MySQL 연결이 해제되었습니다.\n");
    }
}