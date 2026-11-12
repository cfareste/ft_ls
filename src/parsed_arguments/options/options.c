#include <stddef.h>
#include "options.h"

#define RECURSIVE_OPTION_CHAR 'R'
#define LONG_FORMAT_OPTION_CHAR 'l'
#define READ_ALL_OPTION_CHAR 'a'
#define REVERSE_OPTION_CHAR 'r'
#define TIME_SORTING_OPTION_CHAR 't'

static int argument_is_an_option(const char *argument)
{
    return argument[0] == '-';
}

static t_options get_option_flag(const char option)
{
    if (option == RECURSIVE_OPTION_CHAR)
        return OPTIONS_RECURSIVE;
    if (option == LONG_FORMAT_OPTION_CHAR)
        return OPTIONS_LONG_FORMAT;
    if (option == READ_ALL_OPTION_CHAR)
        return OPTIONS_READ_ALL;
    if (option == REVERSE_OPTION_CHAR)
        return OPTIONS_REVERSE;
    if (option == TIME_SORTING_OPTION_CHAR)
        return OPTIONS_TIME_SORTING;
    return OPTIONS_NONE;
}

char options_get_invalid_option(const char **arguments)
{
    for (int i = 0; arguments[i] != NULL; i++)
    {
        if (!argument_is_an_option(arguments[i]))
            continue ;

        for (int j = 1; arguments[i][j] != '\0'; j++)
        {
            if (get_option_flag(arguments[i][j]) == OPTIONS_NONE)
                return arguments[i][j];
        }
    }

    return '\0';
}

t_options options_get(const char **arguments)
{
    int options = OPTIONS_NONE;

    for (int i = 0; arguments[i] != NULL; i++)
    {
        if (!argument_is_an_option(arguments[i]))
            continue ;

        for (int j = 1; arguments[i][j] != '\0'; j++)
        {
            options |= get_option_flag(arguments[i][j]);
        }
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
