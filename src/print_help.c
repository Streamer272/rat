#include "print_help.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "def/style.h"
#include "def/term.h"
#include "def/alloc.h"

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
    // TODO: add printing output on next line if terminal is too small

    size_t len = printable_len(line);

    if (len + 2 < term_size.ws_col) {
        char *temp = colored("│", GREY);
        printf("%s%s", temp, line);

        for (int i = 0; i < term_size.ws_col - len - 2; i++) {
            printf(" ");
        }

        printf("%s\n", temp);
    }
    else {
//    char *temp = colored("│", GREY);
//    printf("%s", temp);
//    printf("line: %s", line);
//    for (int j = 0; j < fill_len; j++) {
//        printf(" ");
//    }
//    printf("%s\n", temp);
//    free(temp);
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
//        exit(EXIT_FAILURE);
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
    print_arg("-C", "--show-chars         ", "Show nonprintable characters as ^SOMETHING");
    print_arg("-c", "--clipboard          ", "Write file content to clipboard instead of STDOUT");
    print_arg("-s", "--start      <NUMBER>", "Line to start printing at");
    print_arg("-e", "--end        <NUMBER>", "Line to end printing at");
    print_arg("-t", "--take       <NUMBER>", "Number of lines to print");
    print_arg("-H", "--highlight  <NUMBER>", "Line to highlight");
    print_arg("-f", "--filter     <FILTER>", "Only print lines containing <FILTER>");
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

    print_arg("-v", "--version            ", "Print version and exit");
    print_arg("-h", "--help               ", "Print help message and exit");

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
