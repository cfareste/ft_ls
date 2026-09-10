#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "error_reporter.h"
#include "libft.h"

void report_error(const char *message)
{
    ft_fprintf(STDERR_FILENO, "ft_ls: ");

    if (!ft_is_str_empty(message))
        ft_fprintf(STDERR_FILENO, "%s: ", message);

    ft_fprintf(STDERR_FILENO, "%s\n", strerror(errno));
}

void report_opening_directory_error(const char *directory_path)
{
    (void) directory_path;
    ft_fprintf(STDERR_FILENO, "ft_ls: cannot open directory\n");
}
