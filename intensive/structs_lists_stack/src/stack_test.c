#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

int test_push_pop() {
    struct stack *s = init(5);
    push(s, 1);
    push(s, 2);
    push(s, 3);
    if (pop(s) != 3) {
        return EXIT_FAILURE;
    }
    if (pop(s) != 2) {
        return EXIT_FAILURE;
    }
    if (pop(s) != 1) {
        return EXIT_FAILURE;
    }
    destroy(s);
    return EXIT_SUCCESS;
}

int test_pop_empty() {
    struct stack *s = init(5);
    if (pop(s) != -1) {
        printf("Error: pop(s) != -1\n");
        return EXIT_FAILURE;
    }
    destroy(s);
    return EXIT_SUCCESS;
}

int main() {
    int result = EXIT_SUCCESS;
    result |= test_push_pop();
    result |= test_pop_empty();
    if (result == EXIT_SUCCESS) {
        printf("SUCCESS\n");
    } else {
        printf("FAIL\n");
    }
    return result;
}