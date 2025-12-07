#ifndef _S21_SPRINTF_H_
#define _S21_SPRINTF_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef unsigned char uchar;

char LocaleDecimalPoint();

char *s21_strchr(const char *str, int c);
size_t s21_strlen(const char *str);
char *s21_strcpy(char *dest, const char *src);

#define MAX_ULONG_LEN 32u
#define MAX_DOUBLE_LEN 512u
#define MAX_DECIMAL_LEN 32u
#define MAX_UNSIGNED_LEN 32u
#define MAX_RAW_OUTPUT_LEN 512u

#define MAX_DOUBLE_EXP 308

#ifdef DEBUG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT(...)
#endif

typedef struct ArgFormat {
  bool minus_flag;
  bool plus_flag;
  bool space_flag;
  bool sharp_flag;
  bool zero_flag;

  bool width_used;
  int width;
  bool precision_used;
  int precision;
  bool len_used;
  char len;
  char spec;

  const char *format_begin;
  const char *format_end;
} ArgFormat;

void DEBUG_ARG_FORMAT(ArgFormat *arg_fmt);

void Error(const char *msg);

char *IntToString(uint64_t value, bool sign, int num_digits, size_t base,
                  char *str);

void DoubleToString(long double value, int precision, bool need_dot, char *str);

const char *ParseArg(const char *format, ArgFormat *arg_fmt, va_list args);

char *PrintWCString(char *buf, const wchar_t *str, int max_len);

void Print_c(char *buf, ArgFormat *arg_fmt, int arg);

int64_t GetIntArgument(ArgFormat *arg_fmt, va_list args);

uint64_t GetUnsignedIntArgument(ArgFormat *arg_fmt, va_list args);

long double GetDoubleArgument(ArgFormat *arg_fmt, va_list args);

void PrintInt(char *buf, int64_t value, ArgFormat *arg_fmt, int base);

void PrintUnsignedInt(char *buf, int num_digits, int64_t value, int base);

void Print_d(char *buf, ArgFormat *arg_fmt, va_list args);

void Print_o(char *buf, ArgFormat *arg_fmt, va_list args);

void Print_x(char *buf, ArgFormat *arg_fmt, va_list args);

void HexToUpper(char *str);

void Print_X(char *buf, ArgFormat *arg_fmt, va_list args);

void Print_i(char *buf, ArgFormat *arg_fmt, va_list args);

void Print_f(char *buf, ArgFormat *arg_fmt, long double value);

int CalcExp(long double value);

void RemoveTrailingZeroes(char *str);

void PrintExp(char *buf, ArgFormat *arg_fmt, long double value, char exp_char,
              bool print_trailing_zeroes);

void Print_s(char *buf, ArgFormat *arg_fmt, va_list args);

void PrintGeneral(char *buf, ArgFormat *arg_fmt, long double value,
                  char exp_char);

void PrintPointer(char *buf, ArgFormat *arg_fmt, void *p);

void PrintPercent(char *buf);

bool IsDigitSpec(char spec);

char *ProcessWidth(char *buf, const char *str, const ArgFormat *arg_fmt,
                   bool is_nan_or_inf, bool is_eol);

char *ProcessArg(char *buf, ArgFormat *arg_fmt, va_list args);

int Process(char *buf, const char *format, va_list args);

int s21_sprintf(char *buf, const char *format, ...);

#endif  // _S21_SPRINTF_H_
