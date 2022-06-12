#include "print_help.h"
#include <stdio.h>
#include "def/style.h"

void print_line() {

}

void print_help() {
    printf("Usage: rat [OPTIONS] [FILES]\n");
    printf("\n");

    printf(GREY "┌ Options ─");
    // 12 is strlen(┌ options ─) + strlen(┐)
    for (int i = 0; i < term_size.ws_col - 12; i++) {
        printf("─");
    }
    printf("┐\n" RESET);

    printf("│  " GREEN "-v" RESET " " CYAN "--version" RESET "  Print version and exit\n");
}
