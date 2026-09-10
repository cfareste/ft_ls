#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "error_reporter.h"
#include "libft.h"

void report_error(const char *message)
{
    (void) message;
    ft_fprintf(STDERR_FILENO, "ft_ls: %s\n", strerror(errno));
}
