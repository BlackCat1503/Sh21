#include "s21_sprintf.h"

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "s21_string.h"

#define ERROR(msg) \
  {                \
    puts(msg);     \
    exit(1);       \
  }

typedef unsigned char uchar;

char LocaleDecimalPoint() { return localeconv()->decimal_point[0]; }

bool s21_isinf(long double value) {
  return isinf(value) || fabsl(value) == HUGE_VALL;
}

void DEBUG_ARG_FORMAT(ArgFormat *arg_fmt) {
  DEBUG_PRINT("minus_flag: %d\n", arg_fmt->minus_flag);
  DEBUG_PRINT("plus_flag: %d\n", arg_fmt->plus_flag);
  DEBUG_PRINT("space_flag: %d\n", arg_fmt->space_flag);
  DEBUG_PRINT("sharp_flag: %d\n", arg_fmt->sharp_flag);
  DEBUG_PRINT("zero_flag: %d\n", arg_fmt->zero_flag);
  if (arg_fmt->width_used) {
    DEBUG_PRINT("width: %d\n", arg_fmt->width);
  } else {
    DEBUG_PRINT("width not used\n");
  }
  if (arg_fmt->precision_used) {
    DEBUG_PRINT("precision: %d\n", arg_fmt->precision);
  }
  if (arg_fmt->len_used) {
    DEBUG_PRINT("len: %c\n", arg_fmt->len);
  }
  DEBUG_PRINT("spec: %c\n", arg_fmt->spec);
  fflush(stdout);
}

long double PrecisionRound(long double value, int precision) {
  int power = pow(10, precision);
  return roundl(value * power) / power;
}

char *IntToString(uint64_t value, bool sign, int num_digits, size_t base,
                  char *str) {
  DEBUG_PRINT("IntToString: value = %lu, num_digits = %zu\n", value,
              num_digits);
  if (value != 0 || num_digits != 0) {
    if (sign) *str++ = '-';

    char buf[MAX_ULONG_LEN] = {0};
    int len = 0;
    do {
      int digit = value % base;
      value /= base;
      char c = '0';
      if (digit < 10)
        c += digit;
      else
        c = 'a' + digit - 10;
      buf[len++] = c;
    } while (value > 0);
    DEBUG_PRINT("IntToString: buf1 = '%s'\n", buf);

    // append zeroes before if num_digits required
    while (len < num_digits) {
      buf[len++] = '0';
    }
    buf[len] = '\0';

    DEBUG_PRINT("IntToString: buf = '%s'\n", buf);

    while (len > 0) {
      --len;
      *str++ = buf[len];
    }
  } else {
    if (num_digits > 0) *str++ = '0';
  }
  *str = '\0';
  return str;
}

char *AddZeroes(char *buf, int number) {
  for (int i = 0; i < number; ++i) {
    *buf++ = '0';
  }
  *buf = '\0';
  return buf;
}

void DoubleToString(long double value, int precision, bool need_dot,
                    char *str) {
  // sign
  if (value < 0) {
    *str++ = '-';
    value = -value;
  }

  // In case of huge number like 1E30 or even 1E300
  size_t crop_to_long_int_shift = 0;
  while (value >= (long double)ULLONG_MAX) {
    value /= 10;
    ++crop_to_long_int_shift;
  }
  DEBUG_PRINT("crop_to_long_int_shift: %zu\n", crop_to_long_int_shift);

  if (precision == 0) {
    uint64_t int_part = (uint64_t)roundl(value);
    str = IntToString(int_part, false, 1, 10, str);
    str = AddZeroes(str, crop_to_long_int_shift);
    if (need_dot) {
      *str++ = LocaleDecimalPoint();
    }
    *str = '\0';
  } else if (precision > 0) {
    precision > 16 && (precision = 16);  // Restriction for precision!!!

    uint64_t int_part = (uint64_t)value;
    long double float_part = value - int_part;
    uint64_t power = pow(10, precision);
    long double shifted_float_part = float_part * power;
    uint64_t rounded = (uint64_t)roundl(shifted_float_part);

    DEBUG_PRINT("int part: %ld", int_part);
    DEBUG_PRINT("float_part: %.30Lf\n", float_part);
    DEBUG_PRINT("shifted_float_part: %.30Lf\n", shifted_float_part);
    DEBUG_PRINT("rounded: %ld\n", rounded);

    if (rounded == power) {
      int_part++;
      float_part -= 1;
    } else if (rounded == -power) {
      int_part--;
      float_part += 1;
    }
    shifted_float_part = float_part * power;
    rounded = (uint64_t)roundl(shifted_float_part);

    // if it's huge number then let's consider float part as zero
    if (crop_to_long_int_shift > 0) {
      rounded = 0;
    }

    DEBUG_PRINT("int part: %ld", int_part);
    DEBUG_PRINT("float_part: %.30Lf\n", float_part);
    DEBUG_PRINT("shifted_float_part: %.30Lf\n", shifted_float_part);
    DEBUG_PRINT("rounded: %ld\n", rounded);

    str = IntToString(int_part, false, 1, 10, str);
    str = AddZeroes(str, crop_to_long_int_shift);

    *str++ = LocaleDecimalPoint();
    IntToString(rounded, false, precision, 10, str);
  } else {
    ERROR("DoubleToString: Negative precision");
  }
}

const char *ParseArg(const char *format, ArgFormat *arg_fmt, va_list args) {
  const char *p = format;
  assert(*p == '%');  // "Format have to start from '%'");
  ++p;

  while (s21_strchr("-+ #0", *p)) {
    switch (*p) {
      case '-':
        arg_fmt->minus_flag = true;
        break;
      case '+':
        arg_fmt->plus_flag = true;
        break;
      case ' ':
        arg_fmt->space_flag = true;
        break;
      case '#':
        arg_fmt->sharp_flag = true;
        break;
      case '0':
        arg_fmt->zero_flag = true;
        break;
    }
    ++p;
  }

  // Flag ' ' is ignored when flag '+' is present
  if (arg_fmt->plus_flag) {
    arg_fmt->space_flag = false;
  }

  if (*p == '*') {
    arg_fmt->width = va_arg(args, int);
    arg_fmt->width_used = true;
    ++p;
  } else {
    size_t w = 0;
    // read width
    while (isdigit(*p)) {
      w = 10 * w + (*p - '0');
      ++p;
    }
    if (w > 0) {
      arg_fmt->width_used = true;
      arg_fmt->width = w;
    }
  }

  if (*p == '.') {
    ++p;
    if (*p == '*') {
      int prec = va_arg(args, int);
      // if (prec < 0) prec = 0;
      arg_fmt->precision = prec;
      arg_fmt->precision_used = true;
      ++p;
    } else {
      size_t prec = 0;
      while (isdigit(*p)) {
        prec = 10 * prec + (*p - '0');
        ++p;
      }
      arg_fmt->precision = prec;
      arg_fmt->precision_used = true;
    }
  }

  if (s21_strchr("hlL", *p)) {
    arg_fmt->len = *p++;
    arg_fmt->len_used = true;
  }

  if (false == arg_fmt->precision_used) {
    if (s21_strchr("eEfgG", *p)) {
      arg_fmt->precision = 6;
      arg_fmt->precision_used = true;
    } else if (*p != 's') {
      arg_fmt->precision = 1;
      arg_fmt->precision_used = true;
    }
  }

  if (s21_strchr("cdieEfgGosuxXpn%", *p)) {
    arg_fmt->spec = *p;
  } else {
    arg_fmt->spec = '?';
    arg_fmt->format_begin = format;
    arg_fmt->format_end = p;
  }
  ++p;

  return p;
}

int64_t GetIntArgument(ArgFormat *arg_fmt, va_list args) {
  int64_t value = 0;
  if (arg_fmt->len_used) {
    if (arg_fmt->len == 'h') {
      value = (short)va_arg(args, int);
    } else if (arg_fmt->len == 'l') {
      value = va_arg(args, int64_t);
    }
  } else {
    value = (int64_t)va_arg(args, int);
  }
  return value;
}

uint64_t GetUnsignedIntArgument(ArgFormat *arg_fmt, va_list args) {
  uint64_t value = 0;
  if (arg_fmt->len_used) {
    if (arg_fmt->len == 'h') {
      value = (unsigned short)va_arg(args, unsigned int);
    } else if (arg_fmt->len == 'l') {
      value = va_arg(args, uint64_t);
    }
  } else {
    value = (uint64_t)va_arg(args, unsigned int);
  }
  return value;
}

long double GetDoubleArgument(ArgFormat *arg_fmt, va_list args) {
  long double value;
  if (arg_fmt->len_used && arg_fmt->len == 'L') {
    value = va_arg(args, long double);
  } else {
    value = (long double)va_arg(args, double);
  }
  return value;
}

int GetNumDigits(ArgFormat *arg_fmt) {
  int num_digits = 0;
  if (arg_fmt->precision_used) {
    num_digits = arg_fmt->precision;
  }
  return num_digits;
}

char *ProcessFlags(char *buf, ArgFormat *arg_fmt, bool negative_value) {
  if (!negative_value) {
    if (arg_fmt->plus_flag)
      *buf++ = '+';
    else if (arg_fmt->space_flag)
      *buf++ = ' ';
  }
  return buf;
}

void PrintInt(char *buf, int64_t value, ArgFormat *arg_fmt, int base) {
  char str[MAX_DECIMAL_LEN] = {0};
  IntToString(labs(value), value < 0, GetNumDigits(arg_fmt), base, str);
  for (const char *p = str; *p; ++p) {
    *buf++ = *p;
  }
  *buf = '\0';
}

void PrintUnsignedInt(char *buf, int num_digits, int64_t value, int base) {
  char str[MAX_UNSIGNED_LEN] = {0};
  IntToString(value, false, num_digits, base, str);
  for (const char *p = str; *p; ++p) {
    *buf++ = *p;
  }
  *buf = '\0';
}

void Print_d(char *buf, ArgFormat *arg_fmt, va_list args) {
  int64_t value = GetIntArgument(arg_fmt, args);
  buf = ProcessFlags(buf, arg_fmt, value < 0);
  PrintInt(buf, value, arg_fmt, 10);
}

void Print_o(char *buf, ArgFormat *arg_fmt, va_list args) {
  int64_t value = GetUnsignedIntArgument(arg_fmt, args);

  char tmp_buf[MAX_DECIMAL_LEN] = {0};
  PrintUnsignedInt(tmp_buf, GetNumDigits(arg_fmt), value, 8);

  if (value != 0 && arg_fmt->sharp_flag && *tmp_buf != '0') {
    *buf++ = '0';
  }
  s21_strcpy(buf, tmp_buf);
}

void Print_x(char *buf, ArgFormat *arg_fmt, va_list args) {
  int64_t value = GetUnsignedIntArgument(arg_fmt, args);
  if (value != 0 && arg_fmt->sharp_flag) {
    *buf++ = '0';
    *buf++ = 'x';
  }
  PrintUnsignedInt(buf, GetNumDigits(arg_fmt), value, 16);
}

void HexToUpper(char *str) {
  for (; *str != '\0'; ++str) {
    if (islower(*str)) {
      *str = toupper(*str);
    }
  }
}

void Print_X(char *buf, ArgFormat *arg_fmt, va_list args) {
  char *buf_start = buf;
  Print_x(buf, arg_fmt, args);
  HexToUpper(buf_start);
}

void Print_i(char *buf, ArgFormat *arg_fmt, va_list args) {
  Print_d(buf, arg_fmt, args);  // %d and %i are the same for printf
}

void Print_f(char *buf, ArgFormat *arg_fmt, long double value) {
  buf = ProcessFlags(buf, arg_fmt, value < 0);
  int precision = 6;
  if (arg_fmt->precision_used) {
    precision = arg_fmt->precision;
  }
  DEBUG_PRINT("Print_f: value=%Lf prec=%d\n", value, precision);
  char str[MAX_DOUBLE_LEN] = {0};
  DoubleToString(value, precision, arg_fmt->sharp_flag, str);
  for (const char *p = str; *p; ++p) {
    *buf++ = *p;
  }
  *buf = '\0';
}

#define LOG10_2 0.301029995663981195230L
int GetExp10(long double x) {
  int exp2;
  frexpl(x, &exp2);
  int exp10 = LOG10_2 * exp2;
  if (exp2 < 1) --exp10;

  // we are to check it and correct a bit if needs
  long double nx = fabsl(x * pow(10, -exp10));
  if (nx < 1)
    --exp10;
  else if (nx >= 10)
    ++exp10;

  if (x == 0) exp10 = 0;
  return exp10;
}

void RemoveTrailingZeroes(char *str) {
  char dec_point = LocaleDecimalPoint();
  while (*str && *str != dec_point) ++str;
  if (*str == dec_point) {
    ++str;
    while (isdigit(*str)) ++str;
    --str;
    while (*str == '0') --str;
    if (*str == dec_point)
      *str = '\0';
    else
      *++str = '\0';
  }
}

void PrintExp(char *buf, ArgFormat *arg_fmt, long double value, char exp_char,
              bool print_trailing_zeroes) {
  DEBUG_PRINT("PrintExp: value=%.30Lf\n", value);

  buf = ProcessFlags(buf, arg_fmt, value < 0);
  int exp = GetExp10(value);
  DEBUG_PRINT("PrintExp: exp = %d\n", exp);

  if (exp < -MAX_DOUBLE_EXP) {
    value = 0;
    exp = 0;
  }
  value *= pow(10, -exp);
  DEBUG_PRINT("PrintExp: prec = %d, exp = %d\n", arg_fmt->precision, exp);

  char mantissa_str[MAX_DOUBLE_LEN] = {0};
  int precision = 6;
  if (arg_fmt->precision_used) {
    precision = arg_fmt->precision;
  }

  // just in case to avoid inf loop
  precision > MAX_DOUBLE_EXP && (precision = MAX_DOUBLE_EXP);

  DEBUG_PRINT("PrintExp: value = %Lf, prec = %d\n", value, precision);
  DoubleToString(value, precision, arg_fmt->sharp_flag, mantissa_str);

  // Kinda problem: mantissa_str could be equal 10, but it has to be less
  // in that case we need to change it to 1.
  DEBUG_PRINT("PrintExp: mantissa_str: %s\n", mantissa_str);
  char *p = strstr(mantissa_str, "10");
  DEBUG_PRINT("found 10 in mant: %p\n", p);
  if (p && (*(p + 2) == LocaleDecimalPoint() ||
            (*(p + 2) == '\0' && !strchr(mantissa_str, '.')))) {
    DEBUG_PRINT("Change mantissa from 10. to 1.) !");
    ++exp;
    value /= 10;
    DoubleToString(value, precision, arg_fmt->sharp_flag, mantissa_str);
  }

  DEBUG_PRINT("PrintExp AFTER: mantissa_str: %s\n", mantissa_str);
  if (!print_trailing_zeroes) {
    RemoveTrailingZeroes(mantissa_str);
  }
  for (p = mantissa_str; *p; ++p) {
    *buf++ = *p;
  }
  *buf++ = exp_char;
  if (exp >= 0) *buf++ = '+';
  char exp_str[MAX_DOUBLE_LEN] = {0};
  IntToString(labs(exp), exp < 0, 2, 10, exp_str);
  for (p = exp_str; *p; ++p) {
    *buf++ = *p;
  }
  *buf = '\0';
}

char *PrintWCString(char *buf, const wchar_t *str, int max_len) {
  const size_t kWcharSize = 4;
  const size_t wlen = wcslen(str);
  size_t convert_len = wlen;
  if (max_len > 0) {
    convert_len = max_len;
  }
  int len = wcstombs(buf, str, convert_len * kWcharSize);
  DEBUG_PRINT("PrintWCString: len = %d\n", len);

  if (len > 0) {
    buf[len] = '\0';
  }
  return buf + len;
}

void Print_c(char *buf, ArgFormat *arg_fmt, int arg) {
  DEBUG_PRINT("Print_c: arg = %d\n", arg);
  if (arg_fmt->len_used && arg_fmt->len == 'l') {
    wchar_t value = (wchar_t)arg;
    wchar_t wcs[2] = {value, 0};
    buf = PrintWCString(buf, wcs, 1);
  } else {
    char value = (char)arg;
    *buf++ = value;
  }
  *buf = '\0';
}

char *Print_null_as_str(char *buf, int max_len) {
  const char *const null_str = "(null)";
  const int kLen = s21_strlen(null_str);
  s21_strcpy(buf, null_str);
  if (max_len < 0 || max_len >= kLen) buf += kLen;
#ifdef __APPLE__
  else
    buf += max_len;
#endif
  return buf;
}

void Print_s(char *buf, ArgFormat *arg_fmt, va_list args) {
  int max_len = -1;
  if (arg_fmt->precision_used) {
    max_len = arg_fmt->precision;
  }
  DEBUG_PRINT("Print_s: max_len = %d\n", max_len);

  if (arg_fmt->len_used && arg_fmt->len == 'l') {
    DEBUG_PRINT("Print_s: ws\n");
    const wchar_t *str = va_arg(args, wchar_t *);
    if (str) {
      buf = PrintWCString(buf, str, max_len);
    } else {
      buf = Print_null_as_str(buf, max_len);
    }
  } else {
    DEBUG_PRINT("Print_s: ascii str\n");
    const char *str = va_arg(args, const char *);
    if (str) {
      for (const char *p = str; *p; ++p) {
        if (max_len >= 0 && (p - str >= max_len)) {
          break;
        }
        *buf++ = *p;
      }
    } else {
      buf = Print_null_as_str(buf, max_len);
    }
  }
  *buf = '\0';
}

void PrintGeneral(char *buf, ArgFormat *arg_fmt, long double value,
                  char exp_char) {
  int exp = GetExp10(value);
  if (exp < -MAX_DOUBLE_EXP) {
    value = 0;
    exp = 0;
  }
  int precision = 6;
  if (arg_fmt->precision_used) precision = arg_fmt->precision;
  if (precision == 0) precision = 1;

  if (exp < -4 || exp >= precision) {
    // use e and remove trailing zeroes from fract part of res
    DEBUG_PRINT("use e: prec = %d, exp = %d\n", precision, exp);

    ArgFormat af = *arg_fmt;
    af.precision_used = true;
    af.precision = precision - 1;
    DEBUG_PRINT("PrintGeneral: prec=%d\n", af.precision);

    char str[MAX_DOUBLE_LEN] = {0};
    bool print_trailing_zeroes = arg_fmt->sharp_flag;
    PrintExp(str, &af, value, exp_char, print_trailing_zeroes);
    s21_strcpy(buf, str);
  } else {
    // use f
    DEBUG_PRINT("use f: prec = %d, exp = %d\n", precision, exp);
    ArgFormat af = *arg_fmt;
    af.precision_used = true;
    af.precision = precision - exp - 1;

    if (fabsl(PrecisionRound(value, af.precision)) >= 1.0L &&
        fabsl(value) < 1.0L) {
      af.precision--;
    }
    DEBUG_PRINT("use f change: prec = %d, exp = %d\n", af.precision, exp);

    DEBUG_PRINT("PrintGeneral: prec=%d\n", af.precision);
    Print_f(buf, &af, value);
    if (!arg_fmt->sharp_flag) {
      RemoveTrailingZeroes(buf);
    }
  }
}

void PrintPointer(char *buf, ArgFormat *arg_fmt, void *p) {
  if (p != NULL) {
    buf = ProcessFlags(buf, arg_fmt, false);
  }

  bool unix_host = false;
#ifdef __unix__
  unix_host = true;
#endif
  if (unix_host && p == NULL) {
    strcpy(buf, "(nil)");
  } else {
    *buf++ = '0';
    *buf++ = 'x';
    PrintUnsignedInt(buf, GetNumDigits(arg_fmt), (uint64_t)p, 16);
  }
}

void PrintPercent(char *buf) {
  *buf++ = '%';
  *buf = '\0';
}

bool IsDigitSpec(char spec) { return s21_strchr("dieEfgGouxX", spec); }

char *ProcessWidth(char *buf, const char *str, const ArgFormat *arg_fmt,
                   bool is_nan_or_inf, bool is_eol) {
  if (arg_fmt->width_used) {
    if (arg_fmt->minus_flag) {
      // align left
      DEBUG_PRINT("align left\n");

      int i = 0;
      while (str[i]) {
        *buf++ = str[i++];
      }

      int width = arg_fmt->width;
      if (is_eol) {
        *buf++ = '\0';
        --width;
      }

      while (i++ < width) {
        *buf++ = ' ';
      }
    } else {
      // align right
      DEBUG_PRINT("align right\n");

      char filler = ' ';
      if (arg_fmt->zero_flag && IsDigitSpec(arg_fmt->spec) && !is_nan_or_inf) {
        filler = '0';
      }

      int width = arg_fmt->width;
      // str: -1.234, after adding zeros should be: -00001.234
      // need to move '-', '+', ' ' to the front of a number
      if (filler == '0') {
        if (*str == '+' || *str == '-' || *str == ' ') {
          *buf++ = *str;
          --width;
          ++str;
        }
      }

      int str_len = s21_strlen(str);
      if (is_eol) {
        ++str_len;
      }
      int fill_len = width - str_len;

      DEBUG_PRINT("ProcessWidth: str='%s'\n", str);
      DEBUG_PRINT("ProcessWidth: str_len = %d\n", str_len);
      for (int i = 0; i < fill_len; ++i) {
        *buf++ = filler;
      }

      for (int i = 0; i < str_len; ++i) {
        *buf++ = str[i];
      }
    }
  } else {
    DEBUG_PRINT("Process width: just copy\n");
    DEBUG_PRINT("is_eol: %d\n", is_eol);
    while (*str) {
      *buf++ = *str++;
    };
    if (is_eol) {
      *buf++ = '\0';
    }
  }
  return buf;
}

void PrintIntArg(char *out, ArgFormat *arg_fmt, va_list args) {
  switch (arg_fmt->spec) {
    case 'd':
      Print_d(out, arg_fmt, args);
      break;
    case 'i':
      Print_i(out, arg_fmt, args);
      break;
    case 'o':
      DEBUG_PRINT("PrintIntArg: before Print_o: out: '%s', addr: %p\n", out,
                  out);
      Print_o(out, arg_fmt, args);
      DEBUG_PRINT("PrintIntArg: after Print_o: out: '%s', addr: %p\n", out,
                  out);
      break;
    case 'x':
      Print_x(out, arg_fmt, args);
      break;
    case 'X':
      Print_X(out, arg_fmt, args);
      break;
    case 'u': {
      uint64_t value = GetUnsignedIntArgument(arg_fmt, args);
      PrintUnsignedInt(out, GetNumDigits(arg_fmt), value, 10);
      break;
    }
  }
}

void PrintFloatArg(char *out, ArgFormat *arg_fmt, long double value) {
  switch (arg_fmt->spec) {
    case 'f':
      Print_f(out, arg_fmt, value);
      break;
    case 'e':
      PrintExp(out, arg_fmt, value, 'e', true);
      break;
    case 'E':
      PrintExp(out, arg_fmt, value, 'E', true);
      break;
    case 'g':
      PrintGeneral(out, arg_fmt, value, 'e');
      break;
    case 'G':
      PrintGeneral(out, arg_fmt, value, 'E');
      break;
  }
}

char *PrintUknownSequence(char *buf, ArgFormat *arg_fmt) {
  for (const char *p = arg_fmt->format_begin; p <= arg_fmt->format_end; ++p) {
    *buf++ = *p;
  }
  *buf = '\0';
  return buf;
}

void ProcessNanOrInf(char *buf, long double value, bool upper_case) {
  bool unix_host = false;
#ifdef __unix__
  unix_host = true;
#endif
  if (isnan(value)) {
    if (unix_host && signbit(value)) {
      *buf++ = '-';
    }
    strcpy(buf, upper_case ? "NAN" : "nan");
  } else if (s21_isinf(value)) {
    strcpy(buf, upper_case ? "INF" : "inf");
  }
}

char *ProcessArg(char *buf, ArgFormat *arg_fmt, va_list args) {
  DEBUG_ARG_FORMAT(arg_fmt);
  DEBUG_PRINT("ProcessArg\n");

  char raw_out[MAX_RAW_OUTPUT_LEN] = {0};
  bool is_nan_or_inf = false;
  char spec = arg_fmt->spec;

  bool is_eol = false;

  if (spec == '?') {
    // for uknown combo % and chars after (e.g. "%10.3hr")
    buf = PrintUknownSequence(buf, arg_fmt);
  } else {
    if (s21_strchr("diouxX", spec)) {
      PrintIntArg(raw_out, arg_fmt, args);
      DEBUG_PRINT("ProcessArg: PrintIntArg: raw_out: '%s'\n", raw_out);
    } else if (s21_strchr("feEgG", spec)) {
      long double value = GetDoubleArgument(arg_fmt, args);
      if (isnan(value) || s21_isinf(value)) {
        is_nan_or_inf = true;
        ProcessNanOrInf(raw_out, value, isupper(spec));
      } else {
        PrintFloatArg(raw_out, arg_fmt, value);
      }
    } else {
      switch (spec) {
        case 'c': {
          int arg = va_arg(args, int);
          Print_c(raw_out, arg_fmt, arg);
          if (arg == 0) {
            is_eol = true;
          }
          break;
        }
        case 's':
          Print_s(raw_out, arg_fmt, args);
          break;
        case 'p':
          PrintPointer(raw_out, arg_fmt, va_arg(args, void *));
          break;
        case '%':
          PrintPercent(raw_out);
          break;
      }
    }

    DEBUG_PRINT("ProcessArg: raw_out: '%s'\n", raw_out);
    buf = ProcessWidth(buf, raw_out, arg_fmt, is_nan_or_inf, is_eol);
  }
  return buf;
}

int Process(char *buf, const char *format, va_list args) {
  char *buf_base = buf;

  const char *p = format;
  int counter = 0;
  while (*p) {
    if (*p == '%') {
      ArgFormat arg_fmt = {0};
      p = ParseArg(p, &arg_fmt, args);
      if (arg_fmt.spec == 'n') {
        int *p_number = va_arg(args, void *);
        int bytes_counter = buf - buf_base;
        *p_number = bytes_counter;
      } else {
        buf = ProcessArg(buf, &arg_fmt, args);
      }
      ++counter;
    } else {
      *buf++ = *p++;
    }

    DEBUG_PRINT("buf: %s\n", buf_base);
  }

  if (buf == buf_base || (buf > buf_base && *(buf - 1) != '\0')) {
    *buf = '\0';
  }
  return buf - buf_base;
}

int s21_sprintf(char *buf, const char *format, ...) {
  va_list args;
  va_start(args, format);
  int res = Process(buf, format, args);
  va_end(args);
  return res;
}
