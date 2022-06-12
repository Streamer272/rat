#ifndef RAT_OPTIONS_H
#define RAT_OPTIONS_H

#include <stdbool.h>

typedef struct {
    bool showLineNumber;
    bool showNonPrintableChars;
    bool copyToClipboard;
    char *start;
    char *end;
    char *highlightLine;
    char *filter;
} OPTIONS;

#define OPTIONS_DEFAULT (OPTIONS) { \
    .showLineNumber = false, \
    .showNonPrintableChars = false, \
    .copyToClipboard = false,       \
    .start = "1",                   \
    .end = "",                      \
    .highlightLine = "",            \
    .filter = ""                    \
}

#endif
