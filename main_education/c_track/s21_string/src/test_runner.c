#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Suite *Create_suite_s21_string();
Suite *Create_suite_s21_sprintf();
Suite *Create_suite_s21_sscanf();

int main(void) {
  printf("\n");
  bool exit_code = EXIT_SUCCESS;

  // test s21_string functions
  Suite *suite1 = Create_suite_s21_string();
  SRunner *suite_runner1 = srunner_create(suite1);
  // srunner_set_fork_status(suite_runner1, CK_NOFORK);
  srunner_run_all(suite_runner1, CK_NORMAL);
  int failed_count1 = srunner_ntests_failed(suite_runner1);
  srunner_free(suite_runner1);
  if (failed_count1 != 0) {
    printf("Failed tests: %d\n", failed_count1);
    exit_code = EXIT_FAILURE;
  } else {
    printf("Success\n");
  }
  printf("\n");

  // test s21_sprintf
  Suite *suite2 = Create_suite_s21_sprintf();
  SRunner *suite_runner2 = srunner_create(suite2);
  // srunner_set_fork_status(suite_runner2, CK_NOFORK);
  srunner_run_all(suite_runner2, CK_NORMAL);
  int failed_count2 = srunner_ntests_failed(suite_runner2);
  srunner_free(suite_runner2);

  if (failed_count2 != 0) {
    printf("Failed tests: %d\n", failed_count2);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  // test s21_sscanf
  Suite *suite3 = Create_suite_s21_sscanf();
  SRunner *suite_runner3 = srunner_create(suite3);
  // srunner_set_fork_status(suite_runner3, CK_NOFORK);
  srunner_run_all(suite_runner3, CK_NORMAL);
  int failed_count3 = srunner_ntests_failed(suite_runner3);
  srunner_free(suite_runner3);

  if (failed_count3 != 0) {
    printf("Failed tests: %d\n", failed_count2);
    return EXIT_FAILURE;
  } else {
    printf("Success\n");
  }

  return exit_code;
}
