#include "stack.h"

#include <stdlib.h>

struct stack *init(int size) {
    struct stack *s = malloc(sizeof(struct stack));
    s->data = malloc(size * sizeof(int));
    s->size = 0;
    s->capacity = size;
    return s;
}

void push(struct stack *s, int value) {
    if (s->size == s->capacity) {
        s->capacity *= 2;
        s->data = realloc(s->data, s->capacity * sizeof(int));
    }
    s->data[s->size] = value;
    s->size++;
}

int pop(struct stack *s) {
    if (s->size == 0) {
        return -1;  // or any other error code
    }
    s->size--;
    return s->data[s->size];
}

void destroy(struct stack *s) {
    free(s->data);
    free(s);
}
