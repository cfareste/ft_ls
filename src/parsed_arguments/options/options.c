#include <stddef.h>
#include "options.h"

#define RECURSIVE_OPTION_CHAR 'R'
#define LONG_FORMAT_OPTION_CHAR 'l'

static int argument_is_an_option(const char *argument)
{
    return argument[0] == '-';
}

t_options options_get(const char **arguments)
{
    int options = OPTIONS_NONE;

    for (int i = 0; arguments[i] != NULL; i++)
    {
        if (!argument_is_an_option(arguments[i]))
            continue ;

        if (arguments[i][1] == RECURSIVE_OPTION_CHAR)
            options |= OPTIONS_RECURSIVE;
        if (arguments[i][1] == LONG_FORMAT_OPTION_CHAR)
            options |= OPTIONS_LONG_FORMAT;
    }

    return options;
}

int options_get_amount(const char **arguments)
{
    int num_of_options = 0;

    for (int i = 0; arguments[i] != 0; i++)
    {
        if (argument_is_an_option(arguments[i]))
            num_of_options++;
    }

    return num_of_options;
}
