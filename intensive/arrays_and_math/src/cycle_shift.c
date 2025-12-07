#include <stdio.h>

#define NMAX 10

int input(int *buffer, int *length, int *shift);
void output(int *buffer, int length);
void cycle_shift(int *buffer, int length, int shift);

int main() {
    int n, data[NMAX], c;
    if (input(data, &n, &c)) {
        cycle_shift(data, n, c);
        output(data, n);
    } else {
        printf("n/a\n");
        return 1;
    }
    return 0;
}

int input(int *a, int *n, int *shift) {
    // Чтение длины массива
    if (scanf("%d", n) != 1 || *n <= 0 || *n > NMAX) {
        return 0;
    }

    // Чтение элементов массива
    for (int i = 0; i < *n; i++) {
        if (scanf("%d", &a[i]) != 1) {
            return 0;
        }
    }

    // Чтение сдвига и проверка на целое число
    float temp_shift;
    if (scanf("%f", &temp_shift) != 1 || (int)temp_shift != temp_shift) {
        return 0;  // Если не удалось считать или сдвиг не целый
    }
    *shift = (int)temp_shift;  // Преобразуем в целое

    return 1;
}

void cycle_shift(int *buffer, int length, int shift) {
    if (length == 0) return;  // Если массив пустой, ничего не делаем
    shift = shift % length;   // Нормализация сдвига
    if (shift < 0) {
        shift += length;  // Преобразуем отрицательный сдвиг в положительный
    }

    int temp[NMAX];
    for (int i = 0; i < length; i++) {
        temp[i] = buffer[(i + shift) % length];
    }

    for (int i = 0; i < length; i++) {
        buffer[i] = temp[i];
    }
}

void output(int *a, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}
