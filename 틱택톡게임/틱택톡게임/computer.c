#include <stdio.h>

void computer(int(*ip)[3], int r, int c) {
	static int count = 1;

	// 1. ù ��° ���� (�߾ӿ� ����)
	if (count == 1) {
		ip[1][1] = 1;
		count++;
		return;
	}

	// 2. �� ��° ���� (���� �ڸ� ���ϱ�)
	if (count == 2) {
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				if (ip[i][j] == 0 && ip[2 - i][2 - j] != 2) {
					ip[i][j] = 1;
					count++;
					return;
				}
			}
		}
	}

	// 3. ���� �켱
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (ip[i][j] == 0 && ip[2 - i][2 - j] == 1) {
				ip[i][j] = 1;
				return;
			}
		}
	}

	// 4. ���� (����� �¸� ����)
	for (int i = 0; i < 3; i++) {
		// ���� ����
		if (ip[i][0] == 2 && ip[i][1] == 2 && ip[i][2] == 0) {
			ip[i][2] = 1;
			return;
		}
		if (ip[i][0] == 2 && ip[i][2] == 2 && ip[i][1] == 0) {
			ip[i][1] = 1;
			return;
		}
		if (ip[i][1] == 2 && ip[i][2] == 2 && ip[i][0] == 0) {
			ip[i][0] = 1;
			return;
		}

		// ���� ����
		if (ip[0][i] == 2 && ip[1][i] == 2 && ip[2][i] == 0) {
			ip[2][i] = 1;
			return;
		}
		if (ip[0][i] == 2 && ip[2][i] == 2 && ip[1][i] == 0) {
			ip[1][i] = 1;
			return;
		}
		if (ip[1][i] == 2 && ip[2][i] == 2 && ip[0][i] == 0) {
			ip[0][i] = 1;
			return;
		}
	}

	// 5. �밢�� ����
	if (ip[0][0] == 2 && ip[1][1] == 2 && ip[2][2] == 0) {
		ip[2][2] = 1;
		return;
	}
	if (ip[0][0] == 2 && ip[2][2] == 2 && ip[1][1] == 0) {
		ip[1][1] = 1;
		return;
	}
	if (ip[1][1] == 2 && ip[2][2] == 2 && ip[0][0] == 0) {
		ip[0][0] = 1;
		return;
	}

	if (ip[0][2] == 2 && ip[1][1] == 2 && ip[2][0] == 0) {
		ip[2][0] = 1;
		return;
	}
	if (ip[0][2] == 2 && ip[2][0] == 2 && ip[1][1] == 0) {
		ip[1][1] = 1;
		return;
	}
	if (ip[1][1] == 2 && ip[2][0] == 2 && ip[0][2] == 0) {
		ip[0][2] = 1;
		return;
	}

	// 6. �� �ڸ� �ƹ� ���̳� ����
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (ip[i][j] == 0) {
				ip[i][j] = 1;
				return;
			}
		}
	}
}
