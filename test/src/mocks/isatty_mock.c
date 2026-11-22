#include "mocks.h"

static int configured_fd = -1;
static int configured_result;

int isatty_mock(int fd)
{
    if (fd != configured_fd)
        return 0;
    return configured_result;
}

void isatty_mock_setup(int fd, int result)
{
    configured_fd = fd;
    configured_result = result;
}

void isatty_mock_reset(void)
{
    configured_fd = -1;
    configured_result = 0;
}
