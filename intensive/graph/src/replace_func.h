#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPRESSION_LENGTH 1024
#define MAX_FUNCTIONS 6

void replace_function(char *source, const char *search, const char *replace);

void shorten_functions(char *expression);

int check_brackets(const char *expression);

int is_digit(char ch);

int check_numbers_and_operators(const char *expression);

int process_expression(char *expression);
