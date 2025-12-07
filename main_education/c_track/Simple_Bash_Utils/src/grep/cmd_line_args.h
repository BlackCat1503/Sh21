#pragma once

#include "string_list.h"

//------------------------------------------
// structure to store command line options
//------------------------------------------
typedef struct CmdArgs {
    bool regexp;  // e
    StrList *patterns;

    bool file_regexp;  // f
    StrList *files;

    bool ignore_case;         // i
    bool invert_match;        // v
    bool count;               // c
    bool files_with_matches;  // l
    bool line_number;         // n
    bool no_filename;         // h
    bool no_messages;         // s
    bool only_matching;       // o
    bool color;               // --color

    int num_files_to_process;  // not an option, just a list in cmd line
    bool read_stdin;           // not an option, just follows from args
} CmdArgs;

void Error();
void PrintUsage();
void FreeArgs(CmdArgs *cmd_args);
void PrintArgs(const CmdArgs *cmd_args);
CmdArgs ProcessArgs(int argc, char *argv[]);
int ParseArgs(int argc, char *argv[], CmdArgs *cmd_args);
void LoadPatternsFromFiles(CmdArgs *cmd_args);
