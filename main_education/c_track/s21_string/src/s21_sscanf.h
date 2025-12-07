#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

struct info {
  char ch;
  char width;
  int lenght;
};

int s21_sscanf(char *str1, const char *format, ...);
void check_specifiers(const char *format, struct info *main_info);
int convert_decimal(void *offset, char *str1, int flag, int width, int lenght,
                    int *return_counter);
int __rewrite_ptr_use_lenght__(int lenght, const char *old, char *dst);
int __convert_decimal__(void *offset, char **old, int *n_counter, int width);
int __convert_hexadecimal__(void *offset, char **old, int case_o,
                            int *n_counter, int width);
void __convert_hexadecimal_p__(void *offset, char **old, int case_o,
                               int *n_counter);
int __take_len__(void *offset, int width);
long long __converter__(int power, char *old, int case_o);
double __converter_point__(int power, char *old, int case_o);
double __decimal_to_double_converter__(long long decimal);
int __check_exponential_form__(char *str, char **old, int *n_counter);
void __rewrite_ptr__(void *offset, char **old, int *n_counter);
int __aggregation__(char ch);
void return_base(int *minus, int *flag, char str);
