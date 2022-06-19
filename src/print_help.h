#ifndef RAT_PRINT_HELP_H
#define RAT_PRINT_HELP_H

#include <stddef.h>

#define OFFSET "  "
#define SPACE " "
#define MINIMUM_LINE_SIZE 40
#define print_empty_line() print_line("")
#define ESCAPE_CHAR 27

size_t printable_len(char *line);
void print_line(char *line);
void print_arg(char *short_name, char *long_name, char *description);
void print_help();

#endif
