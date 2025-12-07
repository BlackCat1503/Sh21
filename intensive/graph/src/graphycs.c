#include "graphycs.h"

#include "transform_to_polish_notation.h"
#define rows 25
#define cols 80

void printf_field(char* string, double Xmin, double Xmax, double Ymin, double Ymax) {
    char matrix[rows][cols];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = '.';
        }
    }

    for (int i = 0; i < cols; i++) {
        double x = Xmin + (Xmax - Xmin) * i / (cols - 1);
        double m = evaluatePolishNotation(string, x);
        int j = (int)((m - Ymin) / (Ymax - Ymin) * (rows - 1));
        int yplot = rows - 1 - j;
        if (yplot >= 0 && yplot < rows) {
            matrix[yplot][i] = '*';
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c", matrix[i][j]);
        }
        printf("\n");
    }
    free(string);
}
