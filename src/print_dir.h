#ifndef RAT_PRINT_DIR_H
#define RAT_PRINT_DIR_H

#include <stdbool.h>
#include "def/options.h"

#define ENTRIES_LENGTH 16

void print_path(char *path, FILE_OPTIONS file_options, DIR_OPTIONS dir_options, bool content);
void print_dir(char *start, char *path, int nested_depth, FILE_OPTIONS file_options, DIR_OPTIONS dir_options);

#endif
