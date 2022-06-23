#include "print_dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "print_file.h"
#include "def/alloc.h"
#include "def/style.h"

void print_path(char *path, FILE_OPTIONS file_options, DIR_OPTIONS dir_options, bool content) {
    if (path[0] == '\\')
        path++;

    FILE *file = NULL;
    struct stat stats;

    if (strcmp(path, "-") == 0)
        file = stdin;

    if (file == NULL && stat(path, &stats) == -1) {
        char *message = colored("Couldn't open %s\n", RED);
        fprintf(stderr, message, path);
        free(message);
        return;
    }

    print_file_name(path, stats, "", 0, file_options);

    if (!content)
        return;

    // check if it is a file
    if ((stats.st_mode & S_IFDIR) == 0) {
        if (file == NULL && (file = fopen(path, "r")) == NULL) {
            char *message = colored("Couldn't open %s\n", RED);
            fprintf(stderr, message, path);
            free(message);
            return;
        }

        print_file(file, file_options);

        if (file != stdin)
            fclose(file);
    } else {
        print_dir(path, file_options, dir_options);
    }
}

void print_dir(char *path, FILE_OPTIONS file_options, DIR_OPTIONS dir_options) {
    DIR *dir;
    struct dirent *entry;

    if (path[0] == '\\')
        path++;

    if ((dir = opendir(path)) == NULL) {
        char *message = colored("Couldn't open %s\n", RED);
        fprintf(stderr, message, path);
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char *file_name = alloc(strlen(entry->d_name) + 1);
        strcpy(file_name, entry->d_name);

        if (strcmp(file_name, ".") == 0 || strcmp(file_name, "..") == 0) {
            free(file_name);
            continue;
        }
        if (file_name[0] == '.' && !dir_options.show_hidden) {
            free(file_name);
            continue;
        }

        struct stat stats;

        if (stat(file_name, &stats) == -1) {
            char *message = colored("Couldn't open %s\n", RED);
            fprintf(stderr, message, file_name);
            free(message);
            free(file_name);
            continue;
        }

        print_file_name(file_name, stats, "    ", 0, file_options);

        free(file_name);
    }

    closedir(dir);
}
