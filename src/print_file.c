#include "print_file.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <limits.h>
#include "alloc.h"
#include "str_contains.h"
#include "def/style.h"

void print_line_number(int line_number) {
    char *line_number_str = alloc(sizeof(char) * LINE_NUMBER_WIDTH);
    sprintf(line_number_str, "%d", line_number);
    size_t line_number_str_len = strlen(line_number_str);

    unsigned int spaces_start = (LINE_NUMBER_WIDTH - line_number_str_len) / 2;
    if ((LINE_NUMBER_WIDTH - line_number_str_len) % 2 == 1) spaces_start++;
    unsigned int spaces_end = LINE_NUMBER_WIDTH - line_number_str_len - spaces_start;
    if (spaces_end == 0) spaces_end = 1;

    for (unsigned int i = 0; i < spaces_start; i++) {
        printf(" ");
    }
    char *message = colored("%s", GREY);
    printf(message, line_number_str);
    free(message);
    for (unsigned int i = 0; i < spaces_end; i++) {
        printf(" ");
    }

    free(line_number_str);
}

void print_file(FILE *file, FILE_OPTIONS options) {
    char *tmp_filename = NULL;
    FILE *tmp_file = NULL;
    if (options.copy_to_clipboard) {
        tmp_filename = tmpnam(NULL);
        tmp_file = freopen(tmp_filename, "w+", stdout);
    }

    int line_number = 1;
    size_t line_size = LINE_SIZE;
    char *line = alloc(line_size + 1);
    long start = 1;
    if (strcmp(options.start, "") != 0) start = strtol(options.start, NULL, 10);
    long end = LONG_MAX;
    if (strcmp(options.end, "") != 0) end = strtol(options.end, NULL, 10);
    long highlight = 0;
    if (strcmp(options.highlight_line, "") != 0) highlight = strtol(options.highlight_line, NULL, 10);

    int i = 0;
    char ch;
    bool ready = false;
    bool jump_back = false;
    while ((ch = (char) getc(file)) != EOF) {
        if (ch == '\n') {
            ready = true;
        } else {
            if (i >= line_size) {
                line = ralloc(line, line_size + 1, LINE_SIZE);
                line_size += LINE_SIZE;
            }

            char *to_add = &ch;
            bool needs_free = false;
            if (options.show_non_printable_chars) {
                switch (ch) {
                    case '\t':
                        to_add = "····";
                        break;
                    case '\r':
                        to_add = "®";
                        break;
                    case '\v':
                        to_add = colored("\\v", UNDERLINE);
                        needs_free = true;
                        break;
                    case ' ':
                        to_add = "·";
                        break;
                    default:
                        break;
                }
            }
            strcat(line, to_add);
            if (needs_free) free(to_add);
        }

        if (ready) {
            print_line:;
            bool is_start = line_number >= start;
            bool is_end = line_number <= end;
            bool is_filtered = strcmp(options.filter, "") != 0 ? str_contains(line, options.filter) : true;

            if (is_filtered) {
                char *highlighted = highlight_needle(line, options.filter);
                free(line);
                line = highlighted;
            }

            if (is_start && is_end && is_filtered) {
                if (options.show_line_number) print_line_number(line_number);
                char *prefix = highlight == line_number ? GREY_BG : NONE;
                char *suffix = highlight == line_number ? RESET : NONE;
                char *line_feed = ready ? (options.show_non_printable_chars ? "⏎\n" : "\n") : "\n";
                char *message = join_strings(3, "%s%s", RESET, "%s%s");
                printf(message, prefix, line, line_feed, suffix);
                free(message);
            }

            if (jump_back) goto back;

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
        jump_back = true;
        goto print_line;
        back:;
    }

    free(line);

    if (options.copy_to_clipboard) {
        fflush(tmp_file);
        fclose(tmp_file);

        char *command = join_strings(3, "xclip ", tmp_filename, " -selection clipboard");
        int exit_code = system(command);
        if (exit_code != 0) {
            char *message = join_strings(7, RED, "Couldn't copy to clipboard (make sure you have ", ITALIC, "xclip", RESET, "installed)\n", RESET);
            fprintf(stderr, "%s", message);
            free(message);
        }

        free(command);
        remove(tmp_filename);
    }
}
