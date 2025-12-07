#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_list.h"

StrList *init_list() {
    StrList *list = malloc(sizeof(StrList));
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
    return list;
}

void free_list(StrList *list) {
    while (!empty_list(list)) {
        pop_front(list);
    }
    list->size = 0;
    free(list);
}

bool empty_list(StrList *list) { return list->size == 0; }

void push_back(StrList *list, char *str) {
    Node *p = malloc(sizeof(Node));
    p->data = strdup(str);
    p->next = NULL;
    if (list->first == NULL) {
        list->first = p;
        list->last = p;
    } else {
        list->last->next = p;
        list->last = p;
    }
    list->size++;
}

void pop_front(StrList *list) {
    assert(!empty_list(list));
    Node *node = list->first;
    Node *next = node->next;
    list->first = next;
    if (list->last == node)
        list->last = next;
    free(node->data);
    free(node);
    list->size--;
}

void debug_print(StrList *list) {
#ifdef DEBUG
    printf("[ ");
    for (Node *p = list->first; p != NULL; p = p->next) {
        printf("'%s', ", p->data);
    }
    printf("]\n");
#else
    (void)list;
#endif
}
