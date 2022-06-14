#include "print_file.h"
#include <string.h>
#include <malloc.h>
#include "alloc.h"
#include "def/style.h"

void print_file(FILE *file, FILE_OPTIONS options) {
    int line_number = 1;
    size_t line_size = LINE_SIZE;
    char *line = alloc(line_size);

    int i = 0;
    char ch;
    bool next_line = false;
    while ((ch = (char) getc(file)) != EOF) {
        if (ch == '\n') {
            strcat(line, options.show_non_printable_chars ? "⏎\n" : "\n");
            next_line = true;
        } else {
            if (i >= line_size) {
                line = ralloc(line, line_size, LINE_SIZE);
                line_size += LINE_SIZE;
            }

            char *to_add = &ch;
            if (options.show_non_printable_chars) {
                switch (ch) {
                    case '\t':
                        to_add = "····";
                        break;
                    case '\r':
                        to_add = "®";
                        break;
                    case '\v':
                        to_add = UNDERLINE "\\v" RESET;
                        break;
                    case ' ':
                        to_add = "·";
                        break;
                    default:
                        break;
                }
            }
            strcat(line, to_add);
        }

        if (next_line) {
            if (options.show_line_number) printf("%d: ", line_number);
            printf("%s", line);

            i = 0;
            ch = 0;
            next_line = false;
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
        if (next_line) printf(options.show_non_printable_chars ? "⏎\n" : "\n");
    }

    free(line);
}
