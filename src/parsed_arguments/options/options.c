#include "options.h"

t_options options_get(const int num_of_arguments, const char **arguments)
{
    int options = OPTIONS_NONE;

    for (int i = 0; i < num_of_arguments; i++)
    {
        if (arguments[i][0] != '-')
            continue ;

        if (arguments[i][1] == 'R')
            options |= OPTIONS_RECURSIVE;
    }

    return options;
}
