#include <stdio.h>
#include <stdlib.h>

#define MAX_WIDTH 100
#define MAX_HEIGHT 100

void printMatrix(int **matrix, int height, int width);
void freeMemory(int allocationType, int **matrix, int height);
int **allocateMatrix(int allocationType, int *status, int height, int width);
int readMatrix(int **matrix, int height, int width);
int readInput(int *allocationType, int *height, int *width);

int main() {
    int **matrix = NULL;
    int allocationType, height, width, status = 1;

    status = readInput(&allocationType, &height, &width);
    if (status == 1) {
        matrix = allocateMatrix(allocationType, &status, height, width);
    }
    if (status == 1 && matrix != NULL) {
        status = readMatrix(matrix, height, width);
    }
    if (status == 1) {
        printMatrix(matrix, height, width);
    } else {
        printf("n/a\n");
    }

    freeMemory(allocationType, matrix, height);
    return status;
}

int readInput(int *allocationType, int *height, int *width) {
    return scanf("%d %d %d", allocationType, width, height) == 3 &&
           (*allocationType >= 1 && *allocationType <= 4) && (*width >= 1 && *height >= 1);
}

int **allocateMatrix(int allocationType, int *status, int height, int width) {
    int **matrix = NULL;

    switch (allocationType) {
        case 1: {  // Статическое выделение
            static int staticMatrix[MAX_HEIGHT][MAX_WIDTH];
            static int *rowPointers[MAX_HEIGHT];
            for (int i = 0; i < height; i++) {
                rowPointers[i] = staticMatrix[i];
            }
            matrix = rowPointers;
            break;
        }

        case 2: {  // Динамическое выделение с помощью массива указателей на массивы
            matrix = malloc(height * sizeof(int *));
            if (matrix) {
                for (int i = 0; i < height; i++) {
                    matrix[i] = malloc(width * sizeof(int));
                    if (!matrix[i]) {
                        *status = 0;
                        break;
                    }
                }
            } else {
                *status = 0;
            }
            break;
        }

        case 3: {  // Динамическое выделение с помощью массива указателей на сегменты
                   // массива
            matrix = malloc(height * sizeof(int *));
            int *array = malloc(height * width * sizeof(int));
            if (matrix && array) {
                for (int i = 0; i < height; i++) {
                    matrix[i] = &array[i * width];
                }
            } else {
                *status = 0;
                free(array);
            }
            break;
        }

        case 4: {  // Динамическое выделение с помощью массива указателей на сегменты
                   // второго массива
            matrix = malloc((height + 1) * sizeof(int *));
            int *dataBuffer = malloc(height * width * sizeof(int));
            if (matrix && dataBuffer) {
                for (int i = 0; i < height; i++) {
                    matrix[i] = &dataBuffer[i * width];
                }
            } else {
                *status = 0;
                free(dataBuffer);
            }
            break;
        }

        default:
            *status = 0;
            break;
    }
    return matrix;
}

int readMatrix(int **matrix, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void printMatrix(int **matrix, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%d", matrix[i][j]);
            if (j != width - 1) printf(" ");
        }
        if (i != height - 1) printf("\n");
    }
}

void freeMemory(int allocationType, int **matrix, int height) {
    if (allocationType == 2) {
        for (int i = 0; i < height; i++) {
            free(matrix[i]);
        }
    }
    free(matrix);
}
