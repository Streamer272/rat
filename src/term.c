#include "term.h"
#include <unistd.h>

void init_terminal() {
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_size);
}
