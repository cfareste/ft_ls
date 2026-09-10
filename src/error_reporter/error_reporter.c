#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "error_reporter.h"
#include "libft.h"

#define APPLICATION_PREFIX "ft_ls"
#define OPENING_DIRECTORY_ERROR "cannot open directory"
#define READING_DIRECTORY_ERROR "reading directory"
#define CLOSING_DIRECTORY_ERROR "closing directory"

static void report_error(const char *error_message, const char *file_path)
{
    ft_fprintf(STDERR_FILENO, "%s: %s", APPLICATION_PREFIX, error_message);

    if (!ft_is_str_empty(file_path))
        ft_fprintf(STDERR_FILENO, " '%s'", file_path);

    ft_fprintf(STDERR_FILENO, ": %s\n", strerror(errno));
}

void report_opening_directory_error(const char *directory_path)
{
    report_error(OPENING_DIRECTORY_ERROR, directory_path);
}

void report_reading_directory_error(const char *directory_path)
{
    report_error(READING_DIRECTORY_ERROR, directory_path);
}

void report_closing_directory_error(const char *directory_path)
{
    report_error(CLOSING_DIRECTORY_ERROR, directory_path);
}
