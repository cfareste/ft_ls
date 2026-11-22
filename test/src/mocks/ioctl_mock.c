#include <errno.h>
#include <stdarg.h>
#include "mocks.h"

static unsigned short configured_column_count;

int ioctl_mock(int fd, unsigned long request, ...)
{
    (void)fd;
    if (request != TIOCGWINSZ)
    {
        errno = ENOTTY;
        return -1;
    }

    va_list args;
    va_start(args, request);
    struct winsize *window_size = va_arg(args, struct winsize *);
    va_end(args);

    if (window_size == NULL)
    {
        errno = EFAULT;
        return -1;
    }

    window_size->ws_col = configured_column_count;
    return 0;
}

void ioctl_mock_setup_columns(unsigned short columns)
{
    configured_column_count = columns;
}

void ioctl_mock_reset(void)
{
    configured_column_count = 0;
}
