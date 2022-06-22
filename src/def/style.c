#include "style.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "alloc.h"

char *RESET;
char *BOLD;
char *ITALIC;
char *UNDERLINE;
char *BLACK;
char *RED;
char *GREEN;
char *YELLOW;
char *CYAN;
char *GREY;
char *GREY_BG;
char *YELLOW_BG;

void enable_colors() {
    RESET = style("0");
    BOLD = style("1");
    ITALIC = style("3");
    UNDERLINE = style("4");
    BLACK = style("30");
    RED = style("31");
    GREEN = style("32");
    YELLOW = style("33");
    CYAN = style("36");
    GREY = style("38;2;153;153;153");
    GREY_BG = style("48;2;69;69;69");
    YELLOW_BG = style("48;2;255;255;0");
}

void disable_colors() {
    RESET = NONE;
    BOLD = NONE;
    ITALIC = NONE;
    UNDERLINE = NONE;
    BLACK = NONE;
    RED = NONE;
    GREEN = NONE;
    YELLOW = NONE;
    CYAN = NONE;
    GREY = NONE;
    GREY_BG = NONE;
    YELLOW_BG = NONE;
}

char *join_strings(int count, ...) {
    char *result = NULL;
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        char *arg = va_arg(args, char *);
        if (result == NULL) {
            result = alloc(strlen(arg) + 1);
            strcpy(result, arg);
        } else {
            result = ralloc(result, strlen(result) + 1, strlen(arg));
            strcat(result, arg);
        }
    }
    va_end(args);

    return result;
}

char *colored(char *text, char *color) {
    return join_strings(3, color, text, RESET);
}
