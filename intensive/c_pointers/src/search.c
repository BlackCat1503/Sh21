#include <math.h>
#include <stdio.h>

#define NMAX 30

// Функция для ввода массива
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

// Функция для вычисления математического ожидания
double mean(int *a, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum / n;
}

// Функция для вычисления дисперсии
double variance(int *a, int n, double mean_value) {
    double var_sum = 0.0;
    for (int i = 0; i < n; i++) {
        var_sum += (a[i] - mean_value) * (a[i] - mean_value);
    }
    return var_sum / n;  // Возвращаем дисперсию
}

// Функция для поиска подходящего числа
int find_valid_number(int *a, int n, double mean_value, double std_dev) {
    for (int i = 0; i < n; i++) {
        if (a[i] % 2 == 0 && a[i] >= mean_value && a[i] <= mean_value + 3 * std_dev && a[i] != 0) {
            return a[i];  // Возвращаем первое подходящее число
        }
    }
    return 0;  // Если не найдено подходящее число
}

int main() {
    int n, data[NMAX];

    if (input(data, &n)) {
        double mean_value = mean(data, n);
        double var = variance(data, n, mean_value);
        double std_dev = sqrt(var);  // Стандартное отклонение

        int result = find_valid_number(data, n, mean_value, std_dev);

        printf("%d\n", result);
    } else {
        printf("n/a\n");
        return 1;  // Завершение программы с ошибкой
    }
    return 0;
}
