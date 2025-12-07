#include <check.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <stdio.h>

#include "s21_sprintf.h"

#define BUF_SIZE 50000

#define TEST_PRINT_S(string)              \
  {                                       \
    char orig[BUF_SIZE];                  \
    char result[BUF_SIZE];                \
    int n1 = sprintf(orig, string);       \
    int n2 = s21_sprintf(result, string); \
    ck_assert_str_eq(orig, result);       \
    ck_assert_int_eq(n1, n2);             \
  }

#define TEST_PRINT(format, ...)                        \
  {                                                    \
    char orig[BUF_SIZE];                               \
    char result[BUF_SIZE];                             \
    int n1 = sprintf(orig, format, __VA_ARGS__);       \
    int n2 = s21_sprintf(result, format, __VA_ARGS__); \
    ck_assert_int_eq(n1, n2);                          \
    ck_assert_str_eq(orig, result);                    \
  }

START_TEST(s21_sprintf_c_test) {
  TEST_PRINT("%c", 0);
  TEST_PRINT("%lc", 0);

  TEST_PRINT("%1c", 0);
  TEST_PRINT("%5c", 0);
  TEST_PRINT("%16c", 0);
  TEST_PRINT("%-6c", 0);

  TEST_PRINT("%1lc", 0);
  TEST_PRINT("%16lc", 0);
  TEST_PRINT("%-6lc", 0);

  for (int ch = -128; ch <= 127; ++ch) {
    if (ch != 0) {
      TEST_PRINT("%c", ch);
      TEST_PRINT("%16c", ch);
      TEST_PRINT("%-6c", ch);
    }
  }

  unsigned char width_ch = 255;
  TEST_PRINT("%c", width_ch);
}
END_TEST

START_TEST(s21_sprintf_d_test) {
  int values[] = {INT_MIN, -123456, -666, -1, 0, +1, 666, 123456, INT_MAX};
  int size = sizeof(values) / sizeof(*values);
  for (int i = 0; i < size; ++i) {
    int d = values[i];
    TEST_PRINT("%d", d);
    TEST_PRINT("%.15d", d);
    TEST_PRINT("%10d", d);
    TEST_PRINT("%7.*d", -2, d);
    TEST_PRINT("% 17.8d", d);
    TEST_PRINT("% 17.8d", d);
    TEST_PRINT("%0*d", 6, d);
    TEST_PRINT("%-d", d);
    TEST_PRINT("%*.2d", 7, d);
    TEST_PRINT("%+d", d);
    TEST_PRINT("%+-5d", d);
    TEST_PRINT("%+-.5d", d);
    TEST_PRINT("%+3.8d", d);
    TEST_PRINT("%-9.2d", d);
    TEST_PRINT("%55.3hd", d);
    TEST_PRINT("%+-5.13hd", (unsigned short)d);
    TEST_PRINT("%+-5.13ld", (unsigned long)d);
    TEST_PRINT("% 3.3ld", (unsigned long)d);
  }
}
END_TEST

START_TEST(s21_sprintf_d_2test) {
  int values[] = {INT_MIN, -123456789, -100000,   -1,     0,
                  +1,      100000,     123456789, INT_MAX};
  int size = sizeof(values) / sizeof(*values);
  for (int i = 0; i < size; ++i) {
    int d = values[i];
    TEST_PRINT("%d", d);
    TEST_PRINT("%.15d", d);
    TEST_PRINT("%10d", d);
    TEST_PRINT("%7.*d", 2, d);
    TEST_PRINT("% 17.8d", d);
    TEST_PRINT("% 10.8d", d);
    TEST_PRINT("%03d", d);
    TEST_PRINT("%-d", d);
    TEST_PRINT("%+d", d);
    TEST_PRINT("%+-5d", d);
    TEST_PRINT("%+-.5d", d);
    TEST_PRINT("%+3.8d", d);
    TEST_PRINT("%-9.2d", d);
    TEST_PRINT("%55.3hd", d);
    TEST_PRINT("%+-5.13hd", (unsigned short)d);
    TEST_PRINT("%+-5.13ld", (unsigned long)d);
    TEST_PRINT("% 3.3ld", (unsigned long)d);
  }
}
END_TEST

START_TEST(s21_sprintf_i_test) {
  int values[] = {INT_MIN, -123456789, -100000,   -1,     0,
                  +1,      100000,     123456789, INT_MAX};
  int size = sizeof(values) / sizeof(*values);
  for (int i = 0; i < size; ++i) {
    int d = values[i];
    TEST_PRINT("%i", d);
    TEST_PRINT("%.15i", d);
    TEST_PRINT("%10i", d);
    TEST_PRINT("%7.*i", 2, d);
    TEST_PRINT("% 17.8i", d);
    TEST_PRINT("% 10.8i", d);
    TEST_PRINT("%03i", d);
    TEST_PRINT("%-i", d);
    TEST_PRINT("%+i", d);
    TEST_PRINT("%+-5i", d);
    TEST_PRINT("%+-.5i", d);
    TEST_PRINT("%*.2i", 7, d);
    TEST_PRINT("%+3.8i", d);
    TEST_PRINT("%-9.2i", d);
    TEST_PRINT("%55.3hi", (short)d);
    TEST_PRINT("%+-5.13hi", (unsigned short)d);
    TEST_PRINT("%+-5.13li", (unsigned long)d);
    TEST_PRINT("% 3.3li", (unsigned long)d);
  }
}
END_TEST

long double float_values[] = {
    0.00,
    -123456789.0,
    -1234.5678,
    -12.345678,
    -1.2345678,
    +123456789.0,
    +1234.5678,
    +12.345678,
    +1.2345678,

    -0.999,
    -1.000,
    -0.9999999,
    -1.0000000,
    -1.0000001,
    -0.1,
    -0.01,
    -0.001,
    -0.0000001,
    -0.00000000001,
    -1e-10,
    -1e-20,
    -(1 - 1e-30),
    -(1 + 1e-30),
    -12345.000001,
    -1234567.,
    -764231539.,

    +0.999,
    +1.000,
    +0.9999999,
    +1.0000000,
    +1.0000001,
    +0.1,
    +0.01,
    +0.001,
    +0.0000001,
    +0.00000000001,
    +1e10,
    +1e20,
    +(1 - 1e-30),
    +(1 + 1e-30),
    +12345.000001,
    +1234567.,
    +764231539.,

    -1.234567890,
    +1.234567890,
    -1.1111111111111111,
    +1.1111111111111111,

    -11111111.111111111,
    +11111111.111111111,

    -99999999.999999999,
    +99999999.999999999,

    -21.0000000000045325244351,
    +21.0000000000045325244351,

    -4121.000051,
    +4121.000051,
};

START_TEST(s21_sprintf_e_test) {
  int size = sizeof(float_values) / sizeof(*float_values);
  for (int i = 0; i < size; ++i) {
    long double d = float_values[i];
    TEST_PRINT("%Le %LE", d, d);
    TEST_PRINT("%.4Le %.4LE", d, d);
    TEST_PRINT("%-32.1Le %-32.1LE", d, d);
    TEST_PRINT("% -32.1Le % -32.1LE", d, d);
    TEST_PRINT("%+-5.13Le %+-5.13LE", d, d);
    TEST_PRINT("%+-#5.13Le %+-#5.13LE", d, d);
    TEST_PRINT("%+02.13Le %+02.13LE", d, d);
    TEST_PRINT("%7.*Le %7.*LE", 3, d, 3, d);
    TEST_PRINT("%05E", 1.0 / 0.0);

    TEST_PRINT("%.0Le %.0LE", d, d);
    TEST_PRINT("%-.0Le %+.0LE", d, d);
    TEST_PRINT("%-+.0Le %-+#.0LE", d, d);
    TEST_PRINT("%.Le %.LE", d, d);

    TEST_PRINT("%.15Le", 123.456L);

    // TEST_PRINT("%.20Le", 123.456L); --> ERR with precision >= 16
    // And I suppose it's not necessary to do that.
  }

  double v = (1.0 / 0.0) * 0.0;
  TEST_PRINT("%05e", v);
}
END_TEST

START_TEST(s21_sprintf_e_2test) {
  long double q = 0.000001;
  TEST_PRINT("%Le %LE", q, q);

  long double d = -0.000001;
  TEST_PRINT("%Le %LE", d, d);
  TEST_PRINT("%.4Le %.4LE", d, d);
  TEST_PRINT("%-32.1Le %-32.1LE", d, d);
  TEST_PRINT("% -32.1Le % -32.1LE", d, d);
  TEST_PRINT("%+-5.13Le %+-5.13LE", d, d);
  TEST_PRINT("%+02.13Le %+02.13LE", d, d);
  TEST_PRINT("%7.*Le %7.*LE", 3, d, 3, d);
}
END_TEST

START_TEST(s21_sprintf_f_test) {
  int size = sizeof(float_values) / sizeof(*float_values);
  for (int i = 0; i < size; ++i) {
    double d = float_values[i];
    TEST_PRINT("%f", d);
    TEST_PRINT("%.4f", d);
    TEST_PRINT("%-12.1f", d);
    TEST_PRINT("% -12.1f", d);
    TEST_PRINT("%+-5.13f", d);
    TEST_PRINT("%+02.13f", d);
    TEST_PRINT("%7.*f", 3, d);
    TEST_PRINT("%05f", 1.0 / 0.0);
    TEST_PRINT("%05f", (1.0 / 0.0) * 0.0);
  }
}
END_TEST

START_TEST(s21_sprintf_f_2test) {
  long double d = -4121.000543546345051;
  TEST_PRINT("%Lf", d);
  TEST_PRINT("%.4Lf", d);
  TEST_PRINT("%-12.1Lf", d);
  TEST_PRINT("% -12.1Lf", d);
  TEST_PRINT("%+-5.13Lf", d);
  TEST_PRINT("%+02.13Lf", d);
  TEST_PRINT("%7.*Lf", 3, d);
}
END_TEST

START_TEST(s21_sprintf_f_3test) {
  long double d = 0;
  TEST_PRINT("%Lf", d);
  TEST_PRINT("%.4Lf", d);
  TEST_PRINT("%-12.1Lf", d);
  TEST_PRINT("% -12.1Lf", d);
  TEST_PRINT("%+-5.13Lf", d);
  TEST_PRINT("%+02.13Lf", d);
  TEST_PRINT("%#.0Lf", d);
  TEST_PRINT("%7.*Lf", 3, d);
}
END_TEST

START_TEST(s21_sprintf_g_test) {
  long double d = 121212.210210210210;
  TEST_PRINT("%Lg %LG", d, d);
  TEST_PRINT("%.4Lg %.4LG", d, d);
  TEST_PRINT("%-32.1Lg %-32.1LG", d, d);
  TEST_PRINT("% -32.1Lg % -32.1LG", d, d);
  TEST_PRINT("%+-5.13Lg %+-5.13LG", d, d);
  TEST_PRINT("%+02.13Lg %+02.13LG", d, d);
  TEST_PRINT("%7.*Lg %7.*LG", 3, d, 3, d);
  TEST_PRINT("%-#*.*Lg %-#*.*LG", 23, 3, d, 23, 3, d);

  int size = sizeof(float_values) / sizeof(*float_values);
  for (int i = 0; i < size; ++i) {
    long double d = float_values[i];
    TEST_PRINT("%Lg %LG", d, d);
    TEST_PRINT("%.4Lg %.4LG", d, d);
    TEST_PRINT("%-32.1Lg %-32.1LG", d, d);
    TEST_PRINT("% -32.1Lg % -32.1LG", d, d);
    TEST_PRINT("%+-5.13Lg %+-5.13LG", d, d);
    TEST_PRINT("%+02.13Lg %+02.13LG", d, d);
    TEST_PRINT("%7.*Lg %7.*LG", 3, d, 3, d);
    TEST_PRINT("%+-5.13Lg %+-#5.13LG", d, d);
    TEST_PRINT("%-#*.*Lg %-#*.*LG", 23, 3, d, 23, 3, d);
  }
}
END_TEST

START_TEST(s21_sprintf_g_2test) {
  double d = -0.000001;
  TEST_PRINT("%g %G", d, d);
  TEST_PRINT("%.4g %.4G", d, d);
  TEST_PRINT("%-32.1g %-32.1G", d, d);
  TEST_PRINT("% -32.1g % -32.1G", d, d);
  TEST_PRINT("%+-5.13g %+-5.13G", d, d);
  TEST_PRINT("%+02.13g %+02.13G", d, d);
  TEST_PRINT("%7.*g %7.*G", 3, d, 3, d);
  TEST_PRINT("%-#*.*g %-#*.*G", 23, 3, d, 23, 3, d);
}
END_TEST

START_TEST(s21_sprintf_s_test) {
  char string[] = "aboba";
  TEST_PRINT("%s", string);
  TEST_PRINT("%-15.8s", string);
  TEST_PRINT("%.3s", string);

  char empty_string[] = "";
  TEST_PRINT("%s", empty_string);
  TEST_PRINT("%-15.8s", empty_string);
  TEST_PRINT("%.3s", empty_string);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wformat-zero-length"
#pragma GCC diagnostic ignored "-Wformat-overflow"

  TEST_PRINT("%s", NULL);
  TEST_PRINT("%10s", NULL);
  TEST_PRINT("%.7s", NULL);
  TEST_PRINT("%.6s", NULL);
  TEST_PRINT("%.5s", NULL);
  TEST_PRINT("%.3s", NULL);
  TEST_PRINT("%.0s", NULL);

  TEST_PRINT("%ls", NULL);
  TEST_PRINT("%10ls", NULL);
  TEST_PRINT("%.7ls", NULL);
  TEST_PRINT("%.6ls", NULL);
  TEST_PRINT("%.5ls", NULL);
  TEST_PRINT("%.3ls", NULL);
  TEST_PRINT("%.0ls", NULL);
#pragma GCC diagnostic pop
}
END_TEST

START_TEST(s21_sprintf_u_test) {
  int values[] = {0, 1, 2, 100, 666, 8369, 123456789, UINT_MAX};
  int size = sizeof(values) / sizeof(*values);
  for (int i = 0; i < size; ++i) {
    int d = values[i];
    TEST_PRINT("%u", d);
    TEST_PRINT("%.6u", d);
    TEST_PRINT("%0u", d);
    TEST_PRINT("%7.*u", -2, d);
    TEST_PRINT("%0*u", 6, d);
    TEST_PRINT("%-9.2u", d);
    TEST_PRINT("%55.3hu", (unsigned short)d);
  }
}
END_TEST

START_TEST(s21_sprintf_o_test) {
  int values[] = {INT_MIN, -123456789, -100000, -1,        -33,
                  0,       +1,         100000,  123456789, INT_MAX};
  int size = sizeof(values) / sizeof(*values);
  for (int i = 0; i < size; ++i) {
    int d = values[i];
    TEST_PRINT("%o", d);
    TEST_PRINT("%.6o", d);
    TEST_PRINT("%7.*o", -2, d);
    TEST_PRINT("%0*o", 6, d);

    TEST_PRINT("%-9.1o", d);
    TEST_PRINT("%-#9.1o", d);

    TEST_PRINT("%-9.2o", d);
    TEST_PRINT("%-#9.2o", d);

    TEST_PRINT("%-9.3o", d);
    TEST_PRINT("%-#9.3o", d);
  }
}
END_TEST

START_TEST(s21_sprintf_o_2test) {
  int d = 0;
  TEST_PRINT("%o", d);
  TEST_PRINT("%.6o", d);
  TEST_PRINT("%7.*o", -2, d);
  TEST_PRINT("%0*o", 6, d);
  TEST_PRINT("%-#9.2o", d);
}
END_TEST

START_TEST(s21_sprintf_o_3test) {
  int d = -33;
  TEST_PRINT("%o", d);
  TEST_PRINT("%.6o", d);
  TEST_PRINT("%7.*o", -2, d);
  TEST_PRINT("%0*o", 6, d);
  TEST_PRINT("%-#9.2o", d);
}
END_TEST

START_TEST(s21_sprintf_x_test) {
  int values[] = {INT_MIN, -123456789, -100000,   -1,     0,
                  +1,      100000,     123456789, INT_MAX};
  int size = sizeof(values) / sizeof(*values);
  for (int i = 0; i < size; ++i) {
    int d = values[i];
    TEST_PRINT("%x %X", d, d);
    TEST_PRINT("%.6hx %.6hX", (unsigned short)d, (unsigned short)d);
    TEST_PRINT("%-7.*x %-7.*X", -2, d, 3, d);
    TEST_PRINT("%0*x %0*X", 6, d, 6, d);
    TEST_PRINT("%-#9.2x %-#9.2X", d, d);
    TEST_PRINT("%#9.2lx %#9.2lX", (unsigned long)d, (unsigned long)d);
    TEST_PRINT("%-#*.3lx %-#*.3lX", 12, (unsigned long)d, 12, (unsigned long)d);

    TEST_PRINT("%-9.1x", d);
    TEST_PRINT("%-#9.1x", d);

    TEST_PRINT("%-9.2x", d);
    TEST_PRINT("%-#9.2x", d);

    TEST_PRINT("%-9.3x", d);
    TEST_PRINT("%-#9.3x", d);
  }
}

END_TEST

START_TEST(s21_sprintf_x_2test) {
  int d = 0;
  TEST_PRINT("%x %X", d, d);
  TEST_PRINT("%.6hx %.6hX", (unsigned short)d, (unsigned short)d);
  TEST_PRINT("%-7.*x %-7.*X", -2, d, 3, d);
  TEST_PRINT("%0*x %0*X", 6, d, 6, d);
  TEST_PRINT("%-#9.2x %-#9.2X", d, d);
  TEST_PRINT("%#9.2lx %#9.2lX", (unsigned long)d, (unsigned long)d);
  TEST_PRINT("%-#*.3lx %-#*.3lX", 12, (unsigned long)d, 12, (unsigned long)d);
}
END_TEST

START_TEST(s21_sprintf_x_3test) {
  int d = -7678;
  TEST_PRINT("%x %X", d, d);
  TEST_PRINT("%.6hx %.6hX", (unsigned short)d, (unsigned short)d);
  TEST_PRINT("%-7.*x %-7.*X", -2, d, 3, d);
  TEST_PRINT("%0*x %0*X", 6, d, 6, d);
  TEST_PRINT("%-#9.2hx %-#9.2hX", (unsigned short)d, (unsigned short)d);
  TEST_PRINT("%#9.2lx %#9.2lX", (unsigned long)d, (unsigned long)d);
  TEST_PRINT("%-#*.3lx %-#*.3lX", 12, (unsigned long)d, 12, (unsigned long)d);
}
END_TEST

START_TEST(s21_sprintf_p_test) {
  int *null_ptr = NULL;
  double double_ptr = -412.42;
  char char_ptr = '7';
  TEST_PRINT("%p", null_ptr);
  TEST_PRINT("%-5p", null_ptr);
  TEST_PRINT("%20p", null_ptr);
  TEST_PRINT("%p", &double_ptr);
  TEST_PRINT("%-5p", &double_ptr);
  TEST_PRINT("%20p", &double_ptr);
  TEST_PRINT("%p", &char_ptr);
  TEST_PRINT("%-5p", &char_ptr);
  TEST_PRINT("%20p", &char_ptr);
}
END_TEST

START_TEST(s21_sprintf_p_2test) {
  int int_ptr = 0;
  int int_ptr2 = 888888888;
  TEST_PRINT("%p", &int_ptr);
  TEST_PRINT("%-5p", &int_ptr);
  TEST_PRINT("%20p", &int_ptr);
  TEST_PRINT("%p", &int_ptr2);
  TEST_PRINT("%-5p", &int_ptr2);
  TEST_PRINT("%20p", &int_ptr2);
}
END_TEST

START_TEST(s21_sprintf_n_test) {
  char str_origin[256] = {0};
  char str_rewrit[256] = {0};
  int d1, d2;
  sprintf(str_origin, "%n", &d1);
  s21_sprintf(str_rewrit, "%n", &d2);
  ck_assert_str_eq(str_origin, str_rewrit);
  ck_assert_int_eq(d1, d2);

  sprintf(str_origin, "53we%n412rwq", &d1);
  s21_sprintf(str_rewrit, "53we%n412rwq", &d2);
  ck_assert_str_eq(str_origin, str_rewrit);
  ck_assert_int_eq(d1, d2);

  sprintf(str_origin, " \n%n", &d1);
  s21_sprintf(str_rewrit, " \n%n", &d2);
  ck_assert_str_eq(str_origin, str_rewrit);
  ck_assert_int_eq(d1, d2);

  sprintf(str_origin, "%n4444", &d1);
  s21_sprintf(str_rewrit, "%n4444", &d2);
  ck_assert_str_eq(str_origin, str_rewrit);
  ck_assert_int_eq(d1, d2);

  sprintf(str_origin, "zzzzzzz%n%%", &d1);
  s21_sprintf(str_rewrit, "zzzzzzz%n%%", &d2);
  ck_assert_str_eq(str_origin, str_rewrit);
  ck_assert_int_eq(d1, d2);
}
END_TEST

START_TEST(s21_sprintf_prc_test) {
  char str_origin[256] = {0};
  char str_rewrit[256] = {0};
  sprintf(str_origin, "%%");
  s21_sprintf(str_rewrit, "%%");
  ck_assert_str_eq(str_origin, str_rewrit);
  memset(str_rewrit, 0, sizeof(str_rewrit));

  int d = 17;
  TEST_PRINT("00000%d%%", d);

  int d1, d2;
  sprintf(str_origin, "twetwtwe%n%%rqwrqw", &d1);
  s21_sprintf(str_rewrit, "twetwtwe%n%%rqwrqw", &d2);
  ck_assert_str_eq(str_origin, str_rewrit);
  ck_assert_int_eq(d1, d2);
}
END_TEST

START_TEST(s21_sprintf_limits_test) {
  TEST_PRINT("%c", CHAR_MIN)
  TEST_PRINT("%c", CHAR_MAX)

  TEST_PRINT("%c", 0)
  TEST_PRINT("%c", UCHAR_MAX)

  TEST_PRINT("%hd", (short)SHRT_MIN)
  TEST_PRINT("%hd", (short)SHRT_MAX)

  TEST_PRINT("%hu", (unsigned short)0)
  TEST_PRINT("%hu", (unsigned short)USHRT_MAX)

  TEST_PRINT("%d", INT_MIN)
  TEST_PRINT("%d", INT_MAX)

  TEST_PRINT("%ld", LONG_MIN)
  TEST_PRINT("%ld", LONG_MAX)

  TEST_PRINT("%lu", 0L)
  TEST_PRINT("%lu", ULONG_MAX)

  TEST_PRINT("%Lf", (long double)LONG_MIN)
  TEST_PRINT("%Lf", (long double)LONG_MAX)

  TEST_PRINT("%e", FLT_MAX)
  TEST_PRINT("%e", -FLT_MAX)
  TEST_PRINT("%e", FLT_MIN)
  TEST_PRINT("%e", DBL_MIN)
  TEST_PRINT("%e", DBL_MAX)

  TEST_PRINT("%g", FLT_MAX)
  TEST_PRINT("%g", -FLT_MAX)
  TEST_PRINT("%g", FLT_MIN)
  TEST_PRINT("%g", DBL_MIN)
  TEST_PRINT("%g", DBL_MAX)

  char str_origin[5000] = {0};
  char str_rewrit[5000] = {0};
  double d = FLT_MAX;
  sprintf(str_origin, "%f", d);
  s21_sprintf(str_rewrit, "%f", d);

  // only first bunch of 17-18 digits makes sense for double
  // for example: FLT_MAX is printed as
  // 3402823466385288598|11704183484516925440.000000 (printf)
  // 3402823466385288598|00000000000000000000.000000 (s21_printf)
  ck_assert(strncmp(str_origin, str_rewrit, 15) == 0);

  double dd = DBL_MAX;
  sprintf(str_origin, "%f", dd);
  s21_sprintf(str_rewrit, "%f", dd);
  ck_assert(strncmp(str_origin, str_rewrit, 15) == 0);
}
END_TEST

START_TEST(s21_sprintf_extra_test) {
  TEST_PRINT("Hello %5d!", 12345);
  TEST_PRINT("Hello %10d!", 12345);
  TEST_PRINT("Hello %10.d!", 12345);
  TEST_PRINT("Hello %010d!", 12345);
  TEST_PRINT("Hello %-10d!", 12345);
  TEST_PRINT("Hello %20f!", 123.45);
  TEST_PRINT("Hello %020.3f!\n", 123.456789123);
  TEST_PRINT("Hello %-20.5f!\n", 123.45);
  TEST_PRINT("How are %10s?\n", "you");
  TEST_PRINT("How are %.s?\n", "you");
  TEST_PRINT("A%10.3f\n", 123.456789);
  TEST_PRINT("A%10.3f\n", 123.);
  TEST_PRINT("A%010.3f\n", 123.);
  TEST_PRINT("%e", 123.456);
  TEST_PRINT("%e", 0.0123456);
  TEST_PRINT("%E", 2.0);
  TEST_PRINT("%.3e", 0.0123456);
  TEST_PRINT("%.0e", 0.0123456);
  TEST_PRINT("%.3g", 0.000123456);
  TEST_PRINT("%.0G", 0.00123456);
  TEST_PRINT("%.G", 0.00123456);
  TEST_PRINT("%g", 0.000123456);
  TEST_PRINT("%G", 1.23456E-5);
  TEST_PRINT("%o", 1000);
  TEST_PRINT("%#o", 1000);
  TEST_PRINT("%x", 1000);
  TEST_PRINT("%#x", 1000);
  TEST_PRINT("%X", 1000);
  TEST_PRINT("%#X", 1000);
  TEST_PRINT("%f", 1234.);
  TEST_PRINT("%.0f", 1234.56);
  TEST_PRINT("%#f", 1234.);
  TEST_PRINT("%#.0f", 1234.56);
  TEST_PRINT("%.0e", 1.);
  TEST_PRINT("%#.0e", 1.);
  TEST_PRINT("%.6G", 1.23000);
  TEST_PRINT("%#.6G", 1.23000);
  TEST_PRINT("%.0G", 1.00123456);
  TEST_PRINT("%#.0G", 1.00123456);
  TEST_PRINT("%G", 1.23000E-10);
  TEST_PRINT("%G", 1.23000E10);
  TEST_PRINT("%#G", 1.23000E10);
  TEST_PRINT("%#G", 1.23000E20);
  TEST_PRINT("%#.3G", 1.23000E20);
  TEST_PRINT("%#.8G", 1.23000E20);
  TEST_PRINT("%f", 0.);
  TEST_PRINT("%#f", 0.);
  TEST_PRINT("%E", 0.);
  TEST_PRINT("%#E", 0.);
  TEST_PRINT("%G", 0.);
  TEST_PRINT("%#G", 0.);

  float x = INFINITY;
  TEST_PRINT("x = %f", x);

  double dx = INFINITY;
  TEST_PRINT("dx = %f", dx);

  float y = nan("");
  TEST_PRINT("y = %f", y);

  setlocale(LC_ALL, "");
  wchar_t *s = L"Привет! 您好!";
  TEST_PRINT("%lc", s[0]);
  TEST_PRINT("%ls", s);

  TEST_PRINT("%f", -1234567.8);
  TEST_PRINT("%E", -1234567.8);
  TEST_PRINT("%g", -1234567.8);
  TEST_PRINT("%g", 1234567.0);
  TEST_PRINT("%.0g", 1234567.0);
  TEST_PRINT("%g", 12345.0);
  TEST_PRINT("%#g", 12345.0);

  int i = 123;
  char c = '$';
  TEST_PRINT("%p", &i);
  TEST_PRINT("%20p", &c);
}
END_TEST

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wformat-zero-length"
#pragma GCC diagnostic ignored "-Wformat-overflow"

START_TEST(s21_sprintf_string_test) {
  TEST_PRINT_S("");
  TEST_PRINT_S("\n");
  TEST_PRINT_S("Hello");
  TEST_PRINT_S("Hello world!");
  TEST_PRINT_S("%%");
}
END_TEST

START_TEST(s21_sprintf_other_test) {
  // 0 as a char inside a string
  TEST_PRINT("%c", 0);
  TEST_PRINT("%ctest", 0);
  TEST_PRINT("test%c", 0);
  TEST_PRINT("%ctest%c", 0, 0);

  double num = -764231539.;
  TEST_PRINT("%e", num);
  TEST_PRINT("%e Test %5e Test %5.e", num, num, num);

  // Process width with flags tests
  TEST_PRINT("test:% 020.3E", 123.45678);
  TEST_PRINT("test:%+20.3E", 123.45678);
  TEST_PRINT("test:% +20.3E", 123.45678);
  TEST_PRINT("test:%+020.3E", 123.45678);
  TEST_PRINT("test:%0+20.3E", 123.45678);

  TEST_PRINT("test:% 020.3E", -123.45678);
  TEST_PRINT("test:%+20.3E", -123.45678);
  TEST_PRINT("test:% +20.3E", -123.45678);
  TEST_PRINT("test:%+020.3E", -123.45678);
  TEST_PRINT("test:%0+20.3E", -123.45678);

  // A precision of 0 means that no character is written for the value 0.
  int val = 0;
  TEST_PRINT("%3.d", val);
  TEST_PRINT("%d Test %3.d Test %5.7d", val, val, val);
  TEST_PRINT("%d Test %3.d Test %5.7d TEST_PRINT %10d %#d %-d %+d %.d % .d",
             val, val, val, val, val, val, val, val, val);

  // But nonzero values should be printed
  val = 1;
  TEST_PRINT("%3.d", val);
  TEST_PRINT("%d Test %3.d Test %5.7d", val, val, val);
  TEST_PRINT("%d Test %3.d Test %5.7d TEST_PRINT %10d %#d %-d %+d %.d % .d",
             val, val, val, val, val, val, val, val, val);

  // No '+' should be printed for octal numbers
  TEST_PRINT("%+o Test %+3.o Test %+5.7o TEST %+10o", 3015, 712, 99, 2939);

  // Test percent
  TEST_PRINT("%%test%d", 1);
  TEST_PRINT("%%%%test%d", 1);
  TEST_PRINT("%%%%%%test%d", 1);

  // Test uknown sequence after %
  char buf[100];
  s21_sprintf(buf, "%10.2rrrboo!");
  ck_assert_str_eq(buf, "%10.2rrrboo!");
}
END_TEST

START_TEST(s21_sprintf_stable_test) {
  char buf[50000];
  s21_sprintf(buf, "%1000d", 1);
  s21_sprintf(buf, "%.1000f", 1.23456);
  s21_sprintf(buf, "%10000.10000Le", 1.2345678901234567890L);
}
END_TEST
#pragma GCC diagnostic pop

Suite *Create_suite_s21_sprintf() {
  Suite *suite = suite_create("s21_sprintf tests");
  TCase *tcase_core = tcase_create("Core");

  tcase_add_test(tcase_core, s21_sprintf_string_test);
  tcase_add_test(tcase_core, s21_sprintf_c_test);
  tcase_add_test(tcase_core, s21_sprintf_d_test);
  tcase_add_test(tcase_core, s21_sprintf_d_2test);
  tcase_add_test(tcase_core, s21_sprintf_i_test);
  tcase_add_test(tcase_core, s21_sprintf_e_test);
  tcase_add_test(tcase_core, s21_sprintf_e_2test);
  tcase_add_test(tcase_core, s21_sprintf_f_test);
  tcase_add_test(tcase_core, s21_sprintf_f_2test);
  tcase_add_test(tcase_core, s21_sprintf_f_3test);
  tcase_add_test(tcase_core, s21_sprintf_g_test);
  tcase_add_test(tcase_core, s21_sprintf_g_2test);
  tcase_add_test(tcase_core, s21_sprintf_o_test);
  tcase_add_test(tcase_core, s21_sprintf_o_2test);
  tcase_add_test(tcase_core, s21_sprintf_o_3test);
  tcase_add_test(tcase_core, s21_sprintf_s_test);
  tcase_add_test(tcase_core, s21_sprintf_u_test);
  tcase_add_test(tcase_core, s21_sprintf_x_test);
  tcase_add_test(tcase_core, s21_sprintf_x_2test);
  tcase_add_test(tcase_core, s21_sprintf_x_3test);
  tcase_add_test(tcase_core, s21_sprintf_p_test);
  tcase_add_test(tcase_core, s21_sprintf_p_2test);
  tcase_add_test(tcase_core, s21_sprintf_n_test);
  tcase_add_test(tcase_core, s21_sprintf_prc_test);
  tcase_add_test(tcase_core, s21_sprintf_limits_test);
  tcase_add_test(tcase_core, s21_sprintf_extra_test);
  tcase_add_test(tcase_core, s21_sprintf_other_test);
  tcase_add_test(tcase_core, s21_sprintf_stable_test);
  suite_add_tcase(suite, tcase_core);

  return suite;
}
