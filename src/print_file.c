#include "print_file.h"
#include <string.h>
#include <malloc.h>
#include "alloc.h"

void print_file(FILE *file, FILE_OPTIONS options) {
    int line_number = 1;
    size_t line_size = LINE_SIZE;
    char *line = alloc(line_size);

    int i = 0;
    char ch;
    bool ready = false;
    while ((ch = (char) getc(file)) != EOF) {
        if (ch == '\n') {
            ready = true;
        }
        else {
            if (i >= line_size) {
                line = ralloc(line, line_size, LINE_SIZE);
                line_size += LINE_SIZE;
            }

            strcat(line, &ch);
        }

        if (ready) {
            if (options.show_line_number) printf("%d: ", line_number);
            printf("%s\n", line);

            i = 0;
            ch = 0;
            ready = false;
            line_number++;
            free(line);
            line_size = LINE_SIZE;
            line = alloc(line_size);
        }

        i++;
    }

    if (strcmp(line, "") != 0) {
        if (options.show_line_number) printf("%d: ", line_number);
        printf("%s", line);
        if (ready) printf("\n");
    }

    free(line);
}
