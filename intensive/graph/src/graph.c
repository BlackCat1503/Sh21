#include "graphycs.h"
#include "replace_func.h"
#include "transform_to_polish_notation.h"

#define MAX_EXPRESSION_LENGTH 1024
#define MAX_FUNCTIONS 6

int main() {
    char expression[MAX_EXPRESSION_LENGTH];
    char *string = expression;
    if (process_expression(expression) == 0) {
        string = trasform_string_to_polish_notation(string);
        printf_field(string, 0, 4 * M_PI, 1, -1);
    }
    return 0;
}
