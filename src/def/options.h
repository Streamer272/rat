#ifndef RAT_OPTIONS_H
#define RAT_OPTIONS_H

#include <stdbool.h>

typedef struct {
    bool show_line_number;
    bool show_non_printable_chars;
    bool copy_to_clipboard;
    char *start;
    char *end;
    char *take; // how many lines to print
    char *highlight_line;
    char *filter;
} FILE_OPTIONS;

typedef struct {
    bool show_hidden;
    bool recursive;
    char *recursion_depth;
    char *filter;
} DIR_OPTIONS;

#define FILE_OPTIONS_DEFAULT (FILE_OPTIONS) { \
    .show_line_number = false, \
    .show_non_printable_chars = false, \
    .copy_to_clipboard = false, \
    .start = "", \
    .end = "", \
    .take = "", \
    .highlight_line = "", \
    .filter = "" \
}

#define DIR_OPTIONS_DEFAULT (DIR_OPTIONS) { \
    .show_hidden = false, \
    .recursive = true, \
    .recursion_depth = "3", \
    .filter = "" \
}

#endif
