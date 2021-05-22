#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<time.h>

#define ITEMSHAPE  "★"
#define SNAKE_HEAD_SHAPE "■"
#define SNAKE_SHAPE "□"
#define SNAKE_DELETE_SHAPE "  "
#define GAMENAME "S N A K E     G A M E"
#define GAMEOVER "G A M E      O V E R"
#define PESSKEY " - press anykey - "
#define EMPTY_PESSKEY "                            "
#define SCORE "Score : %d"
#define SPEED "\tSpeed : %d"
#define GAMENAME_X 10
#define GAMENAME_Y 9
#define PRESSKEY_X 11
#define PRESSKEY_Y 13
#define SCORE_X 2
#define SCORE_Y 24

#define TOTAL_SNAKE_LENGTH 100
#define START_SPEED 200
#define REDUCE_SPEED 5
#define MAX_SPEED 50
#define PLUS_SCORE 10
#define SNAKE_HEAD_X 10
#define SNAKE_HEAD_Y 9

int score = 0;
int speed = START_SPEED;
int snake_length = 5;

typedef struct {
    int x;
    int y;
}xy_Position;


void put_xy_Position(int x, int y, char* data) {
    COORD pos = { x * 2,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("%s", data);
}

void empty_screen() {
    //32x23
    system("cls");
    printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■                                                          ■\n");
    printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
}


void print_scoreboard() {
    put_xy_Position(SCORE_X, SCORE_Y, "");
    printf(EMPTY_PESSKEY);
    put_xy_Position(SCORE_X, SCORE_Y, "");
    printf(SCORE, score);
    printf(SPEED, speed);
}

void start_screen() {
    srand(time(NULL));
    empty_screen();
    put_xy_Position(GAMENAME_X, GAMENAME_Y, GAMENAME);
    while (1) {
        put_xy_Position(PRESSKEY_X, PRESSKEY_Y, PESSKEY);
        Sleep(speed);
        if (_kbhit()) break;
        put_xy_Position(PRESSKEY_X, PRESSKEY_Y, EMPTY_PESSKEY);
        Sleep(speed);
    }
    while (_kbhit()) _getch();
    system("cls");
    empty_screen();
    print_scoreboard();
}

void first_snake(int snake_length, xy_Position* snake) {
    for (int i = 0; i < snake_length; i++) {
        snake[i].x = SNAKE_HEAD_X + i;
        snake[i].y = SNAKE_HEAD_Y;
        if (i == 0) put_xy_Position(snake[i].x, snake[i].y, SNAKE_HEAD_SHAPE);
        else put_xy_Position(snake[i].x, snake[i].y, SNAKE_SHAPE);
    }
}

int check_collision(xy_Position* snake) {
    if ((snake[0].x <= 0) || (snake[0].y <= 0) || (snake[0].x >= 29) || (snake[0].y >= 21)) return 0;
    for (int coll = 1; coll < snake_length; coll++)
        if ((snake[0].x == snake[coll].x) && (snake[0].y == snake[coll].y)) return 0;
    return 1;
}

xy_Position create_item(xy_Position item) {
    item.x = rand() % 28 + 1;
    item.y = rand() % 20 + 1;
    put_xy_Position(item.x, item.y, ITEMSHAPE);
    return item;
}

xy_Position item_create_n_check(xy_Position item, xy_Position* snake) {
    if ((snake[0].x == item.x) && (snake[0].y == item.y)) {
        score += PLUS_SCORE;
        item = create_item(item);
        if (speed <= MAX_SPEED) speed = MAX_SPEED;
        else speed -= REDUCE_SPEED;
        if (snake_length < TOTAL_SNAKE_LENGTH) {
            snake[snake_length] = snake[snake_length - 1];
            snake_length++;
        }
        print_scoreboard();
    }
    return item;
}

int move_snake(xy_Position item, xy_Position* snake) {
    static int head_direction = 2;
    if (_kbhit()) {
        int presskey = _getch();
        if (presskey == 27) return 0;
        while (presskey == 224) break;
        switch (presskey) {
        case 72: { //up
            head_direction = 0;
            break;
        }
        case 80: { //down
            head_direction = 1;
            break;
        }
        case 75: { //left
            head_direction = 2;
            break;
        }
        case 77: { //right
            head_direction = 3;
            break;
        }
        }
    }
    if (head_direction != -1) {
        put_xy_Position(snake[snake_length - 1].x, snake[snake_length - 1].y, SNAKE_DELETE_SHAPE);
        for (int i = snake_length - 1; i > 0; i--) snake[i] = snake[i - 1];
        put_xy_Position(snake[1].x, snake[1].y, SNAKE_SHAPE);
    }
    switch (head_direction) {
    case 0: {//위
        snake[0].y--;
        break;
    }
    case 1: { //아레
        snake[0].y++;
        break;
    }
    case 2: { //왼쪽
        snake[0].x--;
        break;
    }
    case 3: { //오른쪽
        snake[0].x++;
        break;
    }
    }
    put_xy_Position(snake[0].x, snake[0].y, SNAKE_HEAD_SHAPE);
    Sleep(speed);
    return 1;
}

void finish_screen() {
    empty_screen();
    put_xy_Position(GAMENAME_X, GAMENAME_Y, GAMEOVER);
    put_xy_Position(PRESSKEY_X, PRESSKEY_Y, "");
    printf(SCORE, score);
    put_xy_Position(SCORE_X, SCORE_Y, "");
}
int main()
{
    static int snake_length = 5;
    xy_Position snake_position[TOTAL_SNAKE_LENGTH];
    xy_Position item_position = { 0,0 };
    int speed = START_SPEED;
    int coll_body = 0;

    start_screen();
    item_position = create_item(item_position);
    first_snake(snake_length, snake_position);
    while (1) {
        item_position = item_create_n_check(item_position, snake_position);
        if (check_collision(snake_position) == 0) break;
        if (move_snake(item_position, snake_position) == 0) break;
    }
    finish_screen();
}