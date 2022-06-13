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
#include "alloc.h"

int main(int argc, char *argv[]) {
    int file_count = 0;
    char **files = alloc(sizeof(char *) * argc);

    init_terminal();

    if (argc == 1) {
        print_file(stdin, FILE_OPTIONS_DEFAULT);
        return EXIT_SUCCESS;
    }

    FILE_OPTIONS file_options;
    DIR_OPTIONS dir_options;

    // command and arg loop
    char *value = NULL;
    for (int i = 1; i < argc; i++) {
        if (value != NULL) {
            strcpy(value, argv[i]);
            value = NULL;
            continue;
        }

        // file args
        if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--number") == 0) {
            file_options.show_line_number = true;
            continue;
        }
        else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--show-nonprintable") == 0) {
            file_options.show_non_printable_chars = true;
            continue;
        }
        else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--clipboard") == 0) {
            file_options.copy_to_clipboard = true;
            continue;
        }
        else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--start") == 0) {
            file_options.start = alloc(sizeof(char) * strlen(argv[i + 1]));
            value = file_options.start;
            continue;
        }
        else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--end") == 0) {
            file_options.end = alloc(sizeof(char) * strlen(argv[i + 1]));
            value = file_options.end;
            continue;
        }
        else if (strcmp(argv[i], "-H") == 0 || strcmp(argv[i], "--highlight") == 0) {
            file_options.highlight_line = alloc(sizeof(char) * strlen(argv[i + 1]));
            value = file_options.highlight_line;
            continue;
        }
        else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--filter") == 0) {
            file_options.filter = alloc(sizeof(char) * strlen(argv[i + 1]));
            value = file_options.filter;
            continue;
        }

        // dir args

        // commands
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printf("%s %s\n", NAME, VERSION);
            continue;
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help();
            continue;
        }

        // incorrect command
        if (argv[i][0] == '-') {
            fprintf(stderr, "invalid option %s (see `rat --help`)\n", argv[i]);
            continue;
        }

        // file
        files[file_count] = alloc(sizeof(char) * strlen(argv[i]));
        strcpy(files[file_count], argv[i]);
        file_count++;
    }

    // file loop
    for (int i = 0; i < file_count; i++) {
        if (files[i][0] == '\\') {
            files[i]++;
        }

        FILE *file = NULL;
        struct stat stats;

        if (strcmp(files[i], "-") == 0) {
            file = stdin;
        }

        if (file == NULL && stat(files[i], &stats) == -1) {
            fprintf(stderr, RED "Couldn't open %s\n" RESET, files[i]);
            continue;
        }

        if (file_count > 2) {
            // 6 is strlen(--  --)
            for (int j = 0; j < ((term_size.ws_col - strlen(files[i]) - 6) / 2 >> 1 << 1); j++) {
                printf(" ");
            }
            printf(GREY "-- %s --\n" RESET, files[i]);
        }

        // check if it is a file
        if ((stats.st_mode & S_IFDIR) == 0) {
            // TODO: add pattern matching (e.g. *.txt -> test.txt, test2.txt)

            if (file == NULL && (file = fopen(files[i], "r")) == NULL) {
                fprintf(stderr, RED "Couldn't open file %s\n" RESET, files[i]);
                continue;
            }

            print_file(file, FILE_OPTIONS_DEFAULT);

            if (file != stdin) {
                fclose(file);
            }
        } else {
            print_dir(files[i], DIR_OPTIONS_DEFAULT);
        }

        if (file_count > 2) {
            printf("\n");
        }
    }

    return EXIT_SUCCESS;
}
