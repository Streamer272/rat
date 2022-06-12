#ifndef RAT_PRINT_HELP_H
#define RAT_PRINT_HELP_H

#include <stddef.h>

extern char *buffer;

void safe_print(const char *line);
void safe_flush();
size_t printable_len(char *line);
void print_line(char *line);
void print_help();

#endif
