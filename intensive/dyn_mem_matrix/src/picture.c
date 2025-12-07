#include <stdio.h>

#define HEIGHT 15
#define WIDTH 13

// Инициализация матрицы
void initializeMatrix(int* buffer, int** matrix, int rows, int cols);
// Функция для рисования картинки на холсте
void drawPicture(int** canvas, int rows, int cols);
// Очистка холста (заполнение нулями)
void clearCanvas(int** canvas, int rows, int cols);
// Вывод холста на экран
void printCanvas(int** canvas, int rows, int cols);

int main() {
    int canvas_data[HEIGHT][WIDTH];  // Данные холста (матрица чисел)
    int* canvas[HEIGHT];             // Указатели на строки холста

    initializeMatrix((int*)canvas_data, canvas, HEIGHT, WIDTH);  // Инициализация матрицы
    drawPicture(canvas, HEIGHT, WIDTH);  // Рисование картинки на холсте
    printCanvas(canvas, HEIGHT, WIDTH);  // Вывод холста на экран

    return 0;
}

// Вывод холста на экран
void printCanvas(int** canvas, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d", canvas[i][j]);  // Вывод текущего элемента холста
            if (j != cols - 1) printf(" ");  // Вывод пробела, если это не последний столбец
        }
        if (i != rows - 1) printf("\n");  // Вывод перевода строки, если это не последняя строка
    }
}

// Рисование картинки на холсте
void drawPicture(int** canvas, int rows, int cols) {
    int frame_horizontal[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};  // Горизонтальная рамка
    int frame_vertical[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};  // Вертикальная рамка
    int tree_trunk[] = {7, 7, 7, 7};                                       // Ствол дерева
    int tree_foliage[] = {3, 3, 3, 3};                                     // Листва дерева
    int sun_data[3][3] = {{6, 6, 6}, {0, 6, 6}, {6, 0, 6}};                // Солнце

    clearCanvas(canvas, rows, cols);  // Очистка холста перед рисованием

    // Рисование рамки
    for (int i = 0; i < 13; i++) {
        canvas[0][i] = frame_horizontal[i];  // Верхняя горизонтальная рамка
        canvas[HEIGHT - 1][i] = frame_horizontal[i];  // Нижняя горизонтальная рамка
        canvas[HEIGHT / 2][i] = frame_horizontal[i];  // Средняя горизонтальная рамка
    }
    for (int i = 0; i < 15; i++) {
        canvas[i][0] = frame_vertical[i];          // Левая вертикальная рамка
        canvas[i][WIDTH - 1] = frame_vertical[i];  // Правая вертикальная рамка
        canvas[i][WIDTH / 2] = frame_vertical[i];  // Средняя вертикальная рамка
    }

    // Рисование ствола дерева
    for (int i = 0; i < 4; i++) {
        canvas[10][2 + i] = tree_trunk[i];  // Вертикальный ствол
        canvas[6 + i][3] = tree_trunk[i];   // Левая часть ствола
        canvas[6 + i][4] = tree_trunk[i];   // Правая часть ствола
    }

    // Рисование листвы дерева
    for (int i = 0; i < 4; i++) {
        canvas[3][2 + i] = tree_foliage[i];  // Верхняя часть листвы
        canvas[4][2 + i] = tree_foliage[i];  // Нижняя часть листвы
        canvas[2 + i][3] = tree_foliage[i];  // Левая часть листвы
        canvas[2 + i][4] = tree_foliage[i];  // Правая часть листвы
    }

    // Рисование солнца
    for (int i = 0; i < 3; i++) {
        canvas[1][7 + i] = sun_data[1][i];  // Верхняя часть солнца
        canvas[4][7 + i] = sun_data[1][i];  // Нижняя часть солнца
        canvas[1][9 + i] = sun_data[0][i];  // Левая часть солнца
    }
    for (int i = 0; i < 3; i++) {
        canvas[4 - i][9] = sun_data[1][i];   // Левая часть солнца
        canvas[4 - i][10] = sun_data[1][i];  // Правая часть солнца
        canvas[4 - i][11] = sun_data[0][i];  // Левая часть солнца
    }
}

// Очистка холста (заполнение нулями)
void clearCanvas(int** canvas, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            canvas[i][j] = 0;  // Заполнение текущего элемента холста нулём
        }
    }
}

// Инициализация матрицы
void initializeMatrix(int* buffer, int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        matrix[i] = buffer + i * cols;  // Указатели на строки матрицы
    }
}
