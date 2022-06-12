#include "term.h"
#include <unistd.h>

struct winsize term_size;

void init_terminal() {
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_size);
}
