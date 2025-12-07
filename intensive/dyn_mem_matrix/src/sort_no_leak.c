#include <stdio.h>
#include <stdlib.h>

int read_array(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            return 0;
        }
    }
    return 1;
}

void bubble_sort(int *arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void print_array(const int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) {
            printf(" ");
        }
    }
}

int main() {
    int n;

    // Чтение длины массива
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("n/a");
        return 1;
    }

    // Выделение памяти для массива
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("n/a");
        return 1;
    }

    // Чтение массива
    if (!read_array(arr, n)) {
        free(arr);  // Освобождение памяти
        printf("n/a");
        return 1;
    }

    // Сортировка и вывод
    bubble_sort(arr, n);
    print_array(arr, n);

    free(arr);  // Освобождение памяти
    return 0;
}