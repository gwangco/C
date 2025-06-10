#pragma once
// --- 공통적으로 필요한 표준 라이브러리 ---
#include <stdio.h>    // 표준 입출력 (printf, scanf, fopen 등)
#include <stdlib.h>   // 일반 유틸리티 (system, malloc, free, exit, atoi 등)
#include <string.h>   // 문자열 함수 (strlen, strcpy, strcat, strstr, strcmp, sscanf 등)
#include <time.h>     // 시간/날짜 정보 (time, localtime)

// Windows 운영체제 전용 API
#include <windows.h>  // Windows API (FindFirstFile, FindNextFile 등)
#define MAX_PATH 260  // Windows 기본 경로 길이

// --- MySQL 데이터베이스 관련 ---
#include <mysql.h>    // MySQL 데이터베이스 관련 함수 및 구조체

// MySQL 데이터베이스 연결 정보를 정의
#define DB_HOST  // MySQL 서버 IP 주소 (여기서는 로컬 PC)
#define DB_USER       // MySQL 접속 사용자 이름
#define DB_PASS     // MySQL 접속 비밀번호 (본인 MySQL 비밀번호로 변경 필수!)
#define DB_NAME      // 사용할 데이터베이스 이름 (미리 생성되어 있어야 함)
#define DB_PORT        // MySQL 서버 포트 (기본은 3306, 여기서는 3307로 설정)

// 문자열 끝의 개행 문자('\n')를 제거하는 매크로 함수
#define CHOP(x) x[strlen(x) - 1] = '\0'

// --- 함수 선언 목록 (나만의 일기장 프로그램의 모든 함수) ---

// diary_display 관련 함수들
void draw_border(int width, char character);
void print_diary_block(const char* title, const char* content, int width);

// diary_menu 관련 함수들
void display_main_menu(int width);
void handle_menu_choice(int choice, int page_width, MYSQL* db_conn, int* logged_in_status);
void display_auth_menu();
int handle_auth_choice(int choice, MYSQL* db_conn, int* logged_in_status);

// diary_io 관련 함수들
int get_next_diary_sequence(int year, int month, int day);
void write_diary();
void view_diary(int page_width);

// diary_list 관련 함수들
void list_diaries();

// user_auth 관련 함수들
MYSQL* init_db_connection();
int check_id_exists(MYSQL* conn, const char* id);
int register_user(MYSQL* conn);
int login_user(MYSQL* conn);
void find_id_password(MYSQL* conn);
void close_db_connection(MYSQL* conn);