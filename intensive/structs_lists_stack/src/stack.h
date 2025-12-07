#ifndef STACK_H
#define STACK_H

#define SUCCESS 0
#define FAIL 1

struct stack {
    int *data;
    int size;
    int capacity;
};

struct stack *init(int size);
void push(struct stack *s, int value);
int pop(struct stack *s);
void destroy(struct stack *s);

#endif