#ifndef RAT_PRINT_FILE_H
#define RAT_PRINT_FILE_H

#include <stdio.h>
#include <sys/stat.h>
#include "def/options.h"

#define LINE_NUMBER_WIDTH 7
#define LINE_SIZE 1024

long get_line_count(FILE *file);
void print_line_number(int line_number);

void print_file(FILE *file, FILE_OPTIONS options);
void print_file_name(char *file_name, struct stat stats, FILE_OPTIONS options);

#endif
