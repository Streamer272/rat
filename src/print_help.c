#include "print_help.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "def/style.h"
#include "term.h"
#include "alloc.h"

char *buffer = NULL;
unsigned int buffer_current_size = 0;
unsigned int buffer_max_size_coefficient = 1;

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
        fprintf(stderr, "Terminal is too small\n");
        exit(EXIT_FAILURE);
    }

    char *temp = alloc(sizeof(char) * 1024);
    sprintf(temp, GREY "│" RESET "%s", line);
    safe_print(temp);
    free(temp);
    // 2 is strlen(│) + strlen(│)
    for (int i = 0; i < ((int) term_size.ws_col) - printable_len(line) - 2; i++) {
        safe_print(" ");
    }
    safe_print(GREY "│\n" RESET);
}

void print_help() {
    safe_print("\n");
    safe_print("Usage: " BOLD "rat [OPTIONS] PATHS...\n" RESET);
    safe_print("\n");

    safe_print(GREY "┌ Options ");
    // 11 is strlen(┌ Options ) + strlen(┐)
    for (int i = 0; i < term_size.ws_col - 11; i++) {
        safe_print("─");
    }
    safe_print("┐\n" RESET);
    print_empty_line();

    print_line(BOLD "File options" RESET);
    print_empty_line();

    print_line(OFFSET GREEN "-n" RESET " " CYAN "--number             " RESET OFFSET "Print line numbers");
    print_line(OFFSET GREEN "-p" RESET " " CYAN "--show-nonprintable  " RESET OFFSET "Show nonprintable characters as ^SOMETHING");
    print_line(OFFSET GREEN "-c" RESET " " CYAN "--clipboard          " RESET OFFSET "Write file content to clipboard instead of STDOUT");
    print_line(OFFSET GREEN "-s" RESET " " CYAN "--start      <NUMBER>" RESET OFFSET "Line to start printing at");
    print_line(OFFSET GREEN "-e" RESET " " CYAN "--end        <NUMBER>" RESET OFFSET "Line to end printing at");
    print_line(OFFSET GREEN "-H" RESET " " CYAN "--highlight  <NUMBER>" RESET OFFSET "Line to highlight");
    print_line(OFFSET GREEN "-f" RESET " " CYAN "--filter     <FILTER>" RESET OFFSET "Only print lines containing <FILTER>");

    print_empty_line();
    print_line(BOLD "Directory options" RESET);
    print_empty_line();

    print_line(GREEN "TODO" RESET);

    print_empty_line();
    print_line(BOLD "Commands" RESET);
    print_empty_line();

    print_line(OFFSET GREEN "-v" RESET " " CYAN "--version            " RESET OFFSET "Print version and exit");
    print_line(OFFSET GREEN "-h" RESET " " CYAN "--help               " RESET OFFSET "Print help message and exit");

    print_empty_line();
    safe_print(GREY "└");
    // 2 is strlen(└) + strlen(┘)
    for (int i = 0; i < term_size.ws_col - 2; i++) {
        safe_print("─");
    }
    safe_print("┘\n" RESET);
    safe_print("\n");

    safe_flush();
}
