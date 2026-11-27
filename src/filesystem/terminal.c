#include <sys/ioctl.h>
#include <unistd.h>
#include "terminal.h"

int terminal_stdout_is_terminal(void)
{
    return isatty(STDOUT_FILENO);
}

size_t terminal_get_stdout_width(void)
{
    struct winsize terminal_size = {0};

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_size) != 0)
        return 0;
    return terminal_size.ws_col;
}
