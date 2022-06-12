#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include "term.h"
#include "print_help.h"
#include "print_file.h"
#include "print_dir.h"
#include "def/program.h"
#include "def/style.h"

int main(int argc, char *argv[]) {
    init_terminal();

    if (argc == 1) {
        print_file(stdin, FILE_OPTIONS_DEFAULT);
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
        FILE *file = NULL;
        struct stat stats;

        if (strcmp(argv[i], "-") == 0) {
            file = stdin;
        }

        if (file == NULL && stat(argv[i], &stats) == -1) {
            fprintf(stderr, RED "Couldn't open %s\n" RESET, argv[i]);
            continue;
        }

        if (argc > 2) {
            // 6 is strlen(--  --)
            for (int j = 0; j < (((term_size.ws_col - strlen(argv[i]) - 6) / 2) ^ 1); j++) {
                printf(" ");
            }
            printf(GREY "-- %s --\n" RESET, argv[i]);
        }

        // check if it is a file
        if ((stats.st_mode & S_IFDIR) == 0) {
            // TODO: add pattern matching (e.g. *.txt -> test.txt, test2.txt)

            if (file == NULL && (file = fopen(argv[i], "r")) == NULL) {
                fprintf(stderr, RED "Couldn't open file %s\n" RESET, argv[i]);
                continue;
            }

            print_file(file, FILE_OPTIONS_DEFAULT);

            if (file != stdin) {
                fclose(file);
            }
        } else {
            print_dir(argv[i], DIR_OPTIONS_DEFAULT);
        }

        if (argc > 2) {
            printf("\n");
        }
    }

    return EXIT_SUCCESS;
}
