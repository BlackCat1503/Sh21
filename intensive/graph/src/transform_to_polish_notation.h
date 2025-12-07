#ifndef TRANSFORM
#define TRANSFORM

#include <math.h>
#include <string.h>

#include "stack.h"

int get_priority(char symb);
char* trasform_string_to_polish_notation(char* string_to_transform);
float evaluatePolishNotation(char* expr, float x);

#define BRACKET_OPEN_PRIORITY 0
#define BRACKET_CLOSED_PRIORITY 1
#define DIVISION 3
#define MULTIPLICATION 3
#define FUNC 2
#define SUMM 4
#define DIFFERENCE 4

#endif
