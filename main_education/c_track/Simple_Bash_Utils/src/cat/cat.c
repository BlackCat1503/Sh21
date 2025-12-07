#define _GNU_SOURCE
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cat.h"

//--------------------------------
// main func
//--------------------------------
int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    CmdArgs cmd_args = {0};
    ParseArgs(argc, argv, &cmd_args);

#ifdef DEBUG
    PrintArgs(&cmd_args);
#endif

    size_t line_number = 1;
    if (optind == argc) {
        ProcessFile(stdin, &cmd_args, &line_number);
    } else {
        while (optind < argc) {
            const char *filename = argv[optind];
            FILE *file = OpenFile(filename);
            if (file) {
                ProcessFile(file, &cmd_args, &line_number);
                CloseFile(file);
            }
            ++optind;
        }
    }
    return EXIT_SUCCESS;
}

FILE *OpenFile(const char *filename) {
    FILE *file = stdin;
    if (strcmp(filename, "-") != 0) {
        file = fopen(filename, "r");
        if (!file) {
            fprintf(stderr, "grep: %s: %s\n", filename, strerror(errno));
        }
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

#ifdef DEBUG
void PrintArgs(const CmdArgs *cmd_args) {
    printf("number_nonblank: %d\n", cmd_args->number_nonblank);
    printf("show_nonprinting: %d\n", cmd_args->show_nonprinting);
    printf("show_ends: %d\n", cmd_args->show_ends);
    printf("show_tabs: %d\n", cmd_args->show_tabs);
    printf("number_strings: %d\n", cmd_args->number_strings);
    printf("squeeze_blank: %d\n", cmd_args->squeeze_blank);
    printf("\n");
}
#endif

void PrintUsage() {
    // clang-format off
    puts("Usage: s21_cat [OPTION]... [FILE]...                                    ");
    puts("Concatenate FILE(s) to standard output.                                 ");
    puts("  -A, --show-all           equivalent to -vET                           ");
    puts("  -b, --number-nonblank    number nonempty output lines, overrides -n   ");
    puts("  -e                       equivalent to -vE                            ");
    puts("  -E, --show-ends          display $ at end of each line                ");
    puts("  -n, --number             number all output lines                      ");
    puts("  -s, --squeeze-blank      suppress repeated empty output lines         ");
    puts("  -t                       equivalent to -vT                            ");
    puts("  -T, --show-tabs          display TAB characters as ^I                 ");
    puts("  -v, --show-nonprinting   use ^ and M- notation, except for LFD and TAB");
    puts("      --help               display this help and exit                   ");
    puts("      --version            output version information and exit          ");
    puts("                                                                        ");
    // clang-format on
}

int ParseArgs(int argc, char *argv[], CmdArgs *cmd_args) {
    int opt_index = 0;
    while (1) {
        // clang-format off
        static struct option long_options[] = {
            {"show-all",         no_argument, 0, 'A'},
            {"number-nonblank",  no_argument, 0, 'b'},
            {"show-ends",        no_argument, 0, 'E'},
            {"number",           no_argument, 0, 'n'},
            {"squeeze-blank",    no_argument, 0, 's'},
            {"show-tabs",        no_argument, 0, 'T'},
            {"show-nonprinting", no_argument, 0, 'v'},
            {"help",             no_argument, 0, 'h'},
            {0,                  0,           0,  0 }
        };
        // clang-format on

        int opt =
            getopt_long(argc, argv, "AbeEnstTuv", long_options, &opt_index);

        if (opt == -1)
            break;

        switch (opt) {
        case 'A':
            cmd_args->show_nonprinting = true;
            cmd_args->show_ends = true;
            cmd_args->show_tabs = true;
            break;
        case 'b':
            cmd_args->number_nonblank = true;
            break;
        case 'E':
            cmd_args->show_ends = true;
            break;
        case 'e':
            cmd_args->show_ends = true;
            cmd_args->show_nonprinting = true;
            break;
        case 'n':
            cmd_args->number_strings = true;
            break;
        case 's':
            cmd_args->squeeze_blank = true;
            break;
        case 'T':
            cmd_args->show_tabs = true;
            break;
        case 't':
            cmd_args->show_tabs = true;
            cmd_args->show_nonprinting = true;
            break;
        case 'u': {
        }  // ignored option according man
        break;
        case 'v':
            cmd_args->show_nonprinting = true;
            break;
        case 'h':
            PrintUsage();
            exit(EXIT_SUCCESS);
            break;
        default:
            fputs("Try './s21_cat --help' for more information.\n", stderr);
            exit(EXIT_FAILURE);
            break;
        }
    }
    return opt_index;
}

bool IsBlankLine(const char *line) { return *line == '\n'; }

void PrintNonprinting(int c) {
    if (c <= 0x1f && c != 0x09 && c != 0x0a) {
        printf("^%c", c + 0x40);
    } else if (c == 0x7f) {
        printf("^%c", c - 0x40);
    } else if (c >= 0x80 && c <= 0x9f) {
        printf("M-^%c", c - 0x40);
    } else {
        putchar(c);
    }
}

void ProcessFile(FILE *file, const CmdArgs *cmd_args, size_t *line_number) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

#if __APPLE__
    *line_number = 1;
#endif

    bool prev_line_was_blank = false;
    while ((nread = getline(&line, &len, file)) != -1) {
        bool skip_line =
            cmd_args->squeeze_blank && prev_line_was_blank && IsBlankLine(line);

        if (!skip_line) {
            bool numbered_line = false;
            if (cmd_args->number_nonblank) {
                if (!IsBlankLine(line)) {
                    printf("%6zu\t", *line_number);
                    numbered_line = true;
                }
            } else if (!numbered_line && cmd_args->number_strings) {
                printf("%6zu\t", *line_number);
                numbered_line = true;
            }

            for (ssize_t i = 0; i < nread; ++i) {
                unsigned char c = line[i];
                bool printed = false;
                if (cmd_args->show_tabs && c == '\t') {
                    printf("^I");
                    printed = true;
                }
                if (cmd_args->show_ends && c == '\n') {
                    printf("$\n");
                    printed = true;
                }
                if (cmd_args->show_nonprinting && c != '\t' && c != '\n') {
                    PrintNonprinting(c);
                    printed = true;
#if __APPLE__
                    if (c == 0x8A) {
                        if (cmd_args->number_nonblank ||
                            cmd_args->number_strings) {
                            if (!IsBlankLine(line)) {
                                ++(*line_number);
                                printf("%6zu\t", *line_number);
                                numbered_line = true;
                            }
                        }
                    }
#endif
                }
                if (!printed) {
                    putchar(c);
                }
            }

            if (numbered_line) {
                ++(*line_number);
            }
            prev_line_was_blank = IsBlankLine(line);
        }

        free(line);
        line = NULL;
        len = 0;
    }
    if (line) {
        free(line);
    }
}
