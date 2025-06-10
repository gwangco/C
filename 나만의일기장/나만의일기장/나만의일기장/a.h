#pragma once
// --- ���������� �ʿ��� ǥ�� ���̺귯�� ---
#include <stdio.h>    // ǥ�� ����� (printf, scanf, fopen ��)
#include <stdlib.h>   // �Ϲ� ��ƿ��Ƽ (system, malloc, free, exit, atoi ��)
#include <string.h>   // ���ڿ� �Լ� (strlen, strcpy, strcat, strstr, strcmp, sscanf ��)
#include <time.h>     // �ð�/��¥ ���� (time, localtime)

// Windows �ü�� ���� API
#include <windows.h>  // Windows API (FindFirstFile, FindNextFile ��)
#define MAX_PATH 260  // Windows �⺻ ��� ����

// --- MySQL �����ͺ��̽� ���� ---
#include <mysql.h>    // MySQL �����ͺ��̽� ���� �Լ� �� ����ü

// MySQL �����ͺ��̽� ���� ������ ����
#define DB_HOST  // MySQL ���� IP �ּ� (���⼭�� ���� PC)
#define DB_USER       // MySQL ���� ����� �̸�
#define DB_PASS     // MySQL ���� ��й�ȣ (���� MySQL ��й�ȣ�� ���� �ʼ�!)
#define DB_NAME      // ����� �����ͺ��̽� �̸� (�̸� �����Ǿ� �־�� ��)
#define DB_PORT        // MySQL ���� ��Ʈ (�⺻�� 3306, ���⼭�� 3307�� ����)

// ���ڿ� ���� ���� ����('\n')�� �����ϴ� ��ũ�� �Լ�
#define CHOP(x) x[strlen(x) - 1] = '\0'

// --- �Լ� ���� ��� (������ �ϱ��� ���α׷��� ��� �Լ�) ---

// diary_display ���� �Լ���
void draw_border(int width, char character);
void print_diary_block(const char* title, const char* content, int width);

// diary_menu ���� �Լ���
void display_main_menu(int width);
void handle_menu_choice(int choice, int page_width, MYSQL* db_conn, int* logged_in_status);
void display_auth_menu();
int handle_auth_choice(int choice, MYSQL* db_conn, int* logged_in_status);

// diary_io ���� �Լ���
int get_next_diary_sequence(int year, int month, int day);
void write_diary();
void view_diary(int page_width);

// diary_list ���� �Լ���
void list_diaries();

// user_auth ���� �Լ���
MYSQL* init_db_connection();
int check_id_exists(MYSQL* conn, const char* id);
int register_user(MYSQL* conn);
int login_user(MYSQL* conn);
void find_id_password(MYSQL* conn);
void close_db_connection(MYSQL* conn);