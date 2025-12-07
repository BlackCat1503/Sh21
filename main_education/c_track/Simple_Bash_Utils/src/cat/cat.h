#pragma once
#include <stdio.h>

//------------------------------------------
// structure to store command line options
//------------------------------------------
typedef struct CmdArgs {
    bool number_nonblank;
    bool show_nonprinting;
    bool show_ends;
    bool show_tabs;
    bool number_strings;
    bool squeeze_blank;
} CmdArgs;

//--------------------------------
// headers
//--------------------------------
int ParseArgs(int argc, char *argv[], CmdArgs *cmd_args);
FILE *OpenFile(const char *filename);
void CloseFile(FILE *file);
void ProcessFile(FILE *file, const CmdArgs *cmd_args, size_t *line_number);
void PrintUsage();

bool IsBlankLine(const char *line);
void PrintNonprinting(int c);

#ifdef DEBUG
void PrintArgs(const CmdArgs *cmd_args);
#endif
