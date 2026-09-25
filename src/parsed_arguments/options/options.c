#include "options.h"

#define RECURSIVE_OPTION_CHAR 'R'

static int is_option(const char *argument)
{
    return argument[0] != '-';
}

t_options options_get(const int num_of_arguments, const char **arguments)
{
    int options = OPTIONS_NONE;

    for (int i = 0; i < num_of_arguments; i++)
    {
        if (is_option(arguments[i]))
            continue ;

        if (arguments[i][1] == RECURSIVE_OPTION_CHAR)
            options |= OPTIONS_RECURSIVE;
    }

    return options;
}
