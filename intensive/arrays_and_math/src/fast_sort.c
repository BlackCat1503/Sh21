#include <stdio.h>

void quick_sort(int *array, int low, int high);
int partition(int *array, int low, int high);
void heap_sort(int *array, int n);
void heapify(int *array, int n, int i);

int main() {
    int array[10];
    int n = 10;

    // Чтение входных данных
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &array[i]) != 1) {
            printf("n/a\n");
            return 1;
        }
    }

    // Копируем массив для сортировки разными методами
    int array_for_heap_sort[10];
    for (int i = 0; i < n; i++) {
        array_for_heap_sort[i] = array[i];
    }

    // Быстрая сортировка
    quick_sort(array, 0, n - 1);
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Пирамидальная сортировка
    heap_sort(array_for_heap_sort, n);
    for (int i = 0; i < n; i++) {
        printf("%d ", array_for_heap_sort[i]);
    }
    printf("\n");

    return 0;
}

// Реализация быстрой сортировки
void quick_sort(int *array, int low, int high) {
    if (low < high) {
        int pi = partition(array, low, high);
        quick_sort(array, low, pi - 1);
        quick_sort(array, pi + 1, high);
    }
}

int partition(int *array, int low, int high) {
    int pivot = array[high];
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (array[j] < pivot) {
            i++;
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
    int temp = array[i + 1];
    array[i + 1] = array[high];
    array[high] = temp;
    return (i + 1);
}

// Реализация пирамидальной сортировки
void heap_sort(int *array, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(array, n, i);
    }
    for (int i = n - 1; i > 0; i--) {
        int temp = array[0];
        array[0] = array[i];
        array[i] = temp;
        heapify(array, i, 0);
    }
}

void heapify(int *array, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && array[left] > array[largest]) {
        largest = left;
    }
    if (right < n && array[right] > array[largest]) {
        largest = right;
    }
    if (largest != i) {
        int temp = array[i];
        array[i] = array[largest];
        array[largest] = temp;
        heapify(array, n, largest);
    }
}
