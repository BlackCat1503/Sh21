#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_line_args.h"
#include "grep.h"

void PrintArgs(const CmdArgs *cmd_args) {
#ifndef DEBUG
    UNUSED(cmd_args)
#endif
    DEBUG_PRINT("regexp: %d\n", cmd_args->regexp);
    DEBUG_PRINT("patterns: ");
    debug_print(cmd_args->patterns);
    DEBUG_PRINT("file_regexp: %d\n", cmd_args->file_regexp);
    DEBUG_PRINT("filename: ");
    debug_print(cmd_args->files);
    DEBUG_PRINT("ignore_case: %d\n", cmd_args->ignore_case);
    DEBUG_PRINT("invert_match: %d\n", cmd_args->invert_match);
    DEBUG_PRINT("count: %d\n", cmd_args->count);
    DEBUG_PRINT("files_with_matches: %d\n", cmd_args->files_with_matches);
    DEBUG_PRINT("line_number: %d\n", cmd_args->line_number);
    DEBUG_PRINT("no_filename: %d\n", cmd_args->no_filename);
    DEBUG_PRINT("no_messages: %d\n", cmd_args->no_messages);
    DEBUG_PRINT("only_matching: %d\n", cmd_args->only_matching);
    DEBUG_PRINT("color: %d\n", cmd_args->color);
    DEBUG_PRINT("\n");
}

void Error() {
    puts("Usage: ./s21_grep [OPTION]... PATTERNS [FILE]...");
    puts("Try './s21_grep --help' for more information.");
    exit(EXIT_ERROR);
}

void FreeArgs(CmdArgs *cmd_args) {
    free_list(cmd_args->patterns);
    free_list(cmd_args->files);
}

void PrintUsage() {
    // clang-format off
    puts("Usage: grep [OPTION]... PATTERNS [FILE]...                                ");
    puts("Search for PATTERNS in each FILE.                                         ");
    puts("Example: grep -i 'hello world' menu.h main.c                              ");
    puts("PATTERNS can contain multiple patterns separated by newlines.             ");
    puts("                                                                          ");
    puts("Pattern selection and interpretation:                                     ");
    puts("  -e, --regexp=PATTERNS     use PATTERNS for matching                     ");
    puts("  -f, --file=FILE           take PATTERNS from FILE                       ");
    puts("  -i, --ignore-case         ignore case distinctions in patterns and data ");
    puts("                                                                          ");
    puts("Miscellaneous:                                                            ");
    puts("  -s, --no-messages         suppress error messages                       ");
    puts("  -v, --invert-match        select non-matching lines                     ");
    puts("      --help                display this help text and exit               ");
    puts("                                                                          ");
    puts("Output control:                                                           ");
    puts("  -n, --line-number         print line number with output lines           ");
    puts("  -h, --no-filename         suppress the file name prefix on output       ");
    puts("  -o, --only-matching       show only nonempty parts of lines that match  ");
    puts("  -l, --files-with-matches  print only names of FILEs with selected lines ");
    puts("  -c, --count               print only a count of selected lines per FILE ");
    puts("      --color               use color to highlight the matching strings   ");
    puts("                                                                          ");
    puts("Exit status is 0 if any line is selected, 1 otherwise;                    ");
    puts("if any error occurs, the exit status is 2.                                ");
    // clang-format on
}

CmdArgs ProcessArgs(int argc, char *argv[]) {
    CmdArgs cmd_args = {0};
    cmd_args.patterns = init_list();
    cmd_args.files = init_list();

    ParseArgs(argc, argv, &cmd_args);
    if (!cmd_args.regexp && !cmd_args.file_regexp) {
        if (optind <= argc - 1) {
            push_back(cmd_args.patterns, argv[optind++]);
        }
    }
    if (optind == argc) {
        cmd_args.read_stdin = true;
    }

    PrintArgs(&cmd_args);
    DEBUG_PRINT("\noptind: %d\n", optind);

    if (!empty_list(cmd_args.files)) {
        LoadPatternsFromFiles(&cmd_args);
    }

    DEBUG_PRINT("All patterns for seacrch:\n");
    debug_print(cmd_args.patterns);

    return cmd_args;
}

int ParseArgs(int argc, char *argv[], CmdArgs *cmd_args) {
    int opt_index = 0;
    while (1) {
        // clang-format off
        static struct option long_options[] = {
            {"regexp",              required_argument, 0, 'e'},
            {"file",                required_argument, 0, 'f'},
            {"ignore-case",         no_argument,       0, 'i'},
            {"no-messages",         no_argument,       0, 's'},
            {"invert-match",        no_argument,       0, 'v'},
            {"line-number",         no_argument,       0, 'n'},
            {"no-filename",         no_argument,       0, 'h'},
            {"only-matching",       no_argument,       0, 'o'},
            {"files-with-matches",  no_argument,       0, 'l'},
            {"count",               no_argument,       0, 'c'},
            {"color",               no_argument,       0, 'C'},
            {"help",                no_argument,       0, 'H'},
            {0,                     0,                 0,  0 }
        };
        // clang-format on

        int opt =
            getopt_long(argc, argv, "e:f:isvnholcCH", long_options, &opt_index);

        if (opt == -1)
            break;

        switch (opt) {
        case 'e':
            cmd_args->regexp = true;
            push_back(cmd_args->patterns, optarg);
            break;
        case 'f':
            cmd_args->file_regexp = true;
            push_back(cmd_args->files, optarg);
            break;
        case 'i':
            cmd_args->ignore_case = true;
            break;
        case 's':
            cmd_args->no_messages = true;
            break;
        case 'v':
            cmd_args->invert_match = true;
            break;
        case 'n':
            cmd_args->line_number = true;
            break;
        case 'h':
            cmd_args->no_filename = true;
            break;
        case 'o':
            cmd_args->only_matching = true;
            break;
        case 'l':
            cmd_args->files_with_matches = true;
            break;
        case 'c':
            cmd_args->count = true;
            break;
        case 'C':
            cmd_args->color = true;
            break;
        case 'H':
            PrintUsage();
            FreeArgs(cmd_args);
            exit(EXIT_SUCCESS);
            break;
        default:
            Error();
            break;
        }
    }
    return opt_index;
}

void LoadPatternsFromFiles(CmdArgs *cmd_args) {
    while (!empty_list(cmd_args->files)) {
        char *filename = cmd_args->files->first->data;
        FILE *file = OpenFile(filename);
        if (!file) {
            fprintf(stderr, "grep: %s: %s\n", filename, strerror(errno));
            FreeArgs(cmd_args);
            exit(EXIT_ERROR);
        }
        // read patterns from file
        DEBUG_PRINT("load patterns from file %s\n", filename);
        char *line = NULL;
        size_t len = 0;
        ssize_t nread;
        while ((nread = getline(&line, &len, file)) != -1) {
            RemoveEOL(line);
            push_back(cmd_args->patterns, line);
            DEBUG_PRINT("\t+ pattern '%s'\n", line);

            free(line);
            line = NULL;
            len = 0;
        }
        if (line) {
            free(line);
        }

        if (file != stdin)
            fclose(file);
        else
            clearerr(stdin);
        pop_front(cmd_args->files);
    }
}
