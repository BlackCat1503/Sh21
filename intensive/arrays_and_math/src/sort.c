#include <stdio.h>
#define ASIZE 10

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
    printf("\n");
}

int main() {
    int arr[ASIZE];

    if (!read_array(arr, ASIZE)) {
        printf("n/a\n");
        return 1;
    }

    bubble_sort(arr, ASIZE);
    print_array(arr, ASIZE);

    return 0;
}
