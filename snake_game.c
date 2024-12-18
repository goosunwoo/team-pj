#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define CMHEIGHT 25
#define CMWIDTH 25
#define SMHEIGHT 20
#define SMWIDTH 20
#define MAX_LENGTH 100

#define MAX_LENGTH_2P 100
#define HEIGHT_2P 20
#define WIDTH_2P 40

typedef struct {
	int snakeX[MAX_LENGTH_2P];
	int snakeY[MAX_LENGTH_2P];
	int length;
	int fruitX, fruitY;
	int score;
	int direction;
	int isAlive;
} Player_2p;

//1p=============================================================================
int HEIGHT = CMHEIGHT, WIDTH = CMWIDTH;
int gameover, score;
int snakeX[MAX_LENGTH], snakeY[MAX_LENGTH]; // 스네이크의 몸통 좌표
int fruitX, fruitY;
int length = 1; // 스네이크의 초기 길이
int direction; // 현재 방향
int event = 0;//이벤트 활성화 판별 변수
int count = 0;//맵 축소 경고 카운트

void setup() {//시작화면
	gameover = 0;
	direction = 0; // 초기 방향: 정지
	snakeX[0] = HEIGHT / 2;
	snakeY[0] = WIDTH / 2;

	// 과일의 초기 위치
	fruitX = rand() % (HEIGHT - 2) + 1;
	fruitY = rand() % (WIDTH - 2) + 1;

	score = 0;
	
}

//주 화면 출력 함수
void common_size() {
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
				printf("#");
			}
			else if (i == fruitX && j == fruitY) {
				printf("*");
			}
			else {
				int isBody = 0;
				for (int k = 0; k < length; k++) {
					if (snakeX[k] == i && snakeY[k] == j) {
						printf("0");
						isBody = 1;
						break;
					}
				}
				if (!isBody) printf(" ");
			}
		}
		printf("\n");
	}
}
//경고용 화면 출력 함수
void Warning_size() {
	int Ws_HEIGHT, Ws_WIDTH;
	if (event == 1)
	{
		Ws_WIDTH = SMWIDTH;
		Ws_HEIGHT = SMHEIGHT;
	}
	else {
		Ws_WIDTH = CMWIDTH;
		Ws_HEIGHT = CMHEIGHT;
	}

	for (int i = 0; i < Ws_HEIGHT; i++) {
		for (int j = 0; j < Ws_WIDTH; j++) {
			if (i == 0 || i == Ws_HEIGHT - 1 || j == 0 || j == Ws_WIDTH - 1) {
				printf("#");
			}
			else if (i == fruitX && j == fruitY) {
				printf("*");
			}
			else {
				int isBody = 0;
				for (int k = 0; k < length; k++) {
					if (snakeX[k] == i && snakeY[k] == j) {
						printf("0");
						isBody = 1;
						break;
					}
				}
				if (!isBody) printf(" ");
			}
		}
		printf("\n");
	}
}
//화면 출력 메인 함수
void draw() {
	system("cls");
	if (event == 1)//이벤트 발생 판별
	{
		if (count == 20) {//맵 사이즈 적용 판별
			WIDTH = SMWIDTH;
			HEIGHT = SMHEIGHT;
			common_size();
		}
		else {
			if (count % 2 == 0) {//맵 축소 경고
				Warning_size();
				Beep(1500, 150);
				count += 1;
			}
			else {
				common_size();
				Beep(500, 150);
				count += 1;
			}
		}
	}
	else {
		if (count == 0) {//맵 사이즈 적용 판별
			WIDTH = CMWIDTH;
			HEIGHT = CMHEIGHT;
			common_size();
		}
		else {
			if (count % 2 == 0) {//맵 축소 경고
				Warning_size();
				Beep(1500, 150);
				count -= 1;
			}
			else {
				common_size();
				Beep(500, 150);
				count -= 1;
			}
		}
	}

	printf("Score: %d\n", score);
	printf("Press X to quit the game\n");
}

void input() {
	if (_kbhit()) {
		switch (getch()) {
		case 'a': direction = 1; break; // 왼쪽
		case 's': direction = 2; break; // 아래
		case 'd': direction = 3; break; // 오른쪽
		case 'w': direction = 4; break; // 위
		case 'x': gameover = 1; break; // 종료
		}
	}
}

void logic() {
	// 몸통 위치를 업데이트
	for (int i = length - 1; i > 0; i--) {
		snakeX[i] = snakeX[i - 1];
		snakeY[i] = snakeY[i - 1];
	}

	// 머리 위치를 업데이트
	switch (direction) {
	case 1: snakeY[0]--; break; // 왼쪽
	case 2: snakeX[0]++; break; // 아래
	case 3: snakeY[0]++; break; // 오른쪽
	case 4: snakeX[0]--; break; // 위
	}

	// 벽에 충돌하면 게임 종료
	if (snakeX[0] <= 0 || snakeX[0] >= HEIGHT - 1 || snakeY[0] <= 0 || snakeY[0] >= WIDTH - 1) {
		gameover = 1;
	}

	// 자신과 충돌하면 게임 종료
	for (int i = 1; i < length; i++) {
		if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
			gameover = 1;
		}
	}

	// 과일을 먹으면
	if (snakeX[0] == fruitX && snakeY[0] == fruitY) {
		score += 10;
		Beep(700, 150);
		length++;
		fruitX = rand() % (HEIGHT - 2) + 1;
		fruitY = rand() % (WIDTH - 2) + 1;
	}

	//과일 위치 이탈 수정
	if (fruitX <= 0 || fruitX >= HEIGHT || fruitY <= 0 || fruitY >= WIDTH)
	{
		fruitX = rand() % (HEIGHT - 2) + 1;
		fruitY = rand() % (WIDTH - 2) + 1;
	}


	//이벤트 실행 점수 설정
	if (score % 100 == 0 && score != 0) {
		event = 0;
	}
	else if (score % 50 == 0 && score != 0) {
		event = 1;
	}



	// 속도 조절
	Sleep(150);
}

void game_over_music() {
	int startFreq = 800; // 시작 주파수
	int decrement = 100; // 감소 폭

	for (int i = 0; i < 5; i++) { // 5번 반복
		Beep(startFreq, 300); // 현재 주파수의 음
		startFreq -= decrement; // 주파수 감소
		Sleep(100); // 짧은 멈춤
	}
}

void game_mode1() {
	setup();

	while (!gameover) {
		draw();
		input();
		logic();
	}

	PlaySound(NULL, NULL, SND_ASYNC);
	game_over_music();


	printf("게임종료! 최종 스코어: %d\n", score);
	return 0;
}
//2p==========================================================================
Player_2p p1, p2;
int gameover_2p;

void setup_2p() {
	gameover_2p = 0;

	// Player 1 초기화
	p1.length = 1;
	p1.snakeX[0] = HEIGHT_2P / 2;
	p1.snakeY[0] = WIDTH_2P / 4;
	p1.fruitX = rand() % (HEIGHT_2P - 2) + 1;
	p1.fruitY = rand() % (WIDTH_2P / 2 - 2) + 1;
	p1.score = 0;
	p1.direction = 0;
	p1.isAlive = 1;

	// Player 2 초기화
	p2.length = 1;
	p2.snakeX[0] = HEIGHT_2P / 2;
	p2.snakeY[0] = 3 * (WIDTH_2P / 4);
	p2.fruitX = rand() % (HEIGHT_2P - 2) + 1;
	p2.fruitY = rand() % (WIDTH_2P / 2 - 2) + (WIDTH_2P / 2);
	p2.score = 0;
	p2.direction = 0;
	p2.isAlive = 1;

	
}

void draw_2p() {
	system("cls");

	for (int i = 0; i < HEIGHT_2P; i++) {
		// 왼쪽 맵 (Player 1)
		for (int j = 0; j < WIDTH_2P / 2; j++) {
			if (i == 0 || i == HEIGHT_2P - 1 || j == 0 || j == WIDTH_2P / 2 - 1) {
				printf("#");
			}
			else if (i == p1.fruitX && j == p1.fruitY && p1.isAlive) {
				printf("*");
			}
			else {
				int isBody = 0;
				for (int k = 0; k < p1.length; k++) {
					if (p1.snakeX[k] == i && p1.snakeY[k] == j) {
						printf("0");
						isBody = 1;
						break;
					}
				}
				if (!isBody) printf(" ");
			}
		}

		// 맵 사이 경계
		printf(" | ");

		// 오른쪽 맵 (Player 2)
		for (int j = 0; j < WIDTH_2P / 2; j++) {
			if (i == 0 || i == HEIGHT_2P - 1 || j == 0 || j == WIDTH_2P / 2 - 1) {
				printf("#");
			}
			else if (i == p2.fruitX && j + WIDTH_2P / 2 == p2.fruitY && p2.isAlive) {
				printf("*");
			}
			else {
				int isBody = 0;
				for (int k = 0; k < p2.length; k++) {
					if (p2.snakeX[k] == i && p2.snakeY[k] - WIDTH_2P / 2 == j) {
						printf("0");
						isBody = 1;
						break;
					}
				}
				if (!isBody) printf(" ");
			}
		}

		printf("\n");
	}

	printf("Player 1 Score: %d   Player 2 Score: %d\n", p1.score, p2.score);
	printf("Press X to quit the game\n");
}

void input_2p() {
	if (_kbhit()) {
		switch (getch()) {
			// Player 1 입력
		case 'a': if (p1.direction != 3) p1.direction = 1; break;
		case 's': if (p1.direction != 4) p1.direction = 2; break;
		case 'd': if (p1.direction != 1) p1.direction = 3; break;
		case 'w': if (p1.direction != 2) p1.direction = 4; break;

			// Player 2 입력
		case 'j': if (p2.direction != 3) p2.direction = 1; break;
		case 'k': if (p2.direction != 4) p2.direction = 2; break;
		case 'l': if (p2.direction != 1) p2.direction = 3; break;
		case 'i': if (p2.direction != 2) p2.direction = 4; break;

			// 게임 종료
		case 'x': gameover_2p = 1; break;
		}
	}
}

void updatePlayer_2p(Player_2p* player, int offset) {
	if (!player->isAlive) return;

	// 몸통 위치 업데이트
	for (int i = player->length - 1; i > 0; i--) {
		player->snakeX[i] = player->snakeX[i - 1];
		player->snakeY[i] = player->snakeY[i - 1];
	}

	// 머리 위치 업데이트
	switch (player->direction) {
	case 1: player->snakeY[0]--; break; // 왼쪽
	case 2: player->snakeX[0]++; break; // 아래
	case 3: player->snakeY[0]++; break; // 오른쪽
	case 4: player->snakeX[0]--; break; // 위
	}

	// 벽 충돌
	if (player->snakeX[0] <= 0 || player->snakeX[0] >= HEIGHT_2P - 1 ||
		player->snakeY[0] - offset <= 0 || player->snakeY[0] - offset >= WIDTH_2P / 2 - 1) {
		player->isAlive = 0;
	}

	// 자신 충돌
	for (int i = 1; i < player->length; i++) {
		if (player->snakeX[0] == player->snakeX[i] && player->snakeY[0] == player->snakeY[i]) {
			player->isAlive = 0;
		}
	}

	// 과일 먹기
	if (player->snakeX[0] == player->fruitX && player->snakeY[0] == player->fruitY) {
		player->score += 10;
		player->length++;
		player->fruitX = rand() % (HEIGHT_2P - 2) + 1;
		player->fruitY = rand() % (WIDTH_2P / 2 - 2) + 1 + offset;
	}
}

void logic_2p() {
	if (p1.isAlive) updatePlayer_2p(&p1, 0);
	if (p2.isAlive) updatePlayer_2p(&p2, WIDTH_2P / 2);

	if (!p1.isAlive && !p2.isAlive) gameover_2p = 1;

	Sleep(100);
}

void game_mode2() {
	setup_2p();

	while (!gameover_2p) {
		draw_2p();
		input_2p();
		logic_2p();
	}

	PlaySound(NULL, NULL, SND_ASYNC);
	printf("게임끝!!\n");

	if (p1.score > p2.score) {
		printf("플레이어 1 승리 점수: %d\n", p1.score);
	}
	else if (p2.score > p1.score) {
		printf("플레이어 2 승리 점수: %d\n", p2.score);
	}
	else {
		printf("무승부!\n");
	}

	return 0;
}
//main===========================================================
int main(void) {
	int choice;

	PlaySound(TEXT("videoplayback.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
re:
	printf("=============================================\n");
	printf("게임 모드 선택\n");
	printf("1. 일반모드(조작키 : wasd)\n");
	printf("2. 2인모드(1p 조작키 : wasd, 2p 조작키 : ijkl)\n");
	printf("3. 종료\n");
	printf("선택 : ");
	scanf_s("%d", &choice);

	if (choice == 1) {
		game_mode1();
	}
	else if(choice == 2) {
		game_mode2();
	}
	else if (choice == 3) {
		return 0;
	}
	else {
		printf("잘못 입력하셨습니다. 다시 입력해주세요.\n");
		goto re;
	}
	game_over_music();
	return 0;
}
