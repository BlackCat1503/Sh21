#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct Stack {
    char symb;
    struct Stack* next;
};

struct Stack* init(char s);
char pop(struct Stack** st);
void push(struct Stack** st, char symb);
void destroy(struct Stack* st);
