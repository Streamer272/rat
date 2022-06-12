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

void safe_print(const char *line) {
    if (buffer == NULL) buffer = alloc(sizeof(char) * 4096);
    if (line == NULL) return;

    sprintf(buffer, "%s%s", buffer, line);
}

void safe_flush() {
    if (buffer == NULL) return;

    printf("%s", buffer);
    free(buffer);
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
        }
        else if (line[i] == '\e') {
            is_color = true;
            continue;
        }
        else if (isprint(line[i])) {
            count++;
        }
    }

    return count;
}

void print_line(char *line) {
    if (term_size.ws_col < printable_len(line) + 2) {
        fprintf(stderr, "Terminal is too small\n");
        exit(EXIT_FAILURE);
    }

    char *temp = alloc(sizeof(char) * 1024);
    sprintf(temp, GREY "|" RESET "%s", line);
    safe_print(temp);
    free(temp);
    // 2 is strlen(│) + strlen(│)
    for (int i = 0; i < term_size.ws_col - printable_len(line) - 2; i++) {
        safe_print(" ");
    }
    safe_print(GREY "│\n" RESET);
}

void print_help() {
    safe_print("\n");
    safe_print("Usage: rat [FILE_OPTIONS] [FILES]\n");
    safe_print("\n");

    safe_print(GREY "┌ Options ");
    // 11 is strlen(┌ Options ) + strlen(┐)
    for (int i = 0; i < term_size.ws_col - 11; i++) {
        safe_print("─");
    }
    safe_print("┐\n" RESET);
    print_line("");

    print_line("  " GREEN "-v" RESET " " CYAN "--version" RESET "  Print version and exit");
    print_line("  " GREEN "-h" RESET " " CYAN "--help   " RESET "  Print help message and exit");

    print_line("");
    safe_print(GREY "└");
    // 2 is strlen(└) + strlen(┘)
    for (int i = 0; i < term_size.ws_col - 2; i++) {
        safe_print("─");
    }
    safe_print("┘\n" RESET);
    safe_print("\n");

    safe_flush();
}
