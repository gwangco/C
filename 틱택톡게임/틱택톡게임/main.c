#include "a.h"
int main(void) {
	int board[ROW][COL] = { 0 };

//	draw(board, ROW, COL);          //void draw(int(*ip)[3], int,int);

	while (1) {


		computer(board, ROW, COL);
		draw(board, ROW, COL);
		check(board, ROW, COL);

		human(board);     //void human(int(*ip)[3]);
		draw(board, ROW, COL);
		check(board, ROW, COL);

		system("pause");
		system("cls");
	}
	return 0;
}
