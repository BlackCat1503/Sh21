#include "stack.h"

struct Stack* init(char s) {
    struct Stack* st = (struct Stack*)malloc(sizeof(struct Stack));
    st->symb = s;
    st->next = NULL;
    return st;
}

char pop(struct Stack** st) {
    char tmp = (*st)->symb;
    struct Stack* temp = *st;
    *st = (temp)->next;
    free(temp);
    return tmp;
}

void push(struct Stack** st, char symb) {
    struct Stack* newNode = init(symb);
    if (*st == NULL) {
        *st = newNode;
    } else {
        newNode->next = *st;
        *st = newNode;
    }
}

void destroy(struct Stack* st) {
    while (st != NULL) {
        destroy(st->next);
        free(st);
    }
}