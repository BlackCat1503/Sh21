#pragma once

#include "string_list.h"
#include <regex.h>
#include <stdio.h>

#include "cmd_line_args.h"

#define STDIN_NAME "(standard input)"
#define EXIT_ERROR 2

#define UNUSED(var) ((void)var);

#ifdef DEBUG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT(...)
#endif

//--------------------------------
// structure to store regex strings
// along with regex_t
//--------------------------------
typedef struct Regex {
    char *string;
    regex_t regex;
} Regex;

//--------------------------------
// headers
//--------------------------------

FILE *OpenFile(const char *filename);
void CloseFile(FILE *file);
void RemoveEOL(char *line);
bool EmptyString(const char *str);

bool PrintFileNameIfNeed(const CmdArgs *cmd_args, const char *filename);
void PrintLineNumberIfNeed(const CmdArgs *cmd_args, size_t line_number);
void PrintRegexError(int code, const regex_t *regex);
void CompileRegexes(const CmdArgs *cmd_args, Regex *regex_array);
bool ProcessFile(FILE *file, const char *filename, const CmdArgs *cmd_args);

void PrintMatches(const CmdArgs *cmd_args, const char *filename,
                  size_t line_number, const char *line,
                  const char *matching_array, size_t len);

void PrintMatchesWithColor(const CmdArgs *cmd_args, const char *filename,
                           size_t line_number, const char *line,
                           const char *matching_array, size_t len);

void PrintMatchesOnly(const CmdArgs *cmd_args, const char *filename,
                      size_t line_number, const char *line,
                      const char *matching_array, size_t len);
