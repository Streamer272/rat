#ifndef RAT_PRINT_DIR_H
#define RAT_PRINT_DIR_H

#include <stdbool.h>
#include "def/options.h"

void print_dir(char *path, FILE_OPTIONS file_options, DIR_OPTIONS dir_options);
void print_path(char *path, FILE_OPTIONS file_options, DIR_OPTIONS dir_options, bool content);

#endif
