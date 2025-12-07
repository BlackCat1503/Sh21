#include <math.h>
#include <stdio.h>

#define NMAX 10

int input(int *a, int *n) {
    if (scanf("%d", n) != 1 || *n <= 0 || *n > NMAX) {
        return 0;
    } else {
        for (int i = 0; i < *n; i++) {
            if (scanf("%d", &a[i]) != 1) {
                return 0;
            }
        }
        int temp;
        if ((temp = getchar()) != '\n') {
            return 0;
        }
    }
    return 1;
}

void output(int *a, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");  // Перевод строки после вывода
}

int max(int *a, int n) {
    int max_value = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] > max_value) {
            max_value = a[i];
        }
    }
    return max_value;
}

int min(int *a, int n) {
    int min_value = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] < min_value) {
            min_value = a[i];
        }
    }
    return min_value;
}

double mean(int *a, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum / n;
}

double variance(int *a, int n) {
    double mean_value = mean(a, n);
    double var_sum = 0.0;

    for (int i = 0; i < n; i++) {
        var_sum += (a[i] - mean_value) * (a[i] - mean_value);
    }

    return var_sum / n;  // Дисперсия для дискретного равномерного распределения
}

void output_result(int max_v, int min_v, double mean_v, double variance_v) {
    printf("%d %d %.6f %.6f", max_v, min_v, mean_v, variance_v);
}

int main() {
    int n, data[NMAX];

    if (input(data, &n)) {
        output(data, n);
        output_result(max(data, n), min(data, n), mean(data, n), variance(data, n));
    } else {
        printf("n/a\n");
        return 1;  // Завершение программы с ошибкой
    }

    return 0;
}