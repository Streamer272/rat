#include "print_file.h"
#include "alloc.h"

void print_file(FILE *file, OPTIONS options) {
    char *line = alloc(sizeof(char) * 1024);
    size_t line_size = 1024;

    int line_number = 0;
    while (getline(&line, &line_size, file) != -1) {
        line_number++;
        if (options.show_line_number) {
            printf("%d: ", line_number);
        }
        printf("%s", line);
    }
}
