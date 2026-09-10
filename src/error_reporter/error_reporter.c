#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "error_reporter.h"
#include "libft.h"

static void report_error(const char *error_message, const char *file_path)
{
    ft_fprintf(STDERR_FILENO, "%s", error_message);

    if (!ft_is_str_empty(file_path))
        ft_fprintf(STDERR_FILENO, " '%s'", file_path);

    ft_fprintf(STDERR_FILENO, ": %s\n", strerror(errno));
}

void report_opening_directory_error(const char *directory_path)
{
    report_error("ft_ls: cannot open directory", directory_path);
}

void report_reading_directory_error(const char *directory_path)
{
    report_error("ft_ls: reading directory", directory_path);
}

void report_closing_directory_error(const char *directory_path)
{
    report_error("ft_ls: closing directory", directory_path);
}
