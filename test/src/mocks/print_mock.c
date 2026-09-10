#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include "mocks.h"
#include "libft.h"

#define BUFFER_COUNT 256
#define PRINT_BUFFER_SIZE 2048

static char printing_buffer[BUFFER_COUNT][PRINT_BUFFER_SIZE];

static void print_error(const char *expected, const char *actual, const int fd)
{
    const char *fd_channel = fd == STDOUT_FILENO ? "STDOUT" : "STDERR";
    write(STDERR_FILENO, "\n|-* ", 5);
    write(STDERR_FILENO, fd_channel, ft_strlen(fd_channel));
    write(STDERR_FILENO, " Failed *-|\n\nExpected:\n-----\n", 29);
    write(STDERR_FILENO, expected, ft_strlen(expected));
    write(STDERR_FILENO, "-----\n\nActual:\n-----\n", 21);
    write(STDERR_FILENO, actual, ft_strlen(actual));
    write(STDERR_FILENO, "-----\n\n|-------------------|\n\n", 30);
}

int printf_mock(const char *str, ...)
{
    char temp_buffer[PRINT_BUFFER_SIZE];
    va_list args;

    va_start(args, str);
    const int written_length = vsnprintf(temp_buffer, PRINT_BUFFER_SIZE, str, args);
    va_end(args);

    const int printing_buffer_length = (int) ft_strlen(printing_buffer[STDOUT_FILENO]);
    ft_strlcat(printing_buffer[STDOUT_FILENO], temp_buffer, printing_buffer_length + written_length + 1);

    return printing_buffer_length + written_length;
}

int fprintf_mock(const int fd, const char *str, ...)
{
    char temp_buffer[PRINT_BUFFER_SIZE];
    va_list args;

    va_start(args, str);
    const int written_length = vsnprintf(temp_buffer, PRINT_BUFFER_SIZE, str, args);
    va_end(args);

    const int printing_buffer_length = (int) ft_strlen(printing_buffer[fd]);
    ft_strlcat(printing_buffer[fd], temp_buffer, printing_buffer_length + written_length + 1);

    return printing_buffer_length + written_length;
}

int verify_that_the_str_that_has_been_printed_is(const char *str, ...)
{
    char expected[PRINT_BUFFER_SIZE];
    va_list args;

    va_start(args, str);
    vsnprintf(expected, PRINT_BUFFER_SIZE, str, args);
    va_end(args);

    const int strings_are_equal = ft_are_string_equals(printing_buffer[STDOUT_FILENO], expected);

    if (!strings_are_equal)
        print_error(expected, printing_buffer[STDOUT_FILENO], STDOUT_FILENO);

    return strings_are_equal;
}

int verify_that_the_error_that_has_been_printed_is(const char *str, ...)
{
    char expected[PRINT_BUFFER_SIZE];
    va_list args;

    va_start(args, str);
    vsnprintf(expected, PRINT_BUFFER_SIZE, str, args);
    va_end(args);

    const int strings_are_equal = ft_are_string_equals(printing_buffer[STDERR_FILENO], expected);

    if (!strings_are_equal)
        print_error(expected, printing_buffer[STDERR_FILENO], STDERR_FILENO);

    return strings_are_equal;
}

int verify_that_no_error_was_printed(void)
{
    return verify_that_the_error_that_has_been_printed_is("");
}

void reset_printing_buffer(void)
{
    for (unsigned int i = 0; i < BUFFER_COUNT; i++)
    {
        for (unsigned int j = 0; j < PRINT_BUFFER_SIZE; j++)
        {
            printing_buffer[i][j] = '\0';
        }
    }
}
