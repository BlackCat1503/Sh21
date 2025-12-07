#include <stdio.h>

// Прототипы функций
int draw(int rocket_1_Y, int rocket_2_Y, int ball_x, int ball_y, int scPlayer1, int scPlayer2);
int update_right_pad(int right_pad_y, char key_input, int height);
int update_left_pad(int left_pad_y, char key_input, int height);
int change_vector_ball_x(int vector_ball_x, int ball_x, int ball_y, int rocket_1_Y, int rocket_2_Y);
int change_vector_ball_y(int vector_ball_y, int ball_y);
int move_ball_x(int vector_ball_x, int ball_x);
int move_ball_y(int vector_ball_y, int ball_y);

int main() {
    int rocket_1_Y = 25 / 2;  // Изначально центральное положение
    int rocket_2_Y = 25 / 2;  // Изначально центральное положение
    int ball_x = 80 / 2;
    int ball_y = 25 / 2;
    int vector_x = 1;
    int vector_y = 1;
    int scPlayer1 = 0;
    int scPlayer2 = 0;

    while (scPlayer1 < 21 && scPlayer2 < 21) {
        char key_input;
        // Отображаем текущее состояние игры
        draw(rocket_1_Y, rocket_2_Y, ball_x, ball_y, scPlayer1, scPlayer2);

        // Ожидаем ввода от пользователя
        printf("Enter command (Space to step, A/Z for player 1, K/M for player 2): ");
        key_input = getchar();

        // Пропускаем лишние символы
        while (getchar() != '\n')
            ;

        // Проверка на допустимый ввод
        if (key_input != ' ' && key_input != 'a' && key_input != 'A' && key_input != 'z' &&
            key_input != 'Z' && key_input != 'k' && key_input != 'K' && key_input != 'm' &&
            key_input != 'M') {
            printf(
                "Error: Invalid input. Please use A/Z for player 1, K/M for player 2, or Space to step.\n");
        } else {
            // Обрабатываем ввод для движения ракеток
            rocket_1_Y = update_left_pad(rocket_1_Y, key_input, 25);
            rocket_2_Y = update_right_pad(rocket_2_Y, key_input, 25);

            // Изменяем направление мяча и перемещаем мяч
            vector_x = change_vector_ball_x(vector_x, ball_x, ball_y, rocket_1_Y, rocket_2_Y);
            vector_y = change_vector_ball_y(vector_y, ball_y);
            ball_x = move_ball_x(vector_x, ball_x);
            ball_y = move_ball_y(vector_y, ball_y);

            // Проверяем гол
            if (ball_x <= 1) {
                scPlayer2++;
                ball_x = 80 / 2;
                ball_y = 25 / 2;
                vector_x = 1;
                vector_y = 1;
            }
            if (ball_x >= 78) {
                scPlayer1++;
                ball_x = 80 / 2;
                ball_y = 25 / 2;
                vector_x = -1;
                vector_y = -1;
            }
        }
    }

    // Объявляем победителя
    if (scPlayer1 == 21) {
        printf("Player one wins!\n");
    } else if (scPlayer2 == 21) {
        printf("Player two wins!\n");
    }

    return 0;
}

int draw(int rocket_1_Y, int rocket_2_Y, int ball_x, int ball_y, int scPlayer1, int scPlayer2) {
    // Верхняя граница
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 80; ++x) {
            if ((y == 0 && x != 0 && x != 80 - 1) || (y == 3 - 1 && x != 0 && x != 80 - 1)) {
                printf("-");
            } else if (((x == 0 && y != 0 && y != 3 - 1) || (x == 80 - 1 && y != 0 && y != 3 - 1))) {
                printf("|");
            } else if (y == 1 && x == 37) {
                printf("%d", scPlayer1);
            } else if (y == 1 && x == 42) {
                printf("%d", scPlayer2);
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    // Основное игровое поле
    for (int y = 3; y < 25; ++y) {
        for (int x = 0; x < 80; ++x) {
            if (x == 2 && (y == rocket_1_Y - 1 || y == rocket_1_Y || y == rocket_1_Y + 1)) {
                printf("|");
            } else if (x == 77 && (y == rocket_2_Y - 1 || y == rocket_2_Y || y == rocket_2_Y + 1)) {
                printf("|");
            } else if (x == ball_x && y == ball_y) {
                printf("o");
            } else if (x == 40) {
                printf("|");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    // Нижняя граница
    for (int x = 0; x < 80; ++x) {
        printf("-");
    }
    printf("\n");

    return 0;
}

int update_right_pad(int right_pad_y, char key_input, int height) {
    switch (key_input) {
        case 'k':
        case 'K':
            if (right_pad_y > 4) right_pad_y--;
            break;
        case 'm':
        case 'M':
            if (right_pad_y < height - 2) right_pad_y++;
            break;
    }
    return right_pad_y;
}

int update_left_pad(int left_pad_y, char key_input, int height) {
    switch (key_input) {
        case 'a':
        case 'A':
            if (left_pad_y > 4) left_pad_y--;
            break;
        case 'z':
        case 'Z':
            if (left_pad_y < height - 2) left_pad_y++;
            break;
    }
    return left_pad_y;
}

int change_vector_ball_x(int vector_ball_x, int ball_x, int ball_y, int rocket_1_Y, int rocket_2_Y) {
    if ((ball_x == 3 && (ball_y == rocket_1_Y - 1 || ball_y == rocket_1_Y || ball_y == rocket_1_Y + 1)) ||
        (ball_x == 76 && (ball_y == rocket_2_Y - 1 || ball_y == rocket_2_Y || ball_y == rocket_2_Y + 1))) {
        vector_ball_x = -vector_ball_x;
    }
    return vector_ball_x;
}

int change_vector_ball_y(int vector_ball_y, int ball_y) {
    if (ball_y <= 3 || ball_y >= 24) {
        vector_ball_y = -vector_ball_y;
    }
    return vector_ball_y;
}

int move_ball_x(int vector_ball_x, int ball_x) { return ball_x + vector_ball_x; }

int move_ball_y(int vector_ball_y, int ball_y) { return ball_y + vector_ball_y; }
