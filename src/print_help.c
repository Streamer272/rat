#include "print_help.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "def/style.h"
#include "term.h"

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
        perror("Terminal is too small");
        exit(EXIT_FAILURE);
    }

    printf(GREY "│" RESET "%s", line);
    // 2 is strlen(│) + strlen(│)
    for (int i = 0; i < term_size.ws_col - printable_len(line) - 2; i++) {
        printf(" ");
    }
    printf(GREY "│\n" RESET);
}

void print_help() {
    printf("\n");
    printf("Usage: rat [OPTIONS] [FILES]\n");
    printf("\n");

    printf(GREY "┌ Options ");
    // 11 is strlen(┌ Options ) + strlen(┐)
    for (int i = 0; i < term_size.ws_col - 11; i++) {
        printf("─");
    }
    printf("┐\n" RESET);
    print_line("");

    print_line("  " GREEN "-v" RESET " " CYAN "--version" RESET "  Print version and exit");
    print_line("  " GREEN "-h" RESET " " CYAN "--help   " RESET "  Print help message and exit");

    print_line("");
    printf(GREY "└");
    // 2 is strlen(└) + strlen(┘)
    for (int i = 0; i < term_size.ws_col - 2; i++) {
        printf("─");
    }
    printf("┘\n" RESET);
    printf("\n");
}
