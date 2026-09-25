#include "options.h"

#define RECURSIVE_OPTION_CHAR 'R'

static int argument_is_an_option(const char *argument)
{
    return argument[0] == '-';
}

t_options options_get(const int num_of_arguments, const char **arguments)
{
    int options = OPTIONS_NONE;

    for (int i = 0; i < num_of_arguments; i++)
    {
        if (!argument_is_an_option(arguments[i]))
            continue ;

        if (arguments[i][1] == RECURSIVE_OPTION_CHAR)
            options |= OPTIONS_RECURSIVE;
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
