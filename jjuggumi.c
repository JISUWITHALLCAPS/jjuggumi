// 2023-2 고급프로그래밍 과제: 쭈꾸미 게임
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "jjuggumi.h"
#include <windows.h>

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);

// low 이상 high 이하 난수를 발생시키는 함수
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	printf("플레이어 수: ");
	scanf_s("%d", &n_player);

	n_alive = n_player;

	for (int i = 0; i < n_player; i++) {
		player[i] = true;
	}
	return 0;
}

void intro(void) // intro()
{
	int x = 10;
	int y = 1;

	for (int i = x; i >= 1; i--)
	{
		for (int j = 1; j <= x - i; j++)
		{
			printf(" ");
		}
		for (int j = 1; j <= 2 * i - 1; j++)
		{
			printf("*");
		}
		printf("\n");
	}
	printf("게임을 시작합니다.\n");

	for (int h = 0; h < 5; h++)
	{
		Sleep(1000);
		if (h == 1)
		{
			Sleep(1000);
			break;
		}
	}
}

void ending(void) // 확실한지?
{
	int winner = 0;
	for (int i = 0; i < n_player; i++)
	{
		if (player[i] == true && win[i] == true)
		{
			winner = i;
		}
	}
	if (n_alive == 1)
	{
		system("cls");
		gotoxy(7, 5);
		printf("우승자는 player %d번 입니다.\n", winner);
	}
	// 게임 하나기 때문에 우승자가 가려지지 않을 경우를 위한 임시코드
	else if (n_alive == 0) {
		system("cls");
		gotoxy(7, 5);
		printf("모든 플레이어가 탈락하였습니다.\n");
	}
	else if (n_alive > 1) {
		system("cls");
		gotoxy(7, 5);
		printf("%d명의 플레이어가 통과했습니다.\n");
	}
}

int main(void) {
	jjuggumi_init();
	intro();
	//sample();
	system("cls");
	system("pause");
	char message[] = "mugunghwa game";
	dialog(message);
	mugunghwa();
	//nightgame();
	//juldarigi();
	//jebi();
	ending();
	return 0;
}
