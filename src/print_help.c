#include "print_help.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "def/style.h"
#include "def/term.h"

size_t printable_len(char *line) {
    if (line == NULL) return 0;

    int count = 0;
    bool is_color = false;
    for (int i = 0; i < strlen(line); i++) {
        char ch = line[i];

        if (is_color) {
            if (ch == 'm') is_color = false;
            continue;
        } else if (ch == '\e') {
            is_color = true;
            continue;
        } else if (isprint(ch)) {
            count++;
        }
    }

    return count;
}

void print_line(char *line) {
    if (line == NULL) return;

    size_t line_length = printable_len(line);

    // 2 is strlen(││)
    if (line_length + 2 < term_size.ws_col) {
        char *temp = colored("│", GREY);
        printf("%s%s", temp, line);

        for (int i = 0; i < term_size.ws_col - line_length - 2; i++) {
            printf(" ");
        }

        printf("%s\n", temp);
        free(temp);
    } else {
        char *border = colored("│", GREY);

        int can_print = term_size.ws_col - 2;
        int remaining = can_print;
        bool is_color = false;
        bool new_line = false;
        for (int i = 0; i < strlen(line); i++) {
            if (remaining == can_print)
                printf("%s", border);

            char ch = line[i];
            if (isspace(ch)) {
                int next_word_length = 0;
                for (int j = i + 1; j < strlen(line); j++) {
                    char next_ch = line[j];

                    if (next_ch >= 33 && next_ch <= 126)
                        next_word_length++;
                    else
                        break;
                }

                if (next_word_length > remaining - 1)
                    new_line = true;
            }

            printf("%c", ch);

            if (is_color) {
                if (ch == 'm') is_color = false;
                continue;
            } else if (ch == '\e') {
                is_color = true;
                continue;
            }

            if (isprint(ch)) remaining--;

            if (new_line) {
                for (int j = 0; j < remaining; j++) {
                    printf(" ");
                }
                printf("%s\n", border);

                remaining = can_print;
                new_line = false;
            }

            if (remaining == 0) {
                printf("%s\n", border);
                remaining = can_print;
            }
        }

        if (remaining == can_print) {
            printf("%s", border);
        }
        for (int i = remaining; i > 0; i--) {
            printf(" ");
        }

        printf("%s\n", border);
        free(border);
    }
}

void print_arg(char *short_name, char *long_name, char *description) {
    char *temp = join_strings(10, OFFSET, GREEN, short_name, RESET, " ", CYAN, long_name, RESET, OFFSET, description);
    print_line(temp);
    free(temp);
}

void print_help() {
    if (term_size.ws_col < MINIMUM_LINE_SIZE) {
        char *message = colored("Terminal is too small (required %d, got %hu)\n", RED);
        fprintf(stderr, message, MINIMUM_LINE_SIZE, term_size.ws_col);
        exit(EXIT_FAILURE);
    }

    // usage text
    printf("\n");
    char *temp = join_strings(4, "Usage: ", BOLD, "rat [OPTIONS] PATHS...", RESET);
    printf("%s\n", temp);
    printf("\n");
    free(temp);

    // header
    temp = join_strings(2, GREY, "┌ Options ");
    printf("%s", temp);
    free(temp);
    // 11 is strlen(┌ Options ) + strlen(┐)
    for (int i = 0; i < term_size.ws_col - 11; i++) {
        printf("─");
    }
    temp = join_strings(2, "┐", RESET);
    printf("%s\n", temp);
    free(temp);

    // file options
    temp = colored("File options", BOLD);
    print_line(temp);
    free(temp);
    print_empty_line();

    print_arg("-n", "--number             ", "Print line numbers");
    print_arg("-C", "--show-chars         ", "Show nonprintable characters");
    print_arg("-c", "--clipboard          ", "Write file content to clipboard instead of STDOUT");
    print_arg("-s", "--start      <NUMBER>", "Line to start printing at");
    print_arg("-e", "--end        <NUMBER>", "Line to end printing at");
    print_arg("-t", "--take       <NUMBER>", "Number of lines to print");
    print_arg("-H", "--highlight  <NUMBER>", "Line to highlight");
    print_arg("-f", "--filter     <FILTER>", "Only print lines containing FILTER");
    print_empty_line();

    // dir options
    temp = colored("Directory options", BOLD);
    print_line(temp);
    free(temp);
    print_empty_line();

    temp = colored("TODO", GREEN);
    print_line(temp);
    free(temp);
    print_empty_line();

    // commands
    temp = colored("Commands", BOLD);
    print_line(temp);
    free(temp);
    print_empty_line();

    print_arg("-v", "--version            ", "Print version");
    print_arg("-h", "--help               ", "Print help message");

    // footer
    print_empty_line();
    temp = join_strings(2, GREY, "└");
    printf("%s", temp);
    free(temp);
    // 2 is strlen(└) + strlen(┘)
    for (int i = 0; i < term_size.ws_col - 2; i++) {
        printf("─");
    }
    temp = join_strings(2, "┘", RESET);
    printf("%s\n", temp);
    free(temp);
    printf("\n");
}
