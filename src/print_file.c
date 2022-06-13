#include "print_file.h"

void print_file(FILE *file, FILE_OPTIONS options) {
    if (options.show_line_number) printf("1: ");

    char ch;
    int line_number = 1;
    bool print_new_line = false;
    while ((ch = (char) getc(file)) != EOF) {
        if (print_new_line) {
            line_number++;

            printf("\n");
            if (options.show_line_number) printf("%d: ", line_number);

            print_new_line = false;
        }
        if (ch == '\n') {
            print_new_line = true;
            continue;
        }

        printf("%c", ch);
    }

    if (print_new_line) printf("\n");
}
