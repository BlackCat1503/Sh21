#include <check.h>
#include <stdio.h>

#include "s21_sscanf.h"

START_TEST(s21_sscanf_c_test) {
  char a, b, c;
  char a1, b1, c1;
  sscanf("\naB1&OFEkf32", "%c %*c %c %c  ", &a, &b, &c);
  s21_sscanf("\naB1&OFEkf32", "%c %*c %c %c  ", &a1, &b1, &c1);
  ck_assert_int_eq(a, a1);
  ck_assert_int_eq(b, b1);
  ck_assert_int_eq(c, c1);
}
END_TEST

START_TEST(s21_sscanf_d_test) {
  int a = 0, b = 0, c = 0, f = 0;
  int a1 = 0, b1 = 0, c1 = 0, f1 = 0;
  short e1 = 0;
  short e = 0;
  long d = 0;
  long d1 = 0;
  sscanf("42144 +12 2147483647 320 -12 12", "%3d %d %d %ld %hd %d", &a, &b, &c,
         &d, &e, &f);
  s21_sscanf("42144 +12 2147483647 320 -12 12", "%3d %d %d %ld %hd %d", &a1,
             &b1, &c1, &d1, &e1, &f1);
  ck_assert_int_eq(a, a1);
  ck_assert_int_eq(b, b1);
  ck_assert_int_eq(c, c1);
  ck_assert_int_eq(d, d1);
  ck_assert_int_eq(e, e1);
  ck_assert_int_eq(f, f1);
}
END_TEST

START_TEST(s21_sscanf_i_test) {
  int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0;
  int a1 = 0, b1 = 0, c1 = 0, d1 = 0, e1 = 0, f1 = 0, g1 = 0;
  sscanf("+42144 11112 -64 1324 -12 12", "%i %2i %*i %i %i %i %i %i", &a, &b,
         &c, &d, &e, &f, &g);
  s21_sscanf("+42144 11112 -64 1324 -12 12", "%i %2i %*i %i %i %i %i %i", &a1,
             &b1, &c1, &d1, &e1, &f1, &g1);
  ck_assert_int_eq(a, a1);
  ck_assert_int_eq(b, b1);
  ck_assert_int_eq(c, c1);
  ck_assert_int_eq(d, d1);
  ck_assert_int_eq(e, e1);
  ck_assert_int_eq(f, f1);
  ck_assert_int_eq(g, g1);
}
END_TEST

START_TEST(s21_sscanf_e_test) {
  double e = 0, a = 0, b = 0, c = 0, d = 0;
  long double f = 0, g = 0;
  double e1 = 0, a1 = 0, b1 = 0, c1 = 0, d1 = 0;
  long double f1 = 0, g1 = 0;
  sscanf("-0.000001 0.95 +12.42143 1.1 21.000045325244351 21.442",
         "%le %le %5le %*e %le %*e %*e %LE %15Le", &a, &b, &c, &d, &f, &g);
  s21_sscanf("-0.000001 0.95 +12.42143 1.1 21.000045325244351 21.442",
             "%le %le %5le %*e %le %*e %*e %LE %15Le", &a1, &b1, &c1, &d1, &f1,
             &g1);
  ck_assert_float_eq(a, a1);
  ck_assert_float_eq(b, b1);
  ck_assert_float_eq(c, c1);
  ck_assert_float_eq(d, d1);
  ck_assert_double_eq(e, e1);
  ck_assert_ldouble_eq(f, f1);
  ck_assert_ldouble_eq(g, g1);

  // check exp
  char buf[100];
  char *ccp = buf;
  int tmp = 0;
  __check_exponential_form__("e-", &ccp, &tmp);
  __check_exponential_form__("e+", &ccp, &tmp);
  // check exp
}
END_TEST

START_TEST(s21_sscanf_f_test) {
  double e = 0, a = 0, b = 0, c = 0, d = 0;
  long double f = 0, g = 0;
  double e1 = 0, a1 = 0, b1 = 0, c1 = 0, d1 = 0;
  long double f1 = 0, g1 = 0;
  sscanf("-0.000001 0.95 +12.42143 1.1 21.000045325244351 21.442",
         "%lf %lf %5lf %*f %lf %*f %*f %Lf %15Lf", &a, &b, &c, &d, &f, &g);
  s21_sscanf("-0.000001 0.95 +12.42143 1.1 21.000045325244351 21.442",
             "%lf %lf %5lf %*f %lf %*f %*f %Lf %15Lf", &a1, &b1, &c1, &d1, &f1,
             &g1);
  ck_assert_float_eq(a, a1);
  ck_assert_float_eq(b, b1);
  ck_assert_float_eq(c, c1);
  ck_assert_float_eq(d, d1);
  ck_assert_double_eq(e, e1);
  ck_assert_ldouble_eq(f, f1);
  ck_assert_ldouble_eq(g, g1);

  // special test for __converted_point__ function
  char buf[100] = "123";
  char *p = buf;
  double res = __converter_point__(-3, p, 10);
  ck_assert_double_eq_tol(res, 0.123, 1e-3);
}
END_TEST

START_TEST(s21_sscanf_g_test) {
  double e = 0, a = 0, b = 0, c = 0, d = 0;
  long double f = 0, g = 0;
  double e1 = 0, a1 = 0, b1 = 0, c1 = 0, d1 = 0;
  long double f1 = 0, g1 = 0;
  sscanf("-0.000001 0.95 +12.42143 1.1 21.000045325244351 21.442",
         "%lG %lg %5lg %*g %lg %*g %*g %Lg %15Lg", &a, &b, &c, &d, &f, &g);
  s21_sscanf("-0.000001 0.95 +12.42143 1.1 21.000045325244351 21.442",
             "%lG %lg %5lg %*g %lg %*g %*g %Lg %15Lg", &a1, &b1, &c1, &d1, &f1,
             &g1);
  ck_assert_float_eq(a, a1);
  ck_assert_float_eq(b, b1);
  ck_assert_float_eq(c, c1);
  ck_assert_float_eq(d, d1);
  ck_assert_double_eq(e, e1);
  ck_assert_ldouble_eq(f, f1);
  ck_assert_ldouble_eq(g, g1);
}
END_TEST

START_TEST(s21_sscanf_o_test) {
  int a = 0, b = 0, c = 0, e = 0, f = 0;
  int a1 = 0, b1 = 0, c1 = 0, e1 = 0, f1 = 0;
  long d = 0, d1 = 0;
  sscanf("42144 +12 -64 21474 -12 12", "%3o %*o %o %o %lo %o %o", &a, &b, &c,
         &d, &e, &f);
  s21_sscanf("42144 +12 -64 21474 -12 12", "%3o %*o %o %o %lo %o %o", &a1, &b1,
             &c1, &d1, &e1, &f1);
  ck_assert_int_eq(a, a1);
  ck_assert_int_eq(b, b1);
  ck_assert_int_eq(c, c1);
  ck_assert_int_eq(d, d1);
  ck_assert_int_eq(e, e1);
  ck_assert_int_eq(f, f1);

  short s1, s2;
  sscanf("123", "%ho", &s1);
  s21_sscanf("123", "%ho", &s2);
  ck_assert_int_eq(s1, s2);
}
END_TEST

START_TEST(s21_sscanf_s_test) {
  char a[128] = "\0", b[128] = "\0", c[128] = "\0", d[128] = "\0";
  char a1[128] = "\0", b1[128] = "\0", c1[128] = "\0", d1[128] = "\0";
  sscanf("Q\nqQ q\tqq aaa", "%*s %s %s %s%s", a, b, c, d);
  s21_sscanf("Q\nqQ q\tqq aaa", "%*s %s %s %s%s", a1, b1, c1, d1);
  ck_assert_str_eq(a, a1);
  ck_assert_str_eq(b, b1);
  ck_assert_str_eq(c, c1);
  ck_assert_str_eq(d, d1);
}
END_TEST

START_TEST(s21_sscanf_u_test) {
  unsigned int a = 0, b = 0, c = 0, e = 0, f = 0, g = 0;
  unsigned int a1 = 0, b1 = 0, c1 = 0, e1 = 0, f1 = 0, g1 = 0;
  unsigned long d = 0, d1 = 0;
  sscanf("+42144 11112 -64 4294967295 -12 12", "%u %u %u %lu %u %u %u", &a, &b,
         &c, &d, &e, &f, &g);
  s21_sscanf("+42144 11112 -64 4294967295 -12 12", "%u %u %u %lu %u %u %u", &a1,
             &b1, &c1, &d1, &e1, &f1, &g1);
  ck_assert_uint_eq(a, a1);
  ck_assert_uint_eq(b, b1);
  ck_assert_uint_eq(c, c1);
  ck_assert_uint_eq(d, d1);
  ck_assert_uint_eq(e, e1);
  ck_assert_uint_eq(f, f1);
  ck_assert_uint_eq(g, g1);
}
END_TEST

START_TEST(s21_sscanf_x_test) {
  unsigned int a = 0, b = 0, c = 0, d = 0, f = 0, g = 0;
  unsigned long long e = 0, e1 = 0;
  unsigned int a1 = 0, b1 = 0, c1 = 0, d1 = 0, f1 = 0, g1 = 0;
  sscanf("+42144 1A112 -b64 0x1324 -12 12", "%x %2X %*X %x %x %llX %x %X", &a,
         &b, &c, &d, &e, &f, &g);
  s21_sscanf("+42144 1A112 -b64 0x1324 -12 12", "%x %2X %*X %x %x %llX %x %X",
             &a1, &b1, &c1, &d1, &e1, &f1, &g1);
  ck_assert_int_eq(a, a1);
  ck_assert_int_eq(b, b1);
  ck_assert_int_eq(c, c1);
  ck_assert_int_eq(d, d1);
  ck_assert_int_eq(e, e1);
  ck_assert_int_eq(f, f1);
  ck_assert_int_eq(g, g1);
}
END_TEST

START_TEST(s21_sscanf_p_test) {
  void *a = NULL, *b = NULL, *c = NULL, *d = NULL, *f = NULL, *g = NULL;
  void *a1 = NULL, *b1 = NULL, *c1 = NULL, *d1 = NULL, *f1 = NULL, *g1 = NULL;
  sscanf(
      "0x7ffe7332168c 0x1a2b3c 0xffffffffffff 0x1324 0x7ffe7332168c "
      "0x7ffe7332168c",
      "%p %5p %*p %p %p %p %2p", &a, &b, &c, &d, &f, &g);
  s21_sscanf(
      "0x7ffe7332168c 0x1a2b3c 0xffffffffffff 0x1324 0x7ffe7332168c "
      "0x7ffe7332168c",
      "%p %5p %*p %p %p %p %2p", &a1, &b1, &c1, &d1, &f1, &g1);
  ck_assert_ptr_eq(a, a1);
  ck_assert_ptr_eq(b, b1);
  ck_assert_ptr_eq(c, c1);
  ck_assert_ptr_eq(d, d1);
  ck_assert_ptr_eq(f, f1);
  ck_assert_ptr_eq(g, g1);
}
END_TEST

START_TEST(s21_sscanf_prc_test) {
  char b[32] = "\0", d[32] = "\0";
  char b1[32] = "\0", d1[32] = "\0";
  sscanf("twe\nrqw\fq%%w.&12", "%*s %s%s%%", b, d);
  s21_sscanf("twe\nrqw\fq%%w.&12", "%*s %s%s%%", b1, d1);
  ck_assert_str_eq(b, b1);
  ck_assert_str_eq(d, d1);
}
END_TEST

START_TEST(s21_sscanf_n_test) {
  void *a = NULL, *b = NULL, *c = NULL, *d = NULL, *f = NULL, *g = NULL;
  void *a1 = NULL, *b1 = NULL, *c1 = NULL, *d1 = NULL, *f1 = NULL, *g1 = NULL;
  int n_counter = 0, n_counter1 = 0;
  sscanf(
      "0x7ffe7332168c 0x1a2b3c 0xffffffffffff 0x1324 0x7ffe7332168c %n "
      "0x7ffe7332168c",
      "%p %5p %*p %p %p %p %2p %n", &a, &b, &c, &d, &f, &g, &n_counter);
  s21_sscanf(
      "0x7ffe7332168c 0x1a2b3c 0xffffffffffff 0x1324 0x7ffe7332168c %n "
      "0x7ffe7332168c",
      "%p %5p %*p %p %p %p %2p %n", &a1, &b1, &c1, &d1, &f1, &g1, &n_counter1);
  ck_assert_int_eq(n_counter, n_counter1);
}
END_TEST

START_TEST(s21_sscanf_extra_test) {
  int a = 0, a1 = 0;
  unsigned int b = 0, b1 = 0;
  char c[128] = "\0", c1[128] = "\0";
  char d, d1;

  sscanf("123 423 abc d", "%d %u %s %c", &a, &b, c, &d);
  s21_sscanf("123 423 abc d", "%d %u %s %c", &a1, &b1, c1, &d1);

  ck_assert_int_eq(a, a1);
  ck_assert_uint_eq(b, b1);
  ck_assert_str_eq(c, c1);
  ck_assert_int_eq(d, d1);
}
END_TEST

START_TEST(s21_sscanf_extra_str_1_test) {
  char a[128] = "\0", a1[128] = "\0";

  sscanf("\nqQ", "%s", a);
  s21_sscanf("\nqQ", "%s", a1);

  ck_assert_str_eq(a, a1);
}
END_TEST

START_TEST(s21_sscanf_extra_str_2_test) {
  char a[128] = "\0", a1[128] = "\0";

  sscanf("", "%s", a);
  s21_sscanf("", "%s", a1);

  ck_assert_str_eq(a, a1);
}
END_TEST

START_TEST(s21_sscanf_extra_str_3_test) {
  char a[128] = "\0", a1[128] = "\0";

  sscanf("\n", "%s", a);
  s21_sscanf("\n", "%s", a1);

  ck_assert_str_eq(a, a1);
}
END_TEST

START_TEST(s21_sscanf_extra_str_4_test) {
  char a[128] = "\0";
  char a1[128];
  sscanf("\n", "%s", a);
  s21_sscanf("\n", "%s", a1);

  ck_assert_str_eq(a, a1);
}
END_TEST

#define TEST_SCAN_int(str, format) \
  {                                \
    int a = 0;                     \
    int a1 = 0;                    \
    s21_sscanf(str, format, &a);   \
    sscanf(str, format, &a1);      \
    ck_assert_int_eq(a, a1);       \
  }

START_TEST(s21_sscanf_extra_int_test) {
  TEST_SCAN_int("1", "%d");
  TEST_SCAN_int("123", "%d");
  TEST_SCAN_int("-123", "%d");
  TEST_SCAN_int(" 12345 ", "%d");
}
END_TEST

#define TEST_SCAN_float(str, format)     \
  {                                      \
    float a = 0;                         \
    float a1 = 0;                        \
    s21_sscanf(str, format, &a);         \
    sscanf(str, format, &a1);            \
    ck_assert_float_eq_tol(a, a1, 1e-3); \
  }

START_TEST(s21_sscanf_extra_float_test) {
  TEST_SCAN_float("+123.45", "%f");
  TEST_SCAN_float("-123.45", "%f");
  TEST_SCAN_float("123.45", "%f");
  TEST_SCAN_float(" -123.45", "%f");
}
END_TEST

#define TEST_SCAN_double(str, format)    \
  {                                      \
    double a = 0;                        \
    double a1 = 0;                       \
    s21_sscanf(str, format, &a);         \
    sscanf(str, format, &a1);            \
    ck_assert_float_eq_tol(a, a1, 1e-7); \
  }

START_TEST(s21_sscanf_extra_double_test) {
  TEST_SCAN_double("+123.45", "%lf");
  TEST_SCAN_double("-123.45", "%lf");
  TEST_SCAN_double("123.45", "%lf");
  TEST_SCAN_double(" -123.45", "%lf");
}
END_TEST

#define TEST_SCAN_long_double(str, format) \
  {                                        \
    long double a = 0;                     \
    long double a1 = 0;                    \
    s21_sscanf(str, format, &a);           \
    sscanf(str, format, &a1);              \
    ck_assert_float_eq_tol(a, a1, 1e-7);   \
  }

START_TEST(s21_sscanf_extra_long_double_test) {
  TEST_SCAN_long_double("+123.45", "%Lf");
  TEST_SCAN_long_double("-123.45", "%Lf");
  TEST_SCAN_long_double("123.45", "%Lf");
  TEST_SCAN_long_double(" -123.45", "%Lf");
}
END_TEST

START_TEST(s21_sscanf_extra_double_zero_test) {
  TEST_SCAN_double("0", "%lf");
  TEST_SCAN_double("-0", "%lf");
  TEST_SCAN_double("+0", "%lf");
}
END_TEST

Suite *Create_suite_s21_sscanf() {
  Suite *suite = suite_create("s21_sscanf tests");
  TCase *tcase_core = tcase_create("Core");
  tcase_add_test(tcase_core, s21_sscanf_c_test);
  tcase_add_test(tcase_core, s21_sscanf_d_test);
  tcase_add_test(tcase_core, s21_sscanf_i_test);
  tcase_add_test(tcase_core, s21_sscanf_e_test);
  tcase_add_test(tcase_core, s21_sscanf_f_test);
  tcase_add_test(tcase_core, s21_sscanf_g_test);
  tcase_add_test(tcase_core, s21_sscanf_o_test);
  tcase_add_test(tcase_core, s21_sscanf_s_test);
  tcase_add_test(tcase_core, s21_sscanf_u_test);
  tcase_add_test(tcase_core, s21_sscanf_x_test);
  tcase_add_test(tcase_core, s21_sscanf_p_test);
  tcase_add_test(tcase_core, s21_sscanf_prc_test);
  tcase_add_test(tcase_core, s21_sscanf_n_test);
  tcase_add_test(tcase_core, s21_sscanf_extra_test);
  tcase_add_test(tcase_core, s21_sscanf_extra_str_1_test);
  tcase_add_test(tcase_core, s21_sscanf_extra_str_2_test);
  tcase_add_test(tcase_core, s21_sscanf_extra_str_3_test);
  tcase_add_test(tcase_core, s21_sscanf_extra_str_4_test);
  tcase_add_test(tcase_core, s21_sscanf_extra_int_test);
  tcase_add_test(tcase_core, s21_sscanf_extra_float_test);
  tcase_add_test(tcase_core, s21_sscanf_extra_double_test);
  tcase_add_test(tcase_core, s21_sscanf_extra_long_double_test);
  tcase_add_test(tcase_core, s21_sscanf_extra_double_zero_test);
  suite_add_tcase(suite, tcase_core);
  return suite;
}

// int main(void) {
//   Suite *suite = Create_suite();
//   SRunner *suite_runner = srunner_create(suite);
//   srunner_run_all(suite_runner, CK_NORMAL);
//   int failed_count = srunner_ntests_failed(suite_runner);
//   srunner_free(suite_runner);
//   if (failed_count != 0) {
//     printf("Failed tests: %d\n", failed_count);
//     return EXIT_FAILURE;
//   } else {
//     printf("Success\n");
//   }
//   return EXIT_SUCCESS;
// }
