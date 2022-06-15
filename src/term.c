#include "term.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "def/style.h"

struct winsize term_size;

// TODO: fix can't >> to a file
void init_terminal() {
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_size);
    if (term_size.ws_col == 0) {
        fprintf(stderr, RED "Couldn't get terminal size\n" RESET);
//        exit(EXIT_FAILURE);
    }
}
