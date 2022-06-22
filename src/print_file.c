#include "print_file.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>
#include "def/alloc.h"
#include "string_manipulation.h"
#include "def/style.h"

long get_line_count(FILE *file) {
    long line_count = 0;
    char ch;

    while (!feof(file)) {
        ch = (char) getc(file);
        if (ch == '\n') line_count++;
    }
    rewind(file);

    return line_count;
}

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
    if (options.copy_to_clipboard) disable_colors(); // you don't want useless ANSI escape sequences in your clipboard now do you?

    char *tmp_filename = NULL;
    FILE *tmp_file = NULL;
    if (options.copy_to_clipboard) {
        tmp_filename = tmpnam(NULL);
        tmp_file = freopen(tmp_filename, "w+", stdout);
    }

    int line_number = 1;
    size_t line_size = LINE_SIZE;
    long line_count = get_line_count(file);
    long took_lines = 0;
    char *line = alloc(line_size + 1);

    long start = 1;
    if (strcmp(options.start, "") != 0) start = strtol(options.start, NULL, 10);
    long end = LONG_MAX;
    if (strcmp(options.end, "") != 0) end = strtol(options.end, NULL, 10);
    long take = 0;
    if (strcmp(options.take, "") != 0) take = strtol(options.take, NULL, 10);
    long highlight = 0;
    if (strcmp(options.highlight_line, "") != 0) highlight = strtol(options.highlight_line, NULL, 10);

    int i = 0;
    char ch;
    bool ready = false;
    bool jump_back = false;
    while (true) {
        ch = (char) getc(file);
        if (feof(file)) break;

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
            bool is_start = false;
            if (start >= 0) is_start = line_number >= start;
            else if (start < 0) is_start = line_count - line_number + 1 <= -start;
            bool is_end = false;
            if (end >= 0) is_end = line_number <= end;
            else if (end < 0) is_end = line_number <= line_count + end;
            bool needs_filter = strcmp(options.filter, "") != 0;
            bool is_filtered = true;
            if (needs_filter) is_filtered = str_contains(line, options.filter);
            bool is_highlighted = line_number == highlight;

            bool needs_break = false;

            if (is_start && is_end && (needs_filter ? is_filtered : true) && take != 0 && !jump_back) {
                took_lines++;
                if (took_lines >= take) needs_break = true;
            }

            if (needs_filter ? is_filtered : false) {
                char *highlighted = highlight_needle(line, options.filter, is_highlighted);
                free(line);
                line = highlighted;
            }

            if (is_start && is_end && (needs_filter ? is_filtered : true)) {
                if (options.show_line_number) print_line_number(line_number);
                char *prefix = is_highlighted ? GREY_BG : NONE;
                char *suffix = is_highlighted ? RESET : NONE;
                char *line_feed = ready ? (options.show_non_printable_chars ? "⏎\n" : "\n") : "\n";
                char *message = join_strings(3, "%s%s", RESET, "%s%s");
                printf(message, prefix, line, line_feed, suffix);
                free(message);
            }

            if (jump_back) goto back;
            if (needs_break) {
                sprintf(line, "%s", "");
                break;
            }

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
            char *message = join_strings(8, RED, "Couldn't copy to clipboard (make sure you have ", ITALIC, "xclip", RESET, RED, "installed)\n", RESET);
            fprintf(stderr, "%s", message);
            free(message);
        }

        free(command);
        remove(tmp_filename);
    }
}

void print_file_name(char *file_name, struct stat stats, FILE_OPTIONS options) {
    if (options.show_line_number) {
        for (int j = 0; j < LINE_NUMBER_WIDTH; j++) {
            printf(" ");
        }
    }
    char *message = join_strings(4, BOLD, "%s%s", RESET, " %s %s, %s");
    char *file_name_suffix = (stats.st_mode & S_IFDIR) ? "/" : "";
    char *perms = format_perms(stats.st_mode);
    char *bytes = format_bytes(stats.st_size);
    printf(message, file_name, file_name_suffix, perms, bytes, ctime(&stats.st_mtime));
    free(message);
    free(perms);
    free(bytes);
}
