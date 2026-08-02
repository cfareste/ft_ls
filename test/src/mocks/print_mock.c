#include <assert.h>
#include <libft.h>
#include <stdio.h>
#include <stdarg.h>

static char printing_buffer[1024];

int printf_mock(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    const int res = vsnprintf(printing_buffer, 1024, format, args);
    va_end(args);

    return res;
}

void verify_that_the_str_that_has_been_printed_is(const char *format, ...)
{
    char temp_buffer[1024];
    va_list args;

    va_start(args, format);
    vsnprintf(temp_buffer, 1024, format, args);
    va_end(args);

    assert(ft_are_string_equals(printing_buffer, temp_buffer));
}
