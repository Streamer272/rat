#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "print_file.h"
#include "term.h"
#include "print_help.h"
#include "def/program.h"
#include "def/style.h"

int main(int argc, char *argv[]) {
    init_terminal();

    if (argc == 1) {
        print_file(stdin, OPTIONS_DEFAULT);
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        printf("%s %s\n", NAME, VERSION);
        return EXIT_SUCCESS;
    }
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_help();
        return EXIT_SUCCESS;
    }

    for (int i = 1; i < argc; i++) {
        FILE *file;
        if (strcmp(argv[i], "-") == 0) {
            file = stdin;
        } else {
            file = fopen(argv[i], "r");
            if (file == NULL) {
                fprintf(stderr, RED "Couldn't open file %s\n" RESET, argv[i]);
                continue;
            }
        }

        print_file(file, OPTIONS_DEFAULT);
    }

    return EXIT_SUCCESS;
}
