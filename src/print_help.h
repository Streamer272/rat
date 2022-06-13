#ifndef RAT_PRINT_HELP_H
#define RAT_PRINT_HELP_H

#include <stddef.h>

#define BUFFER_BASE_SIZE 1024
#define OFFSET "  "
#define print_empty_line() print_line("")

extern char *buffer;
extern unsigned int buffer_current_size;
extern unsigned int buffer_max_size_coefficient;

void safe_print(const char *line);
void safe_flush();
size_t printable_len(char *line);
void print_line(char *line);
void print_help();

#endif
