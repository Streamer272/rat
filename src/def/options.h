#ifndef RAT_OPTIONS_H
#define RAT_OPTIONS_H

#include <stdbool.h>

typedef struct {
    bool show_line_number;
    bool show_non_printable_chars;
    bool copy_to_clipboard;
    char *start;
    char *end;
    char *highlight_line;
    char *filter;
} OPTIONS;

#define OPTIONS_DEFAULT (OPTIONS) { \
    .show_line_number = false, \
    .show_non_printable_chars = false, \
    .copy_to_clipboard = false,       \
    .start = "1",                   \
    .end = "",                      \
    .highlight_line = "",            \
    .filter = ""                    \
}

#endif
