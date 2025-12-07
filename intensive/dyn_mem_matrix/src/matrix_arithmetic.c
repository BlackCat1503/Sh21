#include <stdio.h>
#include <stdlib.h>

void printMatrix(int **matrix, int height, int width);
void freeMatrix(int **matrix, int height);
int **allocateMatrix(int height, int width);
int **addMatrices(int **matrix1, int **matrix2, int height, int width);
int **multiplyMatrices(int **matrix1, int **matrix2, int height1, int width1, int width2);
int **transposeMatrix(int **matrix, int height, int width);

int main() {
    int operation, height1, width1, height2, width2;
    scanf("%d", &operation);

    if (operation == 1) {  // Сложение
        scanf("%d %d", &height1, &width1);
        int **matrix1 = allocateMatrix(height1, width1);
        if (!matrix1) {
            printf("n/a\n");
            return 0;
        }

        for (int i = 0; i < height1; i++) {
            for (int j = 0; j < width1; j++) {
                if (scanf("%d", &matrix1[i][j]) != 1) {
                    freeMatrix(matrix1, height1);
                    printf("n/a\n");
                    return 0;
                }
            }
        }

        scanf("%d %d", &height2, &width2);
        if (height1 != height2 || width1 != width2) {
            freeMatrix(matrix1, height1);
            printf("n/a\n");
            return 0;
        }

        int **matrix2 = allocateMatrix(height2, width2);
        if (!matrix2) {
            freeMatrix(matrix1, height1);
            printf("n/a\n");
            return 0;
        }

        for (int i = 0; i < height2; i++) {
            for (int j = 0; j < width2; j++) {
                if (scanf("%d", &matrix2[i][j]) != 1) {
                    freeMatrix(matrix1, height1);
                    freeMatrix(matrix2, height2);
                    printf("n/a\n");
                    return 0;
                }
            }
        }

        int **result = addMatrices(matrix1, matrix2, height1, width1);
        printMatrix(result, height1, width1);

        freeMatrix(matrix1, height1);
        freeMatrix(matrix2, height2);
        freeMatrix(result, height1);

    } else if (operation == 2) {  // Умножение
        scanf("%d %d", &height1, &width1);
        int **matrix1 = allocateMatrix(height1, width1);
        if (!matrix1) {
            printf("n/a\n");
            return 0;
        }

        for (int i = 0; i < height1; i++) {
            for (int j = 0; j < width1; j++) {
                if (scanf("%d", &matrix1[i][j]) != 1) {
                    freeMatrix(matrix1, height1);
                    printf("n/a\n");
                    return 0;
                }
            }
        }

        scanf("%d %d", &height2, &width2);
        if (width1 != height2) {
            freeMatrix(matrix1, height1);
            printf("n/a\n");
            return 0;
        }

        int **matrix2 = allocateMatrix(height2, width2);
        if (!matrix2) {
            freeMatrix(matrix1, height1);
            printf("n/a\n");
            return 0;
        }

        for (int i = 0; i < height2; i++) {
            for (int j = 0; j < width2; j++) {
                if (scanf("%d", &matrix2[i][j]) != 1) {
                    freeMatrix(matrix1, height1);
                    freeMatrix(matrix2, height2);
                    printf("n/a\n");
                    return 0;
                }
            }
        }

        int **result = multiplyMatrices(matrix1, matrix2, height1, width1, width2);
        printMatrix(result, height1, width2);

        freeMatrix(matrix1, height1);
        freeMatrix(matrix2, height2);
        freeMatrix(result, height1);

    } else if (operation == 3) {  // Транспонирование
        scanf("%d %d", &height1, &width1);
        int **matrix = allocateMatrix(height1, width1);
        if (!matrix) {
            printf("n/a\n");
            return 0;
        }

        for (int i = 0; i < height1; i++) {
            for (int j = 0; j < width1; j++) {
                if (scanf("%d", &matrix[i][j]) != 1) {
                    freeMatrix(matrix, height1);
                    printf("n/a\n");
                    return 0;
                }
            }
        }

        int **result = transposeMatrix(matrix, height1, width1);
        printMatrix(result, width1, height1);

        freeMatrix(matrix, height1);
        freeMatrix(result, width1);
    } else {
        printf("n/a\n");
    }

    return 0;
}

int **allocateMatrix(int height, int width) {
    int **matrix = malloc(height * sizeof(int *));
    if (!matrix) return NULL;

    for (int i = 0; i < height; i++) {
        matrix[i] = malloc(width * sizeof(int));
        if (!matrix[i]) {
            freeMatrix(matrix, i);
            return NULL;
        }
    }
    return matrix;
}

void freeMatrix(int **matrix, int height) {
    for (int i = 0; i < height; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int **addMatrices(int **matrix1, int **matrix2, int height, int width) {
    int **result = allocateMatrix(height, width);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    return result;
}

int **multiplyMatrices(int **matrix1, int **matrix2, int height1, int width1, int width2) {
    int **result = allocateMatrix(height1, width2);
    for (int i = 0; i < height1; i++) {
        for (int j = 0; j < width2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < width1; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}

int **transposeMatrix(int **matrix, int height, int width) {
    int **result = allocateMatrix(width, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

void printMatrix(int **matrix, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%d", matrix[i][j]);
            if (j != width - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
}
