#include <stdio.h>

void computer(int(*ip)[3], int r, int c) {
	static int count = 1;

	// 1. 첫 번째 착수 (중앙에 놓기)
	if (count == 1) {
		ip[1][1] = 1;
		count++;
		return;
	}

	// 2. 두 번째 착수 (나쁜 자리 피하기)
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

	// 3. 공격 우선
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (ip[i][j] == 0 && ip[2 - i][2 - j] == 1) {
				ip[i][j] = 1;
				return;
			}
		}
	}

	// 4. 수비 (상대의 승리 저지)
	for (int i = 0; i < 3; i++) {
		// 가로 차단
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

		// 세로 차단
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

	// 5. 대각선 차단
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

	// 6. 빈 자리 아무 곳이나 선택
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (ip[i][j] == 0) {
				ip[i][j] = 1;
				return;
			}
		}
	}
}
