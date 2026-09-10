#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "error_reporter.h"
#include "libft.h"

void report_opening_directory_error(const char *directory_path)
{
    ft_fprintf(STDERR_FILENO, "ft_ls: cannot open directory");

    if (!ft_is_str_empty(directory_path))
        ft_fprintf(STDERR_FILENO, " '%s'", directory_path);

    ft_fprintf(STDERR_FILENO, ": %s\n", strerror(errno));
}

void report_reading_directory_error(const char *directory_path)
{
    ft_fprintf(STDERR_FILENO, "ft_ls: reading directory");

    if (!ft_is_str_empty(directory_path))
        ft_fprintf(STDERR_FILENO, " '%s'", directory_path);

    ft_fprintf(STDERR_FILENO, ": %s\n", strerror(errno));
}
