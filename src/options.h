#ifndef RAT_OPTIONS_H
#define RAT_OPTIONS_H

#include "bool.h"

typedef struct {
    bool showEndsAsDollarSign;
    bool showLineNumber;
    bool showBlankCharsAsCarrot;
    bool copyToClipboard;
    char *start;
    char *end;
    char *highlightLine;
    char *filter;
} OPTIONS;

#endif
