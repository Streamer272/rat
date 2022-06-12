#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "print_file.h"
#include "term.h"
#include "print_help.h"

int main(int argc, char *argv[]) {
    init_terminal();

    if (argc == 1) {
        print_file(stdin, OPTIONS_DEFAULT);
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_help();
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}
