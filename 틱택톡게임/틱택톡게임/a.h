#pragma once
#include <stdio.h>
#include <stdlib.h>
#define ROW 3
#define COL 3

void draw(int(*ip)[3], int, int);
void human(int(*ip)[3]);
void computer(int(*ip)[3],int,int);
void check(int(*ip)[3], int, int);