#define _GNU_SOURCE
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <locale.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cmd_line_args.h"
#include "grep.h"

#define NO_MATCH 0
#define BEGIN_MATCH 1
#define CONTINUE_MATCH 2

//--------------------------------
// main func
//--------------------------------
int main(int argc, char *argv[]) {
    if (argc == 1)
        Error();
    setlocale(LC_ALL, "");

    CmdArgs cmd_args = ProcessArgs(argc, argv);

    bool errors = false;
    bool found_matching = false;

    if (cmd_args.read_stdin) {
        DEBUG_PRINT("Process stdin\n");
        found_matching |= ProcessFile(stdin, STDIN_NAME, &cmd_args);
    } else {
        cmd_args.num_files_to_process = argc - optind;
        DEBUG_PRINT("There are %d files to process\n",
                    cmd_args.num_files_to_process);
        while (optind < argc) {
            const char *filename = argv[optind];
            DEBUG_PRINT("Process file: %s\n", filename);
            FILE *file = OpenFile(filename);
            if (file) {
                found_matching |= ProcessFile(file, filename, &cmd_args);
                CloseFile(file);
            } else {
                errors = true;
                if (!cmd_args.no_messages) {
                    fprintf(stderr, "grep: %s: %s\n", filename,
                            strerror(errno));
                }
            }
            ++optind;
        }
    }

    FreeArgs(&cmd_args);

    int exit_status = EXIT_SUCCESS;
    if (!found_matching)
        exit_status = EXIT_FAILURE;
    if (errors)
        exit_status = EXIT_ERROR;
    return exit_status;
}

FILE *OpenFile(const char *filename) {
    FILE *file = stdin;
    if (strcmp(filename, "-") != 0) {
        file = fopen(filename, "r");
    }
    return file;
}

void CloseFile(FILE *file) {
    if (file != stdin) {
        fclose(file);
    } else {
        clearerr(stdin);
    }
}

// Remove '\n' from the end of input line
void RemoveEOL(char *line) {
    char *p = strrchr(line, '\n');
    if (p) {
        *p = '\0';
    }
}

bool EmptyString(const char *str) { return *str == '\0'; }

bool FindMatches(Regex *regex_array, size_t num_regex, const char *str,
                 char **match_array) {
    bool found = false;
    size_t len_of_str = strlen(str);
    *match_array = malloc(len_of_str);
    memset(*match_array, NO_MATCH, len_of_str);

    for (size_t i = 0; i < num_regex; i++) {
        if (EmptyString(regex_array[i].string)) {
            found = true;
            memset(*match_array, CONTINUE_MATCH, len_of_str);
            *match_array[0] = BEGIN_MATCH;
            continue;
        }
        const regex_t *regex = &regex_array[i].regex;
        regmatch_t pmatch[1];
        regoff_t off, len;
        const char *s = str;

        int eflags = 0;
        for (int j = 0;; j++) {
            int ret = regexec(regex, s, 1, pmatch, eflags);
            if (ret) {
                break;
            }

            found = true;
            off = pmatch[0].rm_so + (s - str);
            len = pmatch[0].rm_eo - pmatch[0].rm_so;

            if (len > 0)
                (*match_array)[off] = BEGIN_MATCH;  // beginning of matching
            for (regoff_t k = off + 1; k < off + len; ++k) {
                (*match_array)[k] = CONTINUE_MATCH;  // continue matching
            }

            regoff_t shift = pmatch[0].rm_eo;
            if (shift == 0) {
                shift = 1;
            }

            s += shift;                    // offset to find another matches
            if (s > str + len_of_str - 1)  // shifted to the end of string
                break;
#ifdef __unix__
            eflags |= REG_NOTBOL;  // stop matching the beginning of line (BOL)
#endif
        }
    }
    return found;
}

bool PrintFileNameIfNeed(const CmdArgs *cmd_args, const char *filename) {
    bool res = false;
    if ((cmd_args->num_files_to_process > 1 && !cmd_args->no_filename) ||
        cmd_args->files_with_matches) {
        if (strcmp(filename, "-") == 0) {
            printf("%s", STDIN_NAME);
        } else {
            printf("%s", filename);
        }
        res = true;

        if (!cmd_args->files_with_matches && !cmd_args->no_filename) {
            putchar(':');
            res = false;
        }
    }
    return res;
}

void PrintLineNumberIfNeed(const CmdArgs *cmd_args, size_t line_number) {
    if (cmd_args->line_number) {
        printf("%zu:", line_number);
    }
}

void PrintMatchesWithColor(const CmdArgs *cmd_args, const char *filename,
                           size_t line_number, const char *line,
                           const char *matching_array, size_t len) {
    PrintFileNameIfNeed(cmd_args, filename);
    PrintLineNumberIfNeed(cmd_args, line_number);
    for (size_t i = 0; i < len; ++i) {
        if (matching_array[i] == BEGIN_MATCH) {
            printf("\033[91m");
        } else if (matching_array[i] == NO_MATCH && i > 0 &&
                   matching_array[i - 1] != NO_MATCH) {
            printf("\033[0m");
        }
        putchar(line[i]);
    }
    printf("\033[0m");
    printf("\n");
}

void PrintMatchesOnly(const CmdArgs *cmd_args, const char *filename,
                      size_t line_number, const char *line,
                      const char *matching_array, size_t len) {
    bool need_to_print = false;
    bool printed_smth = false;

    PrintFileNameIfNeed(cmd_args, filename);
    PrintLineNumberIfNeed(cmd_args, line_number);

    size_t i = 0;
    while (i < len) {
        if (matching_array[i] == BEGIN_MATCH) {
            need_to_print = true;
            if (printed_smth) {
                putchar('\n');
            }
        } else if (matching_array[i] == NO_MATCH && i > 0 &&
                   matching_array[i - 1] != NO_MATCH) {
            need_to_print = false;
        }
        if (need_to_print && matching_array[i] != NO_MATCH) {
            putchar(line[i++]);
            while (i < len && matching_array[i] == CONTINUE_MATCH) {
                putchar(line[i++]);
            }
            printed_smth = true;
        } else {
            ++i;
        }
    }
#ifdef __unix__
    if (printed_smth)
        printf("\n");
#else
    printf("\n");
#endif
}

void PrintMatches(const CmdArgs *cmd_args, const char *filename,
                  size_t line_number, const char *line,
                  const char *matching_array, size_t len) {
    if (cmd_args->only_matching) {
        if (!cmd_args->invert_match) {
            PrintMatchesOnly(cmd_args, filename, line_number, line,
                             matching_array, len);
        } else {
            PrintFileNameIfNeed(cmd_args, filename);
            PrintLineNumberIfNeed(cmd_args, line_number);
            printf("%s\n", line);
        }
    } else if (cmd_args->color) {
        PrintMatchesWithColor(cmd_args, filename, line_number, line,
                              matching_array, len);
    } else {
        PrintFileNameIfNeed(cmd_args, filename);
        PrintLineNumberIfNeed(cmd_args, line_number);
        printf("%s\n", line);
    }
}

void PrintRegexError(int code, const regex_t *regex) {
    const size_t BUFSIZE = 1024;
    char buf[BUFSIZE];
    size_t errlen = regerror(code, regex, buf, BUFSIZE);
    assert(errlen > 0 && errlen < BUFSIZE);
    fprintf(stderr, "s21_grep: %s\n", buf);
}

void CompileRegexes(const CmdArgs *cmd_args, Regex *regex_array) {
    StrList *patterns = cmd_args->patterns;
    size_t i = 0;
    for (Node *p = patterns->first; p != NULL; p = p->next) {
        regex_array[i].string = p->data;
        int cflags = 0;
        if (cmd_args->ignore_case) {
            cflags |= REG_ICASE;
        }
        if (!EmptyString(regex_array[i].string)) {
            int ret =
                regcomp(&regex_array[i].regex, regex_array[i].string, cflags);
            if (ret != 0) {
                PrintRegexError(ret, &regex_array[i].regex);
                free(regex_array);
                FreeArgs((CmdArgs *)cmd_args);
                exit(EXIT_ERROR);
            }
        }
        ++i;
    }
}

bool ProcessFile(FILE *file, const char *filename, const CmdArgs *cmd_args) {
    size_t num_regex = cmd_args->patterns->size;
    Regex *regex_array = malloc(num_regex * sizeof(Regex));
    CompileRegexes(cmd_args, regex_array);

    size_t line_number = 1;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    size_t matches_counter = 0;

    while ((nread = getline(&line, &len, file)) != -1) {
        RemoveEOL(line);
        len = strlen(line);

        char *matching_array;
        bool found = FindMatches(regex_array, num_regex, line, &matching_array);
        if (found != cmd_args->invert_match) {
            if (!cmd_args->files_with_matches && !cmd_args->count) {
                PrintMatches(cmd_args, filename, line_number, line,
                             matching_array, len);
            }
            matches_counter++;
        }
        ++line_number;

        free(matching_array);
        free(line);
        line = NULL;
        len = 0;
    }
    if (line) {
        free(line);
    }

    // print matches files or counters only
    if (cmd_args->count) {
        bool filename_printed = false;
        if (!cmd_args->no_filename) {
            filename_printed = PrintFileNameIfNeed(cmd_args, filename);
        }
        if (cmd_args->files_with_matches) {
            matches_counter = matches_counter > 0 ? 1 : 0;
        }
        if (filename_printed) {
            printf(":");
        }
        printf("%zu", matches_counter);
        printf("\n");
    }
    if (cmd_args->files_with_matches) {
        if (matches_counter > 0) {
            PrintFileNameIfNeed(cmd_args, filename);
            printf("\n");
        }
    }

    for (size_t i = 0; i < num_regex; ++i) {
        regfree(&regex_array[i].regex);
    }
    free(regex_array);

    return (matches_counter != 0);
}
