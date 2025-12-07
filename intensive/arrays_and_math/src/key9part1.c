#include <stdio.h>

#define NMAX 10

int input(int *buffer, int *length);
void output(int *buffer, int length);
int sum_numbers(int *buffer, int length);
int find_numbers(int *buffer, int length, int number, int *numbers);

/*------------------------------------
 Функция получает массив данных
 через stdin.
 Выдает в stdout особую сумму
 и сформированный массив
 специальных элементов
 (выбранных с помощью найденной суммы):
 это и будет частью ключа
-------------------------------------*/
int main() {
    int n, data[NMAX];

    if (input(data, &n)) {
        int sum = sum_numbers(data, n);
        if (sum == 0) {
            printf("n/a\n");
            return 1;
        }

        int result[NMAX];
        int result_length = find_numbers(data, n, sum, result);

        if (result_length > 0) {
            printf("%d\n", sum);
            output(result, result_length);
        } else {
            printf("n/a\n");
        }
    } else {
        printf("n/a\n");
        return 1;
    }

    return 0;
}

/*------------------------------------
 Функция должна находить
 сумму четных элементов
 с 0-й позиции.
-------------------------------------*/
int sum_numbers(int *buffer, int length) {
    int sum = 0;
    for (int i = 0; i < length; i++) {  // Изменено на i < length
        if (buffer[i] % 2 == 0) {       // Проверка на четность
            sum += buffer[i];
        }
    }
    return sum;
}

/*------------------------------------
 Функция должна находить
 все элементы, на которые нацело
 делится переданное число и
 записывает их в выходной массив.
-------------------------------------*/
int find_numbers(int *buffer, int length, int number, int *numbers) {
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (buffer[i] != 0 && number % buffer[i] == 0) {  // Проверка делимости
            numbers[count++] = buffer[i];
        }
    }
    return count;  // Возвращаем количество найденных элементов
}

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
