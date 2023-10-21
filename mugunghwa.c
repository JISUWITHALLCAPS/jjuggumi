#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "keyin.h"
#include "jjuggumi.h"
#include "canvas.h"
#include <string.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

typedef struct {
	int x, y;
} PLAYER;

PLAYER xy[10];
bool copy[PLAYER_MAX], win[PLAYER_MAX];
char str1[100],str2[100];
int period[PLAYER_MAX],dir[PLAYER_MAX];
static int dx[4] = { -1, 1, 0, 0 };
static int dy[4] = { 0, 0, -1, 0 };
int cnt, sum_tick, temp_tick = 1, camera;
int tick4print = 0;
void kill(int p);
void check_move(int p, int dir);

void move_tail(int player, int nx, int ny) {
	int p = player;
	back_buf[nx][ny] = back_buf[xy[p].x][xy[p].y];
	back_buf[xy[p].x][xy[p].y] = ' ';
	xy[p].x = nx;
	xy[p].y = ny;
}
void passed(int p,int nx,int ny) {
	if ((nx == 15/2 && ny == 2) || (nx == 15 / 2 - 1 && ny == 2) || (nx == 15 / 2 + 1 && ny == 2) || (nx == 15/2+2 && ny ==1) || nx == 15/2-2&&ny==1) {
		win[p] = true;
		back_buf[nx][ny] = ' ';
		
	}
}
void move_player(key_t key) {
	int dir;  // ������ ����(0~3)
	switch (key) {
	case K_UP: dir = DIR_UP; break;
	case K_DOWN: dir = DIR_DOWN; break;
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}
	if (camera == 1) {
		check_move(0, dir);
	}
	int nx, ny;
	nx = xy[0].x + dx[dir];
	ny = xy[0].y + dy[dir];
	passed(0,nx,ny);
	if (!placable(nx, ny)) {
		return;
	}
	move_tail(0, nx, ny);
}

int move_p(int p) {
	int mp;
	mp = randint(1, 100);
	if (camera == 1) {
		if (mp >= 10) {
			return 3;
		}
		else {
			mp = randint(1, 10);
			if (mp == 1) {
				return 2;
			}
			if (mp == 2) {
				return 1;
			}
			if (mp == 3) {
				return 0;
			}
		}
	}
	if (mp <= 70) {
		return 2;
	}
	else if (mp <= 80) {
		return 0;
	}
	else if (mp <= 9) {
		return 1;
	}
	else {
		return 3;
	}
}
void r_move_computer(int player, int dir[]) {
	int p = player;  // �̸��� ��...
	int nx, ny;  // �������� ������ ���� �ڸ�
	dir[p] = move_p(p);
	if (camera == 1 && dir[p] != 3) {
		check_move(p, dir[p]);
	}
	nx = xy[p].x + dx[dir[p]];
	ny = xy[p].y + dy[dir[p]];
	passed(p, nx, ny);
	passed(p, nx, ny);
	if (!placable(nx, ny)) {
		return;
	}
	move_tail(p, nx, ny);
}
void check_move(int p,int dir) {
	if (dir == 0 || dir == 1) {
		player[p] = false;
	}
	else {
		bool t = false;
		for (int i = 1; i < xy[p].y; i++) {
			if (back_buf[xy[p].x][i] != ' ' && back_buf[xy[p].x][i] != '@') {
				t = true;
			}
		}
		if (t == false) {
			player[p] = false;
		}
	}
	kill(p);
}
void map_set() {
	system("cls");
	back_buf_init(15, 35);
	back_buf[15 / 2 - 1][1] = '#', back_buf[15 / 2][1] = '#', back_buf[15 / 2 + 1][1] = '#';
	for (int i = 0; i < n_player; i++) {
		xy[i].x = (15 / 2) - (n_player / 2) + i;
		xy[i].y = 33;
		back_buf[xy[i].x][xy[i].y] = '0' + i;
		period[i] = randint(20, 30) * 10;
	}
	draw();
	display();
	Sleep(1000);
	memcpy(copy, player, sizeof(player));
}

void stop() {
	back_buf[15 / 2 - 1][1] = back_buf[15 / 2][1] = back_buf[15 / 2 + 1][1] = '@';
	camera = 1;
	for (int i = 1; i < n_player; i++) {
		dir[i] = 3;
	}
	sum_tick = 10;
	if (temp_tick % 3001 == 0) {
		int t = 0;
		for (int i = 0; i < n_player; i++) {
			if (copy[i] != player[i]) {
				t = 1;
				break;
			}
		}
		if (t == 1) {
			for (int i = 0; i < n_player; i++) {
				if (copy[i] != player[i]) {
					sprintf(str2, "%d,", i);
					strcat(str1, str2);
				}
			}
			memcpy(copy, player, sizeof(player));
			sprintf(str2,"%d player %s dead!",n_alive,str1);
			//sprintf(str1,,)
			//gotoxy(16, 0);
			//printf("%s", str2);
			dialog(str2);
			memset(str1, 0, sizeof(str1));
		}
		camera = 0;
		gotoxy(15, 0);
		printf("                                     ");
		cnt = 0;
		temp_tick = 1;
		sum_tick = 0;
		tick4print = 0;
		back_buf[15 / 2 - 1][1] = '#', back_buf[15 / 2][1] = '#', back_buf[15 / 2 + 1][1] = '#';
	}
}
void kill(p) {
	if (player[p] != true) {
		back_buf[xy[p].x][xy[p].y] = ' ';
		n_alive -= 1;
	}
}
void mugunghwa() {
	map_set();
	
	while (1) {
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED && player[0] == true && win[0] == false) {
			move_player(key);
		}
	
		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0 && player[i] == true && win[i] == false) {
				r_move_computer(i, dir);
			}
		}

		if ( cnt == 0) {
			gotoxy(15,0);
				switch (tick4print) {
				case 200:printf("�� ");  break;
				case 300:printf("�� �� ");  break;
				case 500:printf("�� �� ȭ ");  break;
				case 800:printf("�� �� ȭ �� ");  break;
				case 1200:printf("�� �� ȭ �� �� "); cnt = 6; break;
				default: break;
				}
		}
		if ( cnt == 6) {
			gotoxy(15, 15);
				switch (tick4print) {
				case 1600:printf("�� "); break;
				case 2000:printf("�� �� ");  break;
				case 2300:printf("�� �� �� ");  break;
				case 2500:printf("�� �� �� �� ");  break;
				case 2600:printf("�� �� �� �� �� "); cnt = 11;  break;
				default: break;
				}
		}
		if (cnt > 10) {
			stop();
		}
		draw();
		display();
		Sleep(10);
		tick += 10;
		tick4print += 10;
		temp_tick += sum_tick;
		int e = 0;
		for (int i = 0; i < n_player; i++) {
			if (win[i] == true || player[i] == false) {
				e++;
			}
		}
		if (e == n_player) {
			break;
		}
	}
}