#include "replace_func.h"

#define MAX_EXPRESSION_LENGTH 1024
#define MAX_FUNCTIONS 6

void replace_function(char *source, const char *search, const char *replace) {
    char buffer[MAX_EXPRESSION_LENGTH];
    char *insert_point = buffer;
    const char *tmp = source;
    size_t search_len = strlen(search);
    size_t replace_len = strlen(replace);

    while (1) {
        const char *p = strstr(tmp, search);
        if (p == NULL) {
            strcpy(insert_point, tmp);
            break;
        }
        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;
        memcpy(insert_point, replace, replace_len);
        insert_point += replace_len;
        tmp = p + search_len;
    }
    strcpy(source, buffer);
}

void shorten_functions(char *expression) {
    const char *valid_functions[MAX_FUNCTIONS] = {"sin(", "cos(", "tan(", "ctg(", "sqrt(", "ln("};
    const char *shortened_functions[MAX_FUNCTIONS] = {"s(", "c(", "t(", "g(", "q(", "l("};

    for (size_t i = 0; i < MAX_FUNCTIONS; i++) {
        replace_function(expression, valid_functions[i], shortened_functions[i]);
    }
}

int check_brackets(const char *expression) {
    int balance = 0;
    for (size_t i = 0; expression[i] != '\0'; ++i) {
        if (expression[i] == '(') {
            ++balance;
        } else if (expression[i] == ')') {
            --balance;
        }
        if (balance < 0) {
            return 0;
        }
    }
    return balance == 0;
}

int is_digit(char ch) { return ch >= '0' && ch <= '9'; }

int check_numbers_and_operators(const char *expression) {
    size_t i = 0;
    int last_was_operator = 1;
    while (expression[i] != '\0') {
        while (expression[i] == ' ') {
            i++;
        }
        if (is_digit(expression[i])) {
            while (is_digit(expression[i])) {
                i++;
            }
            if (expression[i] == '.') {
                i++;
                if (!is_digit(expression[i])) {
                    return 0;
                }
                while (is_digit(expression[i])) {
                    i++;
                }
            }
            last_was_operator = 0;
        } else if ((expression[i] >= 'a' && expression[i] <= 'z') ||
                   (expression[i] >= 'A' && expression[i] <= 'Z')) {
            while ((expression[i] >= 'a' && expression[i] <= 'z') ||
                   (expression[i] >= 'A' && expression[i] <= 'Z')) {
                i++;
            }
            if (expression[i] == '(') {
                last_was_operator = 1;
            } else {
                last_was_operator = 0;
            }
        } else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' ||
                   expression[i] == '/') {
            if (last_was_operator) {
                return 0;
            }
            last_was_operator = 1;
            i++;
        } else if (expression[i] == '(' || expression[i] == ')') {
            i++;
        } else {
            return 0;
        }
    }
    return !last_was_operator;
}

int process_expression(char *expression) {
    printf("Введите математическое выражение: ");
    fgets(expression, MAX_EXPRESSION_LENGTH, stdin);
    size_t len = strlen(expression);
    if (len > 0 && expression[len - 1] == '\n') {
        expression[len - 1] = '\0';
    }

    if (!check_brackets(expression)) {
        printf("Ошибка: некорректные скобки в выражении.\n");
        return 1;
    }

    if (!check_numbers_and_operators(expression)) {
        printf("Ошибка: некорректный ввод чисел или операторов в выражении.\n");
        return 1;
    }

    shorten_functions(expression);
    return 0;
}
