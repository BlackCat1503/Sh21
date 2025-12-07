#include <stdio.h>

#define MAX_LENGTH 100

// Функции для работы с длинной арифметикой
int read_input(int *buffer, int *length);
int align_arrays(int *array1, int *len1, int *array2, int *len2);
void shift_array(int *array, int *current_length, int new_length);
void add_arrays(int *array1, int len1, int *array2, int *result, int *result_length);
int subtract_arrays(int *array1, int len1, int *array2, int *result, int *result_length);
void display_results(int *sum_result, int sum_length, int *sub_result, int sub_length, int sub_error);
void display_buffer(int *buffer, int length);

int main() {
    int num1[MAX_LENGTH], num2[MAX_LENGTH], len1 = 0, len2 = 0;

    // Чтение двух длинных чисел
    if (!read_input(num1, &len1) && !read_input(num2, &len2)) {
        int sum_result[MAX_LENGTH + 1], sum_length = 0;
        int sub_result[MAX_LENGTH], sub_length = 0, sub_error = 0;

        // Выравнивание массивов для сложения и вычитания
        sub_error = align_arrays(num1, &len1, num2, &len2);

        // Сложение двух длинных чисел
        add_arrays(num1, len1, num2, sum_result, &sum_length);

        // Вычитание, если нет ошибки
        if (sub_error == 0) {
            sub_error = subtract_arrays(num1, len1, num2, sub_result, &sub_length);
        }

        // Вывод результатов
        display_results(sum_result, sum_length, sub_result, sub_length, sub_error);
    }
    return 0;
}

// Функция для чтения ввода
int read_input(int *buffer, int *length) {
    int input_error = 0, count = 0;
    char ch;

    do {
        if (scanf("%d", &buffer[*length])) {
            count++;
            ch = getchar();
            // Проверка на корректность ввода
            if ((ch != '\n' && ch != ' ') || buffer[*length] < 0 || buffer[*length] > 9 ||
                count > MAX_LENGTH) {
                input_error = 1;
            }
            // Увеличение длины массива
            if (buffer[*length] != 0 || *length != 0) {
                (*length)++;
            }
        } else {
            input_error = 1;
        }
        if (input_error == 1) {
            break;
        }
    } while (ch != '\n');

    if (input_error == 1) {
        printf("n/a");
    }
    return input_error;
}

// Функция для выравнивания массивов
int align_arrays(int *array1, int *len1, int *array2, int *len2) {
    int sub_error = 0;
    if (*len2 > *len1) {
        sub_error = 1;
        shift_array(array1, len1, *len2);
    } else if (*len1 > *len2) {
        shift_array(array2, len2, *len1);
    }
    return sub_error;
}

// Функция для сдвига массива
void shift_array(int *array, int *current_length, int new_length) {
    int i;
    // Сдвиг элементов массива
    for (i = (*current_length) - 1; i >= 0; i--) {
        array[i + new_length - (*current_length)] = array[i];
    }
    // Заполнение начальных элементов нулями
    for (i = 0; i < new_length - (*current_length); i++) {
        array[i] = 0;
    }
    *current_length = new_length;
}

// Функция для сложения двух массивов
void add_arrays(int *array1, int len1, int *array2, int *result, int *result_length) {
    int carry = 0, i;
    // Сложение с учетом переноса
    for (i = len1 - 1; i >= 0; i--) {
        result[i + 1] = (array1[i] + array2[i] + carry) % 10;
        carry = (array1[i] + array2[i] + carry) / 10;
    }
    result[0] = carry;

    // Установка длины результата
    if (result[0] == 0) {
        // Если нет переноса, сдвигаем результат
        for (i = 0; i < len1; i++) {
            result[i] = result[i + 1];
        }
        *result_length = len1;  // Длина результата равна len1
    } else {
        *result_length = len1 + 1;  // Длина результата равна len1 + 1
    }
}

// Функция для вычитания двух массивов
int subtract_arrays(int *array1, int len1, int *array2, int *result, int *result_length) {
    int sub_error = 0;
    int borrow = 0, i;

    // Вычитание с учетом займа
    for (i = len1 - 1; i >= 0; i--) {
        if (array1[i] - borrow >= array2[i]) {
            result[i] = array1[i] - borrow - array2[i];
            borrow = 0;
        } else {
            result[i] = 10 + array1[i] - borrow - array2[i];
            borrow = 1;
        }
    }
    *result_length = len1;

    // Удаление начальных нулей
    if (borrow == 0) {
        int count = 0;
        while (result[count] == 0) {
            count++;
        }
        for (i = 0; i < (*result_length) - count; i++) {
            result[i] = result[i + count];
        }
        *result_length -= count;
    } else {
        sub_error = 1;  // Ошибка: отрицательный результат
    }
    return sub_error;
}

// Функция для вывода результатов
void display_results(int *sum_result, int sum_length, int *sub_result, int sub_length, int sub_error) {
    display_buffer(sum_result, sum_length);
    printf("\n");
    if (sub_error == 0) {
        display_buffer(sub_result, sub_length);
    } else {
        printf("n/a");
    }
}

// Функция для вывода буфера
void display_buffer(int *buffer, int length) {
    int i;
    // Найдем первый ненулевой элемент
    int start_index = 0;
    while (start_index < length && buffer[start_index] == 0) {
        start_index++;
    }

    // Если все элементы нулевые (например, результат 0)
    if (start_index == length) {
        printf("0");
    } else {
        for (i = start_index; i < length - 1; i++) {
            printf("%d ", buffer[i]);
        }
        printf("%d", buffer[i]);
    }
}
