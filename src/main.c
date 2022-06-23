#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "print_help.h"
#include "print_file.h"
#include "print_dir.h"
#include "def/program.h"
#include "def/style.h"
#include "def/term.h"
#include "def/alloc.h"

int main(int argc, char *argv[]) {
    int file_count = 0;
    char **files = alloc(sizeof(char *) * argc);

    init_colors();
    init_terminal();

    if (argc == 1) {
        print_file(stdin, FILE_OPTIONS_DEFAULT);
        return EXIT_SUCCESS;
    }

    FILE_OPTIONS file_options = FILE_OPTIONS_DEFAULT;
    DIR_OPTIONS dir_options = DIR_OPTIONS_DEFAULT;

    // command and arg loop
    char *value = NULL;
    for (int i = 1; i < argc; i++) {
        if (value != NULL) {
            strcpy(value, argv[i]);
            value = NULL;
            continue;
        }

        if (strcmp(argv[i], "") == 0) continue;

        // file args
        if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--number") == 0) {
            file_options.show_line_number = true;
            continue;
        }
        else if (strcmp(argv[i], "-C") == 0 || strcmp(argv[i], "--show-chars") == 0) {
            file_options.show_non_printable_chars = true;
            continue;
        }
        else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--clipboard") == 0) {
            file_options.copy_to_clipboard = true;
            continue;
        }
        else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--start") == 0) {
            file_options.start = alloc(sizeof(char) * strlen(argv[i + 1]) + 1);
            value = file_options.start;
            continue;
        }
        else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--end") == 0) {
            file_options.end = alloc(sizeof(char) * strlen(argv[i + 1]) + 1);
            value = file_options.end;
            continue;
        }
        else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--take") == 0) {
            file_options.take = alloc(sizeof(char) * strlen(argv[i + 1]) + 1);
            value = file_options.take;
            continue;
        }
        else if (strcmp(argv[i], "-H") == 0 || strcmp(argv[i], "--highlight") == 0) {
            file_options.highlight_line = alloc(sizeof(char) * strlen(argv[i + 1]) + 1);
            value = file_options.highlight_line;
            continue;
        }
        else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--filter") == 0) {
            file_options.filter = alloc(sizeof(char) * strlen(argv[i + 1]) + 1);
            value = file_options.filter;
            continue;
        }

        // dir args
        if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--hidden") == 0) {
            dir_options.show_hidden = true;
            continue;
        }
        else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--tree") == 0) {
            dir_options.recursive = true;
            continue;
        }
        else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--depth") == 0) {
            dir_options.recursion_depth = alloc(sizeof(char) * strlen(argv[i + 1]) + 1);
            value = dir_options.recursion_depth;
            continue;
        }

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
        if (argv[i][0] == '-' && strcmp(argv[i], "-") != 0) {
            char *message = colored("invalid option %s (see `rat --help`)\n", RED);
            fprintf(stderr, message, argv[i]);
            free(message);
            continue;
        }

        // file
        files[file_count] = alloc(sizeof(char) * strlen(argv[i]));
        strcpy(files[file_count], argv[i]);
        file_count++;
    }

    // file loop
    for (int i = 0; i < file_count; i++) {
        print_path(files[i], file_options, dir_options, true);

        if (file_count > 1 && i != file_count - 1) {
            printf("\n");
        }
    }

    return EXIT_SUCCESS;
}
