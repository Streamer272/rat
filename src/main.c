#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "def/style.h"
#include "print_file.h"

struct winsize term_size;

void init_terminal();
void print_help();

int main(int argc, char *argv[]) {
    init_terminal();

    if (argc == 1) {
        print_file(stdin, OPTIONS_DEFAULT);
    }

    if (strcmp(argv[1], "--help") == 0) {
        print_help();
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}

void init_terminal() {
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_size);
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
}
