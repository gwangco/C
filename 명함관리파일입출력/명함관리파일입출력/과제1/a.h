#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10

typedef struct nameCard {
    char name[20];
    char mobile[20];
    char email[20];
    char address[100];
} NAME;

int menu(void);
void insert(NAME*);
void display(NAME*);
void search(NAME* , char*);
void update(NAME*, char*);
