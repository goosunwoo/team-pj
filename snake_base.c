#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define CMHEIGHT 25
#define CMWIDTH 25
#define SMHEIGHT 20
#define SMWIDTH 20
#define MAX_LENGTH 100

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
//===========================================================
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
        if (count == 30) {//맵 사이즈 적용 판별
            WIDTH = SMWIDTH;
            HEIGHT = SMHEIGHT;
            common_size();
        }
        else {
            if (count % 2 == 0) {//맵 축소 경고
                Warning_size();
                count += 1;
            }
            else {
                common_size();
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
                count -= 1;
            }
            else {
                common_size();
                count -= 1;
            }
        }
    }

    printf("Score: %d\n", score);
    printf("Press X to quit the game\n");
}
//===========================================================
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
//===========================================================
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
    Sleep(100);
}
//===========================================================
int main() {
    setup();
    while (!gameover) {
        draw();
        input();
        logic();
    }
    printf("게임종료! 최종 스코어: %d\n", score);
    return 0;
}
