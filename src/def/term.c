#include "term.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "style.h"

struct winsize term_size;

void init_terminal() {
    if (isatty(STDOUT_FILENO) == 0) {
        term_size.ws_col = 80;
        term_size.ws_row = 80;
        return;
    }

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_size);
    if (term_size.ws_col == 0) {
        char *message = colored("Couldn't get terminal size\n", RED);
        fprintf(stderr, "%s", message);
        free(message);
        exit(EXIT_FAILURE);
    }
}
