#include "print_help.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "def/style.h"
#include "def/term.h"
#include "def/alloc.h"

char *buffer = NULL;
unsigned int buffer_current_size = 0;
unsigned int buffer_max_size_coefficient = 1;

bool is_terminal_small = false;
size_t required_terminal_size = 2;  // 2 is strlen(││)

void safe_print(const char *line) {
    if (line == NULL) return;

    if (buffer == NULL) buffer = alloc(BUFFER_BASE_SIZE * buffer_max_size_coefficient);
    while (buffer_current_size + strlen(line) >= buffer_max_size_coefficient * BUFFER_BASE_SIZE) {
        buffer = ralloc(buffer, BUFFER_BASE_SIZE * buffer_max_size_coefficient, BUFFER_BASE_SIZE);
        buffer_max_size_coefficient++;
    }

    sprintf(buffer, "%s%s", buffer, line);
    buffer_current_size += strlen(line);
}

void safe_flush() {
    if (buffer == NULL) return;
    if (is_terminal_small) {
        char *message = colored("Terminal is too small (required %zu, got %hu)\n", RED);
        fprintf(stderr, message, required_terminal_size, term_size.ws_col);
        exit(EXIT_FAILURE);
    }

    printf("%s", buffer);
    free(buffer);
    buffer_current_size = 0;
    buffer_max_size_coefficient = 1;
    buffer = NULL;
}

size_t printable_len(char *line) {
    int count = 0;
    bool is_color = false;
    for (int i = 0; i < strlen(line); i++) {
        if (is_color) {
            if (line[i] == 'm') {
                is_color = false;
            }
            continue;
        } else if (line[i] == '\e') {
            is_color = true;
            continue;
        } else if (isprint(line[i])) {
            count++;
        }
    }

    return count;
}

void print_line(char *line) {
    // TODO: add printing output on next line if terminal is too small

    if (term_size.ws_col < printable_len(line) + 2) {
        is_terminal_small = true;
        if (required_terminal_size < printable_len(line) + 2) required_terminal_size = printable_len(line) + 2;
        return;
    }

    char *temp = join_strings(4, GREY, "│", RESET, line);
    safe_print(temp);
    free(temp);
    // 2 is strlen(│) + strlen(│)
    for (int i = 0; i < (int) ((int) term_size.ws_col) - printable_len(line) - 2; i++) {
        safe_print(" ");
    }
    temp = colored("│\n", GREY);
    safe_print(temp);
    free(temp);
}

void print_arg(char *short_name, char *long_name, char *description) {
    char *temp = join_strings(10, OFFSET, GREEN, short_name, RESET, " ", CYAN, long_name, RESET, OFFSET, description);
    print_line(temp);
    free(temp);
}

void print_help() {
    safe_print("\n");
    char *temp = join_strings(4, "Usage: ", BOLD, "rat [OPTIONS] PATHS...\n", RESET);
    safe_print(temp);
    free(temp);
    safe_print("\n");

    temp = join_strings(2, GREY, "┌ Options ");
    safe_print(temp);
    free(temp);
    // 11 is strlen(┌ Options ) + strlen(┐)
    for (int i = 0; i < term_size.ws_col - 11; i++) {
        safe_print("─");
    }
    temp = join_strings(2, "┐\n", RESET);
    safe_print(temp);
    free(temp);
    print_empty_line();

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
    temp = colored("Directory options", BOLD);
    print_line(temp);
    free(temp);
    print_empty_line();

    temp = colored("TODO", BOLD);
    print_line(temp);
    free(temp);

    print_empty_line();
    temp = colored("Commands", BOLD);
    print_line(temp);
    free(temp);
    print_empty_line();

    print_arg("-v", "--version            ", "Print version and exit");
    print_arg("-h", "--help               ", "Print help message and exit");

    print_empty_line();
    temp = join_strings(2, GREY, "└");
    safe_print(temp);
    free(temp);
    // 2 is strlen(└) + strlen(┘)
    for (int i = 0; i < term_size.ws_col - 2; i++) {
        safe_print("─");
    }
    temp = join_strings(2, "┘\n", RESET);
    safe_print(temp);
    free(temp);
    safe_print("\n");

    safe_flush();
}
