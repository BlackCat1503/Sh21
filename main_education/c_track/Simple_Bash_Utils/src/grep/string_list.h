#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct Node {
    char *data;
    struct Node *next;
} Node;

typedef struct Node Node;

typedef struct StrList {
    Node *first;
    Node *last;
    size_t size;
} StrList;

StrList *init_list();
void free_list(StrList *list);
bool empty_list(StrList *list);
void push_back(StrList *list, char *str);
void pop_front(StrList *list);
void debug_print(StrList *list);
