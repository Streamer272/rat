#include "print_file.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <limits.h>
#include "alloc.h"
#include "def/style.h"

void print_line_number(int line_number) {
    char *line_number_str = alloc(sizeof(char) * LINE_NUMBER_WIDTH);
    sprintf(line_number_str, "%d", line_number);
    unsigned int line_number_str_len = (unsigned int) strlen(line_number_str);

    unsigned int spaces_start = (LINE_NUMBER_WIDTH - line_number_str_len) / 2;
    if ((LINE_NUMBER_WIDTH - line_number_str_len) % 2 == 1) spaces_start++;
    unsigned int spaces_end = LINE_NUMBER_WIDTH - line_number_str_len - spaces_start;
    if (spaces_end == 0) spaces_end = 1;

    for (unsigned int i = 0; i < spaces_start; i++) {
        printf(" ");
    }
    printf(GREY "%s" RESET, line_number_str);
    for (unsigned int i = 0; i < spaces_end; i++) {
        printf(" ");
    }

    free(line_number_str);
}

void print_file(FILE *file, FILE_OPTIONS options) {
    // TODO: implement copy to clipboard with tmpnam(NULL), freopen(tmp_file, "w+", stdout) remove(tmp_file), popen("xclip -in -selection clipboard", "w") and fprintf(xclip_pipe, tmp_file_content)

    int line_number = 1;
    size_t line_size = LINE_SIZE;
    char *line = alloc(line_size);
    long start = 1;
    if (strcmp(options.start, "") != 0) start = strtol(options.start, NULL, 10);
    long end = LONG_MAX;
    if (strcmp(options.end, "") != 0) end = strtol(options.end, NULL, 10);

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
            if (line_number >= start && line_number <= end) {
                if (options.show_line_number) print_line_number(line_number);
                printf("%s", line);
            }

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
        if (options.show_line_number) print_line_number(line_number);
        printf("%s", line);
        if (next_line) printf(options.show_non_printable_chars ? "⏎\n" : "\n");
    }

    free(line);
}
