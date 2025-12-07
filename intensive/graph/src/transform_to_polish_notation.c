#include "transform_to_polish_notation.h"

int get_priority(char symb) {
    int res = -1;
    if (symb == '(') {
        res = BRACKET_OPEN_PRIORITY;
    } else if (symb == ')') {
        res = BRACKET_CLOSED_PRIORITY;
    } else if (symb == 'c' || symb == 's' || symb == 't' || symb == 'g' || symb == 'q' || symb == 'l') {
        res = FUNC;
    } else if (symb == '*' || symb == '/') {
        res = DIVISION;
    } else if (symb == '+' || symb == '-') {
        res = SUMM;
    }
    return res;
}

char* trasform_string_to_polish_notation(char* string_to_transform) {
    size_t size = strlen(string_to_transform);
    struct Stack* stack = NULL;
    char* string_polish = (char*)malloc(sizeof(char) * size * 5);
    int s = 0;
    char symb;
    for (int i = 0; i < (int)size; i++) {
        int priority;
        if (string_to_transform[i] == '.') {
            string_polish[s] = '.';
            s++;
        } else if (string_to_transform[i] != '+' && string_to_transform[i] != '-' &&
                   string_to_transform[i] != '/' && string_to_transform[i] != '*' &&
                   string_to_transform[i] != 'c' && string_to_transform[i] != ' ' &&
                   string_to_transform[i] != '(' && string_to_transform[i] != ')' &&
                   string_to_transform[i] != 's' && string_to_transform[i] != 't' &&
                   string_to_transform[i] != 'g' && string_to_transform[i] != 'q' &&
                   string_to_transform[i] != 'l') {
            string_polish[s] = string_to_transform[i];
            s++;
            if (((int)string_to_transform[i + 1] < 48 || (int)string_to_transform[i + 1] > 57) &&
                string_to_transform[i + 1] != '.') {
                string_polish[s] = ' ';
                s++;
            }
        } else if (string_to_transform[i] == ' ') {
        } else if (string_to_transform[i] != ')' && string_to_transform[i] != '(') {
            priority = get_priority(string_to_transform[i]);
            int flag = 1;
            while (stack && flag) {
                symb = pop(&stack);
                if (get_priority(symb) <= priority && get_priority(symb) != 0) {
                    string_polish[s] = symb;
                    s++;
                    string_polish[s] = ' ';
                    s++;
                } else {
                    push(&stack, symb);
                    flag = 0;
                }
            }
            push(&stack, string_to_transform[i]);
        } else if (string_to_transform[i] == '(') {
            push(&stack, string_to_transform[i]);
        } else {
            while (stack->symb != '(') {
                symb = pop(&stack);
                if (get_priority(symb) != 1) {
                    string_polish[s] = symb;
                    s++;
                    string_polish[s] = ' ';
                    s++;
                }
            }
            pop(&stack);
        }
    }
    while (stack) {
        symb = pop(&stack);
        string_polish[s] = symb;
        s++;
        string_polish[s] = ' ';
        s++;
    }
    string_polish[s] = '\0';
    destroy(stack);
    return string_polish;
}

float evaluatePolishNotation(char* expr, float x) {
    float* stack = (float*)malloc(strlen(expr) * sizeof(char));
    int cur_elems = 0;
    char arr[32];
    for (int i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == ' ') {
        } else if ((int)expr[i] >= 48 && (int)expr[i] <= 57) {
            int arr_index = 0;
            while (expr[i] != ' ' && expr[i] != '\0') {
                arr[arr_index++] = expr[i++];
            }
            arr[arr_index] = '\0';
            stack[cur_elems] = atof(arr);
            cur_elems += 1;
            i--;
        } else if (expr[i] == 'x') {
            stack[cur_elems] = x;
            cur_elems += 1;
        } else if (expr[i] == 'c') {
            stack[cur_elems - 1] = cos(stack[cur_elems - 1]);
        } else if (expr[i] == 's') {
            stack[cur_elems - 1] = sin(stack[cur_elems - 1]);
        } else if (expr[i] == 't') {
            stack[cur_elems - 1] = tan(stack[cur_elems - 1]);
        } else if (expr[i] == 'g') {
            stack[cur_elems - 1] = 1 / tan(stack[cur_elems - 1]);
        } else if (expr[i] == 'q') {
            stack[cur_elems - 1] = sqrt(stack[cur_elems - 1]);
        } else if (expr[i] == 'l') {
            stack[cur_elems - 1] = log(stack[cur_elems - 1]);
        } else if (expr[i] == '*') {
            stack[cur_elems - 2] = stack[cur_elems - 2] * stack[cur_elems - 1];
            cur_elems -= 1;
        } else if (expr[i] == '+') {
            stack[cur_elems - 2] = stack[cur_elems - 2] + stack[cur_elems - 1];
            cur_elems -= 1;
        } else if (expr[i] == '-') {
            stack[cur_elems - 2] = stack[cur_elems - 2] - stack[cur_elems - 1];
            cur_elems -= 1;
        } else if (expr[i] == '/') {
            stack[cur_elems - 2] = stack[cur_elems - 2] / stack[cur_elems - 1];
            cur_elems -= 1;
        }
    }
    float tmp = stack[0];
    free(stack);
    return tmp;
}