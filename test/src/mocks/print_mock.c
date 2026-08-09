#include <assert.h>
#include <libft.h>
#include <stdio.h>
#include <stdarg.h>

#define PRINT_BUFFER_SIZE 1024

static char printing_buffer[PRINT_BUFFER_SIZE] = { '\0' };

int printf_mock(const char *str, ...)
{
    char temp_buffer[1024];
    va_list args;

    va_start(args, str);
    const int written_length = vsnprintf(temp_buffer, PRINT_BUFFER_SIZE, str, args);
    va_end(args);

    const int printing_buffer_length = (int) ft_strlen(printing_buffer);
    ft_strlcat(printing_buffer, temp_buffer, printing_buffer_length + written_length + 1);

    return printing_buffer_length + written_length;
}

void verify_that_the_str_that_has_been_printed_is(const char *str, ...)
{
    char expected[PRINT_BUFFER_SIZE];
    va_list args;

    va_start(args, str);
    vsnprintf(expected, PRINT_BUFFER_SIZE, str, args);
    va_end(args);

    assert(ft_are_string_equals(printing_buffer, expected));
}

void reset_printing_buffer()
{
    printing_buffer[0] = '\0';
}
