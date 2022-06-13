#ifndef RAT_PRINT_HELP_H
#define RAT_PRINT_HELP_H

#include <stddef.h>

#define OFFSET "  "
#define print_empty_line() print_line("")

extern char *buffer;
extern char *buffer_size;

void safe_print(const char *line);
void safe_flush();
size_t printable_len(char *line);
void print_line(char *line);
void print_help();

#endif
