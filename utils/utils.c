#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "utils.h"

void get_terminal_size() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        // Terminal size is available globally through the ui module
        printf("Terminal size: %d x %d\n", w.ws_col, w.ws_row);
    } else {
        printf("Could not determine terminal size, using defaults\n");
    }
}