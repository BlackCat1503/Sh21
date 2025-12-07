#include <check.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_string.h"

#define BUF_SIZE 1024
#define STD_BUF std_buf
#define S21_BUF s21_buf

#define TEST_RETURN(func, ...) \
  { ck_assert(func(__VA_ARGS__) == s21_##func(__VA_ARGS__)); }

int sign(int value) {
  int res = 0;
  if (value < 0)
    res = -1;
  else if (value > 0)
    res = +1;
  return res;
}

#define TEST_RETURN_SIGN(func, ...) \
  { ck_assert(sign(func(__VA_ARGS__)) == sign(s21_##func(__VA_ARGS__))); }

START_TEST(s21_memchr_test) {
  TEST_RETURN(memchr, "ABCDABCD", 'C', 8);
  TEST_RETURN(memchr, "ABCDABCD", 'C', 3);
  TEST_RETURN(memchr, "ABCDABCD", 'C', 2);
  TEST_RETURN(memchr, "ABCDABCD", 'X', 8);
}
END_TEST

START_TEST(s21_memcmp_test) {
  TEST_RETURN_SIGN(memcmp, "", "", 1);
  TEST_RETURN_SIGN(memcmp, "A", "A", 1);
  TEST_RETURN_SIGN(memcmp, "A", "B", 1);
  TEST_RETURN_SIGN(memcmp, "B", "A", 1);
  TEST_RETURN_SIGN(memcmp, "ABC_E", "ABC_E", 5);
  TEST_RETURN_SIGN(memcmp, "ABC_A", "ABC_B", 5);
  TEST_RETURN_SIGN(memcmp, "ABC_B", "ABC_A", 5);
  TEST_RETURN_SIGN(memcmp, "ABC", "XYZ", 3);
}
END_TEST

#define TEST_MEM(func, src, n)             \
  {                                        \
    char std_buf[BUF_SIZE];                \
    char s21_buf[BUF_SIZE];                \
    func(std_buf, src, n);                 \
    s21_##func(s21_buf, src, n);           \
    ck_assert_mem_eq(std_buf, s21_buf, n); \
  }

START_TEST(s21_memcpy_test) {
  TEST_MEM(memcpy, "", 0);
  TEST_MEM(memcpy, "", 1);
  TEST_MEM(memcpy, "A", 0);
  TEST_MEM(memcpy, "A", 1);
  TEST_MEM(memcpy, "ABCDEF", 2);
  TEST_MEM(memcpy, "ABCDEF", 3);
  TEST_MEM(memcpy, "ABCDEF", 6);
  TEST_MEM(memcpy, "ABCDEF", 7);
}
END_TEST

START_TEST(s21_memmove_test) {
  // test memmove with intersection
  char std_buf[BUF_SIZE] = "Hello world!";
  char s21_buf[BUF_SIZE] = "Hello world!";
  memmove(std_buf, std_buf + 6, 5);
  s21_memmove(s21_buf, s21_buf + 6, 5);
  ck_assert_mem_eq(std_buf, s21_buf, 12);

  strcpy(std_buf, "Hello world!");
  strcpy(s21_buf, "Hello world!");
  memmove(std_buf + 6, std_buf, 5);
  s21_memmove(s21_buf + 6, s21_buf, 5);
  ck_assert_mem_eq(std_buf, s21_buf, 12);

  strcpy(std_buf, "Hello world!");
  strcpy(s21_buf, "Hello world!");
  memmove(std_buf + 6, std_buf + 6, 5);
  s21_memmove(s21_buf + 6, s21_buf + 6, 5);
  ck_assert_mem_eq(std_buf, s21_buf, 12);

  strcpy(std_buf, "ABCDEF12345");
  strcpy(s21_buf, "ABCDEF12345");
  memmove(std_buf + 3, std_buf + 4, 3);
  s21_memmove(s21_buf + 3, s21_buf + 4, 3);
  ck_assert_mem_eq(std_buf, s21_buf, 10);

  strcpy(std_buf, "ABCDEF12345");
  strcpy(s21_buf, "ABCDEF12345");
  memmove(std_buf + 5, std_buf + 4, 3);
  s21_memmove(s21_buf + 5, s21_buf + 4, 3);
  ck_assert_mem_eq(std_buf, s21_buf, 10);

  // test memmove without intersection (like memcpy)
  TEST_MEM(memmove, "", 0);
  TEST_MEM(memmove, "", 1);
  TEST_MEM(memmove, "A", 0);
  TEST_MEM(memmove, "A", 1);
  TEST_MEM(memmove, "ABCDEF", 2);
  TEST_MEM(memmove, "ABCDEF", 3);
  TEST_MEM(memmove, "ABCDEF", 6);
  TEST_MEM(memmove, "ABCDEF", 7);
}
END_TEST

START_TEST(s21_memset_test) {
  TEST_MEM(memset, 0, 0);
  TEST_MEM(memset, 0, 1);
  TEST_MEM(memset, 0, 10);
  TEST_MEM(memset, 0, 100);
  TEST_MEM(memset, '#', 100);
}
END_TEST

#define TEST_CAT(src)                   \
  {                                     \
    strcat(STD_BUF, src);               \
    s21_strcat(S21_BUF, src);           \
    ck_assert_str_eq(STD_BUF, S21_BUF); \
  }

START_TEST(s21_strcat_test) {
  char STD_BUF[BUF_SIZE] = "INIT_STR_";
  char S21_BUF[BUF_SIZE] = "INIT_STR_";
  TEST_CAT("str1_");
  TEST_CAT("string2\0tail");
  TEST_CAT("string3");
  TEST_CAT(" a bit more and that's it!");
}
END_TEST

#define TEST_NCAT(src, n)               \
  {                                     \
    strncat(STD_BUF, src, n);           \
    s21_strncat(S21_BUF, src, n);       \
    ck_assert_str_eq(STD_BUF, S21_BUF); \
  }

START_TEST(s21_strncat_test) {
  char STD_BUF[BUF_SIZE] = "INIT_STR_";
  char S21_BUF[BUF_SIZE] = "INIT_STR_";
  TEST_NCAT("str1_", 10);
  TEST_NCAT("string2\0tail", 10);
  TEST_NCAT("string3", 3);
  TEST_NCAT(" a bit more and that's it!", 50);
}
END_TEST

#define TEST_STR(func, src)             \
  {                                     \
    char std_buf[BUF_SIZE];             \
    char s21_buf[BUF_SIZE];             \
    func(std_buf, src);                 \
    s21_##func(s21_buf, src);           \
    ck_assert_str_eq(std_buf, s21_buf); \
  }

START_TEST(s21_strcpy_test) {
  TEST_STR(strcpy, "");
  TEST_STR(strcpy, "");
  TEST_STR(strcpy, "A");
  TEST_STR(strcpy, "AB");
  TEST_STR(strcpy, "ABC");
  TEST_STR(strcpy, "ABC\0DEF");
  TEST_STR(strcpy, "ABC\nDEF");
  TEST_STR(strcpy, "ABCDEF\r123");
}
END_TEST

START_TEST(s21_strncpy_test) {
  TEST_MEM(strncpy, "", 0);
  TEST_MEM(strncpy, "", 1);
  TEST_MEM(strncpy, "A", 0);
  TEST_MEM(strncpy, "A", 1);
  TEST_MEM(strncpy, "ABCDEF", 0);
  TEST_MEM(strncpy, "ABCDEF", 2);
  TEST_MEM(strncpy, "ABCDEF", 3);
  TEST_MEM(strncpy, "ABCDEF", 6);
  TEST_MEM(strncpy, "ABCDEF", 7);
}
END_TEST

START_TEST(s21_strchr_test) {
  TEST_RETURN(strchr, "abcdefABCDEF_01234567890abcdef", 'x');
  TEST_RETURN(strchr, "abcdefABCDEF_01234567890abcdef", 'a');
  TEST_RETURN(strchr, "abcdefABCDEF_01234567890abcdef", '0');
  TEST_RETURN(strchr, "abcdefABCDEF_01234567890abcdef", ' ');
  TEST_RETURN(strchr, "abcdefABCDEF_01234567890abcdef", '_');
  TEST_RETURN(strchr, "abcdefABCDEF_01234567890abcdef", '\0');
  TEST_RETURN(strchr, "abcdefABCDEF_01234567890abcdef\0abcdef", '\0');
  TEST_RETURN(strchr, "abcdefABCDEF_01234567890abcdef\nabcdef", '\n');
  TEST_RETURN(strchr, "", 'x');
  TEST_RETURN(strchr, "abcdefABCDEF_01234567890abcdef", '3');
}
END_TEST

START_TEST(s21_strcmp_test) {
  TEST_RETURN_SIGN(strcmp, "", "");
  TEST_RETURN_SIGN(strcmp, "", "A");
  TEST_RETURN_SIGN(strcmp, "A", "");
  TEST_RETURN_SIGN(strcmp, "A", "A");
  TEST_RETURN_SIGN(strcmp, "A", "B");
  TEST_RETURN_SIGN(strcmp, "B", "A");
  TEST_RETURN_SIGN(strcmp, "ABC_E", "ABC_E");
  TEST_RETURN_SIGN(strcmp, "ABC_A", "ABC_B");
  TEST_RETURN_SIGN(strcmp, "ABC_B", "ABC_A");
}
END_TEST

START_TEST(s21_strncmp_test) {
  TEST_RETURN_SIGN(strncmp, "", "", 1);
  TEST_RETURN_SIGN(strncmp, "A", "A", 1);
  TEST_RETURN_SIGN(strncmp, "A", "B", 1);
  TEST_RETURN_SIGN(strncmp, "B", "A", 1);
  TEST_RETURN_SIGN(strncmp, "ABC_E", "ABC_E", 5);
  TEST_RETURN_SIGN(strncmp, "ABC_A", "ABC_B", 5);
  TEST_RETURN_SIGN(strncmp, "ABC_B", "ABC_A", 5);
  TEST_RETURN_SIGN(strncmp, "ABC_EFG", "ABC_XYZ", 5);
  TEST_RETURN_SIGN(strncmp, "ABC_EFG", "ABC_XYZ", 4);
  TEST_RETURN_SIGN(strncmp, "ABC_EFG", "ABC_XYZ", 1);
  TEST_RETURN_SIGN(strncmp, "ABC_EFG", "ABC_XYZ", 0);
}
END_TEST

START_TEST(s21_strlen_test) {
  TEST_RETURN(strlen, "");
  TEST_RETURN(strlen, " ");
  TEST_RETURN(strlen, "0");
  TEST_RETURN(strlen, "A");
  TEST_RETURN(strlen, "AB");
  TEST_RETURN(strlen, "ABCDEFG");
  TEST_RETURN(strlen, "ABC\0DEF\0XYZ");
}
END_TEST

START_TEST(s21_strrchr_test) {
  TEST_RETURN(strrchr, "abcdefABCDEF_01234567890abcdef", 'x');
  TEST_RETURN(strrchr, "abcdefABCDEF_01234567890abcdef", 'a');
  TEST_RETURN(strrchr, "abcdefABCDEF_01234567890abcdef", '0');
  TEST_RETURN(strrchr, "abcdefABCDEF_01234567890abcdef", '3');
  TEST_RETURN(strrchr, "abcdefABCDEF_01234567890abcdef", ' ');
  TEST_RETURN(strrchr, "abcdefABCDEF_01234567890abcdef", '_');
  TEST_RETURN(strrchr, "abcdefABCDEF_01234567890abcdef", '\0');
  TEST_RETURN(strrchr, "abcdefABCDEF_01234567890abcdef\0abcdef", '\0');
  TEST_RETURN(strrchr, "abcdefABCDEF_01234567890abcdef\nabcdef", '\n');
  TEST_RETURN(strrchr, "abcdefABCDEF_01234567890abcdef\0abcdef", 'a');
  TEST_RETURN(strrchr, "abcdefABCDEF_01234567890abcdef\0abcdef", '_');
  TEST_RETURN(strrchr, "", 'x');
}
END_TEST

START_TEST(s21_strcspn_test) {
  TEST_RETURN(strcspn, "abcdefghi", "");
  TEST_RETURN(strcspn, "abcdefghi", "abc");
  TEST_RETURN(strcspn, "abcdefghi", "bcdefghi");
  TEST_RETURN(strcspn, "abcdefghi", "dcba");
  TEST_RETURN(strcspn, "abcdefghi", "abcdefghi");
  TEST_RETURN(strcspn, "abcdefghi", "cabdfg");
  TEST_RETURN(strcspn, "abcdefghi", "efghi");
  TEST_RETURN(strcspn, "abcde312$#@", "wertyuiopasdfghjklzxcvbnm");
  TEST_RETURN(strcspn, "abc", "abcdefgh");
  TEST_RETURN(strcspn, "abc", "hgfedcba");
  TEST_RETURN(strcspn, "abc", "hgfbceda");
}
END_TEST

START_TEST(s21_strpbrk_test) {
  TEST_RETURN(strspn, "abcdefghi", "");
  TEST_RETURN(strspn, "abcdefghi", "a");
  TEST_RETURN(strspn, "abcdefghi", "i");
  TEST_RETURN(strspn, "abcdefghi", "ai");
  TEST_RETURN(strspn, "abcdefghi", "ia");
  TEST_RETURN(strspn, "abcdefghi", "def");
  TEST_RETURN(strspn, "abcdefghi", "fed");
  TEST_RETURN(strspn, "", "");
  TEST_RETURN(strspn, "", "abc");
  TEST_RETURN(strspn, "hello world friend of mine!", ",!");
  TEST_RETURN(strspn, "hello world, friend of mine!", ",!");
}
END_TEST

START_TEST(s21_strspn_test) {
  TEST_RETURN(strspn, "abcdefghi", "");
  TEST_RETURN(strspn, "abcdefghi", "abc");
  TEST_RETURN(strspn, "abcdefghi", "bcdefghi");
  TEST_RETURN(strspn, "abcdefghi", "dcba");
  TEST_RETURN(strspn, "abcdefghi", "abcdefghi");
  TEST_RETURN(strspn, "abcdefghi", "cabdfg");
  TEST_RETURN(strspn, "abcdefghi", "efghi");
  TEST_RETURN(strspn, "abcde312$#@", "wertyuiopasdfghjklzxcvbnm");
  TEST_RETURN(strspn, "abc", "abcdefgh");
  TEST_RETURN(strspn, "abc", "hgfedcba");
  TEST_RETURN(strspn, "abc", "hgfbceda");
}
END_TEST

START_TEST(s21_strstr_test) {
  /*
      const char *str = "";
      char *p1 = strstr(str, "");
      char *p2 = s21_strstr(str, "");
      printf("std:%p, %ld\n", p1, p1 - str);
      printf("s21:%p, %ld\n", p2, p2 - str);
      fflush(stdout);
  */

  TEST_RETURN(strstr, "", "");
  TEST_RETURN(strstr, "", "a");
  TEST_RETURN(strstr, "", "abc");
  TEST_RETURN(strstr, "", "\0");

  TEST_RETURN(strstr, "a", "");
  TEST_RETURN(strstr, "a", "a");
  TEST_RETURN(strstr, "abc", "abc");
  TEST_RETURN(strstr, "abc", "bc");
  TEST_RETURN(strstr, "abc", "c");
  TEST_RETURN(strstr, "abc", "");

  TEST_RETURN(strstr, "abcdefabcdef", "abc");
  TEST_RETURN(strstr, "abcdefabcdef", "bcd");
  TEST_RETURN(strstr, "abcdefabcdef", "cde");
  TEST_RETURN(strstr, "abcdefabcdef", "fab");
  TEST_RETURN(strstr, "abcdefabcdef", "aaa");
  TEST_RETURN(strstr, "abcdefabcdef", "xyz");
  TEST_RETURN(strstr, "abcdefabcdef", "\0");
  TEST_RETURN(strstr, "\0", "\0");

  TEST_RETURN(strstr, "absaabsasa", "absa");

  TEST_RETURN(strstr, "0123456789", "0123456789");
  TEST_RETURN(strstr, "0123456789", "123456789x");
  TEST_RETURN(strstr, "0123456789", "123456789");
  TEST_RETURN(strstr, "0123456789", "23456789x");
  TEST_RETURN(strstr, "0123456789", "3456789");
  TEST_RETURN(strstr, "0123456789", "3456789x");
  TEST_RETURN(strstr, "0123456789", "456789");
  TEST_RETURN(strstr, "0123456789", "456789x");
  TEST_RETURN(strstr, "0123456789", "56789");
  TEST_RETURN(strstr, "0123456789", "56789x");
  TEST_RETURN(strstr, "0123456789", "6789");
  TEST_RETURN(strstr, "0123456789", "6789x");
  TEST_RETURN(strstr, "0123456789", "789");
  TEST_RETURN(strstr, "0123456789", "789x");
  TEST_RETURN(strstr, "0123456789", "89");
  TEST_RETURN(strstr, "0123456789", "89x");
  TEST_RETURN(strstr, "0123456789", "9");
  TEST_RETURN(strstr, "0123456789", "9x");

  TEST_RETURN(strstr, "wwoworworkwork", "work");
}
END_TEST

#define TEST_TOKENS(str, delim)                 \
  {                                             \
    char std_str[100];                          \
    char s21_str[100];                          \
    strcpy(std_str, str);                       \
    strcpy(s21_str, str);                       \
    char *std_tok = strtok(std_str, delim);     \
    char *s21_tok = s21_strtok(s21_str, delim); \
    while (std_tok || s21_tok) {                \
      ck_assert_str_eq(std_tok, s21_tok);       \
      std_tok = strtok(NULL, delim);            \
      s21_tok = s21_strtok(NULL, delim);        \
    }                                           \
    ck_assert_ptr_eq(std_tok, s21_tok);         \
  }

START_TEST(s21_strtok_test) {
  TEST_TOKENS("Hello world of C programming", " ");
  TEST_TOKENS(" Hello world of C programming", " ");
  TEST_TOKENS("Hello world of C programming ", " ");
  TEST_TOKENS("Hello world of C programming", "");
  TEST_TOKENS("Hello-world_of\nC\tprogramming", "-_\n\t");
  TEST_TOKENS("1    12  123    1234", " ");
  TEST_TOKENS(" 1 12 123 1234 ", " ");
  TEST_TOKENS("--1---12-123--1234--", "-");
  TEST_TOKENS("one,two;three.four", ",.;");
  TEST_TOKENS(";one,,,two,.;three.;.four.", ",.;");
  TEST_TOKENS(",;;one,,,two...hree,.;four,;.", ",.;");

  TEST_TOKENS("", " ");
  TEST_TOKENS("   ", " ");
  TEST_TOKENS(".,;", ".,;");
  TEST_TOKENS(".,;A", ".,;");
  TEST_TOKENS("A.,;", ".,;");
  TEST_TOKENS(".,A;", ".,;");
  TEST_TOKENS(".A,;", ".,;");
}
END_TEST

START_TEST(s21_strerror_test) {
  const int kMaxErrorNumber = 1000;
  for (int i = 0; i < kMaxErrorNumber; ++i) {
    ck_assert_str_eq(strerror(i), s21_strerror(i));
  }
}
END_TEST

START_TEST(s21_to_upper_test) {
  char str1[] = "QwErTyUi";
  char str2[] = "ASDFGH1234567LKJH";
  char str3[] = "zxcvbnm,";
  char *new_str = s21_to_upper(str1);
  if (new_str) {
    ck_assert_str_eq(new_str, "QWERTYUI");
  }
  free(new_str);
  new_str = s21_to_upper(str2);
  if (new_str) {
    ck_assert_str_eq(new_str, "ASDFGH1234567LKJH");
  }
  free(new_str);
  new_str = s21_to_upper(str3);
  if (new_str) {
    ck_assert_str_eq(new_str, "ZXCVBNM,");
  }
  free(new_str);
}
END_TEST

START_TEST(s21_to_lower_test) {
  char str1[] = "QwErTyUi";
  char str2[] = "ASDFGH1234567LKJH";
  char str3[] = "zxcvbnm,";
  char *new_str = s21_to_lower(str1);
  if (new_str) {
    ck_assert_str_eq(new_str, "qwertyui");
  }
  free(new_str);
  new_str = s21_to_lower(str2);
  if (new_str) {
    ck_assert_str_eq(new_str, "asdfgh1234567lkjh");
  }
  free(new_str);
  new_str = s21_to_lower(str3);
  if (new_str) {
    ck_assert_str_eq(new_str, "zxcvbnm,");
  }
  free(new_str);
}
END_TEST

START_TEST(s21_insert_test) {
  char a[] = "qwerty";
  char b[] = "WASD\n";
  char c[] = "\0\0";
  char d[] = "     ";
  char e[] = "";
  char *new_str = s21_insert(a, b, 2);
  if (new_str) ck_assert_str_eq("qwWASD\nerty", new_str);
  free(new_str);
  new_str = s21_insert(a, c, 5);
  if (new_str) ck_assert_str_eq("qwerty", new_str);
  free(new_str);
  new_str = s21_insert(a, d, 1);
  if (new_str) ck_assert_str_eq("q     werty", new_str);
  free(new_str);
  new_str = s21_insert(a, e, 0);
  if (new_str) ck_assert_str_eq("qwerty", new_str);
  free(new_str);
  if (new_str) ck_assert(s21_insert(b, a, 25) == NULL);
  new_str = s21_insert(b, b, 2);
  if (new_str) ck_assert_str_eq("WAWASD\nSD\n", new_str);
  free(new_str);
  new_str = s21_insert(b, c, 5);
  if (new_str) ck_assert_str_eq("WASD\n", new_str);
  free(new_str);
  new_str = s21_insert(b, d, 5);
  if (new_str) ck_assert_str_eq("WASD\n     ", new_str);
  free(new_str);
  new_str = s21_insert(c, d, 0);
  if (new_str) ck_assert_str_eq("     ", new_str);
  free(new_str);
  ck_assert(s21_insert(c, e, 1) == NULL);
  new_str = s21_insert(e, c, 0);
  if (new_str) ck_assert_str_eq("", new_str);
  free(new_str);
}
END_TEST

START_TEST(s21_trim_test) {
  char *a = " qqwERty ";
  char *b = " \n \nQ Qwe Erty y\n \n";
  char *c = "";
  char sep1[] = " yq";
  char sep2[] = " \nQy";
  char *sep3 = S21_NULL;
  char *new_str = s21_trim(a, sep1);
  if (new_str) ck_assert_str_eq("wERt", new_str);
  free(new_str);
  new_str = s21_trim(a, sep2);
  if (new_str) ck_assert_str_eq("qqwERt", new_str);
  free(new_str);

  new_str = s21_trim(a, sep3);
  if (new_str) ck_assert_str_eq(" qqwERty ", new_str);
  free(new_str);

  new_str = s21_trim(b, sep1);
  if (new_str) ck_assert_str_eq("\n \nQ Qwe Erty y\n \n", new_str);
  free(new_str);
  new_str = s21_trim(b, sep2);
  if (new_str) ck_assert_str_eq("we Ert", new_str);
  free(new_str);
}
END_TEST

Suite *Create_suite_s21_string() {
  Suite *suite = suite_create("test_s21_string");
  TCase *tcase_core = tcase_create("Core");
  tcase_add_test(tcase_core, s21_memchr_test);
  tcase_add_test(tcase_core, s21_memcmp_test);
  tcase_add_test(tcase_core, s21_memcpy_test);
  tcase_add_test(tcase_core, s21_memmove_test);
  tcase_add_test(tcase_core, s21_memset_test);
  tcase_add_test(tcase_core, s21_strcat_test);
  tcase_add_test(tcase_core, s21_strncat_test);
  tcase_add_test(tcase_core, s21_strchr_test);
  tcase_add_test(tcase_core, s21_strcmp_test);
  tcase_add_test(tcase_core, s21_strncmp_test);
  tcase_add_test(tcase_core, s21_strcpy_test);
  tcase_add_test(tcase_core, s21_strncpy_test);
  tcase_add_test(tcase_core, s21_strlen_test);
  tcase_add_test(tcase_core, s21_strrchr_test);
  tcase_add_test(tcase_core, s21_strcspn_test);
  tcase_add_test(tcase_core, s21_strerror_test);
  tcase_add_test(tcase_core, s21_strpbrk_test);
  tcase_add_test(tcase_core, s21_strspn_test);
  tcase_add_test(tcase_core, s21_strstr_test);
  tcase_add_test(tcase_core, s21_strtok_test);
  tcase_add_test(tcase_core, s21_to_upper_test);
  tcase_add_test(tcase_core, s21_to_lower_test);
  tcase_add_test(tcase_core, s21_insert_test);
  tcase_add_test(tcase_core, s21_trim_test);
  suite_add_tcase(suite, tcase_core);
  return suite;
}
