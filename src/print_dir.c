#include "print_dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "print_file.h"
#include "def/style.h"

void print_path(char *path, FILE_OPTIONS file_options, DIR_OPTIONS dir_options) {
    if (path[0] == '\\')
        path++;

    FILE *file = NULL;
    struct stat stats;

    if (strcmp(path, "-") == 0)
        file = stdin;

    if (file == NULL && stat(path, &stats) == -1) {
        char *message = colored("Couldn't open %s\n", RED);
        fprintf(stderr, message, path);
        return;
    }

    print_file_name(path, stats, file_options);

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

    if ((dir = opendir(path)) == NULL) {
        char *message = colored("Couldn't open %s\n", RED);
        fprintf(stderr, message, path);
        exit(EXIT_FAILURE);
    }

    bool first = true;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (!first)
            printf("\n");
        else
            first = false;

        print_path(entry->d_name, file_options, dir_options);
    }

    closedir(dir);
}
