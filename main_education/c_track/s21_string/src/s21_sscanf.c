#include "s21_sscanf.h"

int s21_sscanf(char *str1, const char *format, ...) {
  va_list factor;
  va_start(factor, format);

  // reset internal state (static variables)
  __convert_decimal__(NULL, NULL, NULL, 0);
  __convert_hexadecimal__(NULL, NULL, 0, NULL, 0);
  convert_decimal(NULL, NULL, 0, 0, 0, 0);

  int n_counter = 0;
  int return_counter = 0;
  for (char *p = (char *)format; *p != '\0'; p++) {
    if (*p == '%') {
      struct info main_info = {'0', '0', 0};
      check_specifiers(++p, &main_info);
      void *offset = NULL;
      if (*p != '*') {
        offset = va_arg(factor, void *);
      }
      if (*p == 'n') {
        n_counter =
            convert_decimal(offset, str1, __aggregation__(main_info.ch),
                            main_info.width, main_info.lenght, &return_counter);
        if (offset != NULL) {
          *(int *)offset = n_counter;
        }
      } else {
        convert_decimal(offset, str1, __aggregation__(main_info.ch),
                        main_info.width, main_info.lenght, &return_counter);
      }
    }
  }
  va_end(factor);

  return return_counter;
}

void check_specifiers(const char *format, struct info *main_info) {
  char ptr[16] = "cdieEfgGosuxXpn";
  char width[4] = "hlL";
  int flag = 0;
  int exp = 1;
  int lenght = 0;
  for (char *p = (char *)format; *p != '\0'; p++) {
    if (isdigit(*p)) {
      exp *= 10;
    }
    for (char *st = width; *st != '\0'; st++) {
      if (*st == *p) {
        main_info->width = *p;
      }
    }
    for (char *st = ptr; *st != '\0'; st++) {
      if (*st == *p) {
        main_info->ch = *p;
        flag = 1;
        break;
      }
    }
    if (flag == 1) {
      break;
    }
  }
  exp /= 10;

  for (char *p = (char *)format; *p != '\0'; p++) {
    if (isdigit(*p)) {
      lenght += exp * (*p - 48);
      exp /= 10;
    }
  }

  main_info->lenght = lenght;
}

int convert_decimal(void *offset, char *str1, int flag, int width, int lenght,
                    int *return_counter) {
  static int initializer = 0;
  static int n_counter = 0;
  static char *old = NULL;
  static char source = '\0';

  if (offset == NULL && str1 == NULL && flag == 0 && width == 0 &&
      lenght == 0) {
    initializer = 0;
    n_counter = 0;
    old = NULL;
    source = '\0';
    return 0;
  }

  int n_counter_origin = 0;
  int case_o = 16;
  int lenght_flag = 0;
  char *temp_char = NULL;
  int counter = 0;

  if (source != *str1) {
    source = *str1;
    old = NULL;
  }

  if (old == NULL) {
    old = str1;
  }
  if (old != str1) {
    if (*--old == '\0') {
      initializer = 1;
    }
    old++;
  }

  if (lenght != 0) {
    lenght_flag = 1;
    temp_char = old;
    char substitute[1024] = {'\0'};
    counter = __rewrite_ptr_use_lenght__(lenght, old, substitute);
    old = substitute;
  }

  if (!initializer) {
    if (offset != NULL) {
      (*return_counter)++;
      if (flag != 3 && flag != 4) {
        while (isalpha(*old) || isspace(*old) || *old == '+') {
          if (*old == '-') {
            break;
          }
          old++;
        }
      }

      if (*old == '%') {
        old++;
        if (*old == 'n') {
          n_counter_origin = n_counter;
        } else {
          old--;
        }
      }
      if (flag == 5) {
        case_o = 8;
        flag = 2;
      }
      if (flag == 1) {
        int minus = 0;
        if (*old == '-') {
          minus = 1;
        }
        __convert_decimal__(offset, &old, &n_counter, width);
        if (width == 'h') {
          if (*((short *)offset) != 0) {
            *((short *)offset) %=
                (short)pow(10, __take_len__(offset, width) - 1);
            *((short *)offset) =
                minus == 1 ? *((short *)offset) * (-1) : *((short *)offset);
          }
          if (*((short *)offset) == 0 && *return_counter == 1) {
            (*return_counter)--;
          }
        } else if (width == 'l') {
          if (*((long *)offset) != 0) {
            *((long *)offset) %= (long)pow(10, __take_len__(offset, width) - 1);
            *((long *)offset) =
                minus == 1 ? *((long *)offset) * (-1) : *((long *)offset);
          }
          if (*((long *)offset) == 0 && *return_counter == 1) {
            (*return_counter)--;
          }
        } else {
          if (*((int *)offset) != 0) {
            *((int *)offset) %= (int)pow(10, __take_len__(offset, width) - 1);
            *((int *)offset) =
                minus == 1 ? *((int *)offset) * (-1) : *((int *)offset);
          }
          if (*((int *)offset) == 0 && *return_counter == 1) {
            (*return_counter)--;
          }
        }
      } else if (flag == 2) {
        if (*old == '0' && (*++old == 'x' || *old == 'X')) {
          old++;
        }
        if (__convert_hexadecimal__(offset, &old, case_o, &n_counter, width)) {
          long long __pre_point_number_hex__ = *(long long *)offset;
          __convert_hexadecimal__(offset, &old, case_o, &n_counter, width);
          *(double *)offset += __pre_point_number_hex__;
        }
      } else if (flag == 3) {
        __rewrite_ptr__(offset, &old, &n_counter);
      } else if (flag == 4) {
        *(char *)offset = *old;
        old++;
        n_counter++;
      } else if (flag == 6) {
        int minus = 0;
        if (*old == '-') {
          minus = 1;
          old++;
        }

        if (__convert_decimal__(offset, &old, &n_counter, width)) {
          long long __pre_point_number__ = 0;
          if (width == 'L') {
            __pre_point_number__ = (long long)*(long long *)offset;
          } else if (width == 'l') {
            __pre_point_number__ = (long)*(long *)offset;
          } else {
            __pre_point_number__ = (int)*(int *)offset;
          }

          __convert_decimal__(offset, &old, &n_counter, width);
          long long __number__;
          if (width == 'L' || width == 'l') {
            __number__ = *(long long *)offset;
          } else {
            __number__ = *(int *)offset;
          }

          if (width == 'L') {
            *(long double *)offset =
                __decimal_to_double_converter__(__number__) +
                __pre_point_number__;
          } else if (width == 'l') {
            *(double *)offset = __decimal_to_double_converter__(__number__) +
                                __pre_point_number__;
          } else {
            *(float *)offset = __decimal_to_double_converter__(__number__) +
                               __pre_point_number__;
          }
          int i = 0;
          long long num = 0;
          if (width == 'L') {
            num = (long long)*(long double *)offset;
          } else if (width == 'l') {
            num = (long long)*(double *)offset;
          } else {
            num = (long long)*(float *)offset;
          }
          for (i = 0; num != 0; i++, num = num / 10) {
          }
          if (width == 'L') {
            if (*(long double *)offset != 0.0) {
              *(long double *)offset = ((*(long double *)offset -
                                         (long long)*(long double *)offset) *
                                            10 -
                                        1.0) +
                                       ((long long)*(long double *)offset %
                                        (long long)pow(10, i - 1));
            }
            if (*(long double *)offset == 0.0 && *return_counter == 1) {
              (*return_counter)--;
            }
          } else if (width == 'l') {
            if (*(double *)offset != 0.0) {
              *(double *)offset =
                  ((*(double *)offset - (long long)*(double *)offset) * 10 -
                   1.0) +
                  ((long long)*(double *)offset % (long long)pow(10, i - 1));
            }
            if (*(double *)offset == 0.0 && *return_counter == 1) {
              (*return_counter)--;
            }
          } else {
            if (*(float *)offset != 0) {
              *(float *)offset =
                  ((*(float *)offset - (long long)*(float *)offset) * 10 -
                   1.0) +
                  ((long long)*(float *)offset % (long long)pow(10, i - 1));
            }
            if (*(float *)offset == 0.0 && *return_counter == 1) {
              (*return_counter)--;
            }
          }
        } else {
          if (*return_counter == 1) {
            (*return_counter)--;
          }
        }

        char *copy_ptr = NULL;
        copy_ptr = old;
        double koeff = 0;
        if ((koeff = __check_exponential_form__(--copy_ptr, &old,
                                                &n_counter)) != 0) {
          if (width == 'L') {
            *(long double *)offset *= (long double)pow(10, koeff);
          } else if (width == 'l') {
            *(double *)offset *= (double)pow(10, koeff);
          } else {
            *(float *)offset *= (float)pow(10, koeff);
          }
        }
        if (width == 'L') {
          *((long double *)offset) = minus == 1
                                         ? *((long double *)offset) * (-1.0)
                                         : *((long double *)offset);
        } else if (width == 'l') {
          *((double *)offset) =
              minus == 1 ? *((double *)offset) * (-1.0) : *((double *)offset);
        } else {
          *((float *)offset) =
              minus == 1 ? *((float *)offset) * (-1.0) : *((float *)offset);
        }

      } else if (flag == 7) {
        if (*old == '0' && (*++old == 'x' || *old == 'X')) {
          old++;
        }
        __convert_hexadecimal_p__(offset, &old, case_o, &n_counter);
      }
    } else {
      if (flag == 4) {
        old++;
      } else {
        for (;; old++) {
          if (isspace(*old) || *old == '\0') {
            old++;
            break;
          }
        }
      }
    }
    if (lenght_flag) {
      int shift = 0;
      old = temp_char;
      for (;; old++, shift++) {
        if (shift == counter) {
          if (isspace(*old)) {
            old++;
          }
          break;
        }
        if (isspace(*old) || *old == '\0') {
          old++;
          break;
        }
      }
    }
  }
  return n_counter_origin;
}

int __rewrite_ptr_use_lenght__(int lenght, const char *old, char *dst) {
  char *temp_char = NULL;
  int counter = 0;
  for (char *p = (char *)old;; counter++, p++) {
    if (isspace(*p) || *p == ' ' || *p == '\0' || counter == lenght) {
      break;
    }
  }

  temp_char = dst;
  int return_counter = counter;
  for (char *p = (char *)old; *p != '\0'; p++) {
    if (counter == 0) {
      break;
    }
    *dst = *p;
    dst++;
    counter--;
  }
  *dst = '\0';
  dst++;
  dst = temp_char;
  return return_counter;
}

int __take_len__(void *offset, int width) {
  long num = 0;
  if (width == 'h') {
    num = *((short *)offset);
  } else if (width == 'l') {
    num = *((long *)offset);
  } else {
    num = *((int *)offset);
  }
  int i = 0;
  for (i = 0; num != 0; i++, num = num / 10) {
  }
  return i;
}

int __convert_decimal__(void *offset, char **old, int *n_counter, int width) {
  static int flag = 0;
  if (offset == NULL && old == NULL && n_counter == NULL && width == 0) {
    flag = 0;
    return 0;
  }

  long long pow_number = 0;
  int len_decimal_counter = 0;
  char *p = NULL;
  for (p = *old; *p != '\0'; p++, len_decimal_counter++, (*n_counter)++) {
    if (isspace(*p) || *p == '.' || isalpha(*p)) {
      break;
    }
    if (*p == '-' || *p == '+') {
      len_decimal_counter--;
    }
  }

  pow_number = (long long)pow(10, len_decimal_counter - 1);

  long long decimal = pow_number * 10;

  for (p = *old;; p++) {
    if (*p == '-' || *p == '+') {
      p++;
    }
    if (isspace(*p) || *p == '.' || isalpha(*p) || *p == '\0') {
      *old = ++p;
      break;
    }
    decimal += (*p - 48) * pow_number;
    pow_number /= 10;
  }
  if (flag) {
    flag = 0;
  }

  if (width == 'h') {
    *((short *)offset) = (short)decimal;
  } else if (width == 'l') {
    *((long *)offset) = (long)decimal;
  } else if (width == 'L') {
    *((long long *)offset) = decimal;
  } else {
    *((int *)offset) = (int)decimal;
  }

  if (*(p - 1) == '.') {
    (*n_counter)++;
    flag = 1;
  }
  return flag;
}

int __convert_hexadecimal__(void *offset, char **old, int case_o,
                            int *n_counter, int width) {
  static int flag = 0;

  if (offset == NULL && old == NULL) {
    flag = 0;
    return 0;
  }

  int power = 0;
  char *p = NULL;
  int minus = 0;
  p = *old;
  for (; *p != '\0'; p++) {
    if (isspace(*p) || *p == '.') {
      break;
    }
    if (*p == '-' || *p == '+') {
      power--;
      (*old)++;
      if (*p == '-') {
        minus = 1;
      }
    }
    power++;
    (*n_counter)++;
  }
  if (flag) {
    (*n_counter)++;
    *((double *)offset) = (double)__converter_point__(power * -1, *old, case_o);
    *((double *)offset) =
        minus == 1 ? *((double *)offset) * (-1.0) : *((double *)offset);
    flag = 0;
  } else {
    if (width == 'h') {
      *((short *)offset) = (short)__converter__(power - 1, *old, case_o);
      *((short *)offset) =
          minus == 1 ? *((short *)offset) * (-1.0) : *((short *)offset);
    } else if (width == 'l') {
      *((long *)offset) = (long)__converter__(power - 1, *old, case_o);
      *((long *)offset) =
          minus == 1 ? *((long *)offset) * (-1.0) : *((long *)offset);
    } else {
      *((int *)offset) = (int)__converter__(power - 1, *old, case_o);
      *((int *)offset) =
          minus == 1 ? *((int *)offset) * (-1.0) : *((int *)offset);
    }
  }
  if (*p == '.') {
    flag = 1;
  }
  *old = ++p;
  return flag;
}

void __convert_hexadecimal_p__(void *offset, char **old, int case_o,
                               int *n_counter) {
  int power = 0;
  char *p = NULL;
  p = *old;
  for (; *p != '\0'; p++) {
    if (isspace(*p)) {
      break;
    }
    power++;
    (*n_counter)++;
  }
  *((long long *)offset) = (long long)__converter__(power - 1, *old, case_o);
  *old = ++p;
}

long long __converter__(int power, char *old, int case_o) {
  long long decimal = 0;
  char *str = NULL;
  str = old;
  int flag = 0;
  for (; power != -1; power--) {
    int minus = 48;
    return_base(&minus, &flag, *str);
    if (!flag) {
      decimal += (*str - minus) * pow(case_o, power);
    }
    str++;
  }
  return decimal;
}

double __converter_point__(int power, char *old, int case_o) {
  double float_part = 0.0;
  char *str = NULL;
  str = old;
  int flag = 0;
  for (int i = -1; i != power - 1; i--) {
    int minus = 48;
    return_base(&minus, &flag, *str);
    if (!flag) {
      float_part += (*str - minus) * pow(case_o, i);
    }
    str++;
  }
  return float_part;
}

void return_base(int *minus, int *flag, char str) {
  if (str >= 65 && str <= 70) {
    *minus = 55;
  } else if (str >= 97 && str <= 102) {
    *minus = 87;
  } else if (str >= 48 && str < 58) {
    *minus = 48;
  } else {
    *flag = 1;
  }
}

double __decimal_to_double_converter__(long long decimal) {
  double converted_decimal = 0.0;
  while (decimal != 0) {
    converted_decimal += (decimal % 10) * 0.1;
    converted_decimal *= 0.1;
    decimal /= 10;
  }
  return converted_decimal * 10;
}

int __check_exponential_form__(char *str, char **old, int *n_counter) {
  int copy_exp = 0;
  int pattern = 0;
  int minus = 0;
  char *copy_str = NULL;
  if (*str == 'e' || *str == 'E') {
    str++;
    (*n_counter)++;
    copy_str = str;
    while (1) {
      if (isspace(*str) || *str == '\0' || isalpha(*str)) {
        *old = ++str;
        break;
      }
      if (*str == '-') {
        minus = 1;
        copy_str++;
      } else {
        copy_exp++;
        if (copy_exp == 1 && *str == '0') {
          copy_exp--;
          copy_str++;
        }
      }
      str++;
      (*n_counter)++;
    }
    for (int i = copy_exp - 1; i != -1; i--) {
      pattern += (*copy_str - 48) * pow(10, i);
      copy_str++;
    }
  }
  if (minus) {
    pattern *= -1;
  }
  return pattern;
}

void __rewrite_ptr__(void *offset, char **old, int *n_counter) {
  char *p = NULL;
  p = *old;
  for (;; p++) {
    if (*p != '\n') {
      if (isspace(*p) || *p == '\0') {
        (*old)++;
        *(char *)offset = '\0';
        break;
      }
    } else {
      p++;
    }
    *(char *)offset = *p;
    (char *)offset++;
    (*old)++;
    (*n_counter)++;
  }
}

int __aggregation__(char ch) {
  int flag = 0;
  if (ch == 'd' || ch == 'i' || ch == 'u') {
    flag = 1;
  } else if (ch == 'x' || ch == 'X') {
    flag = 2;
  } else if (ch == 's') {
    flag = 3;
  } else if (ch == 'c') {
    flag = 4;
  } else if (ch == 'o') {
    flag = 5;
  } else if (ch == 'e' || ch == 'E' || ch == 'g' || ch == 'G' || ch == 'f') {
    flag = 6;
  } else if (ch == 'p') {
    flag = 7;
  }
  return flag;
}
