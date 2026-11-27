#include <stddef.h>
#include "options.h"
#include "libft.h"

#define COLOR_OPTION "--color"

int options_is_option_argument(const char *argument)
{
    return argument != NULL && argument[0] == '-' && argument[1] != '\0';
}

static int parse_short_option(t_options *options, t_sort_type *sort_option, const char flag)
{
    switch (flag)
    {
        case 'C': options->format = FORMAT_COLUMNS; break;
        case 'l': options->format = FORMAT_LONG; break;
        case 'g':
            options->format = FORMAT_LONG;
            options->print_owner = 0;
            break;
        case 't': *sort_option = SORT_TIME; break;
        case 'U': *sort_option = SORT_NONE; break;
        case 'f':
            *sort_option = SORT_NONE;
            options->filter_hidden_files = 0;
            break;
        case 'u': options->time_type = TIME_ATIME; break;
        case 'r': options->sort_reverse = 1; break;
        case 'a': options->filter_hidden_files = 0; break;
        case 'd': options->immediate_dirs = 1; break;
        case 'R': options->recursion = 1; break;
        default: return 0;
    }
    return 1;
}

static void resolve_options(t_options *options, const t_sort_type sort_option)
{
    if (sort_option != SORT_NOT_SET)
        options->sort_type = sort_option;
    else if (options->format != FORMAT_LONG && options->time_type == TIME_ATIME)
        options->sort_type = SORT_TIME;

    if (options->immediate_dirs)
        options->recursion = 0;

    options->dereference_symlinks = !options->immediate_dirs && options->format != FORMAT_LONG;
}

static t_parsed_options get_default_options(const int stdout_is_terminal)
{
    t_parsed_options parsed_options = {
        .options = {
            .format = FORMAT_SINGLE_COLUMN,
            .sort_type = SORT_NAME,
            .time_type = TIME_MTIME,
            .print_owner = 1,
            .filter_hidden_files = 1,
            .dereference_symlinks = 1
        },
        .options_amount = 0,
        .invalid_option = '\0'
    };

    if (stdout_is_terminal)
        parsed_options.options.format = FORMAT_COLUMNS;

    return parsed_options;
}

t_parsed_options options_parse(const char **arguments, const int stdout_is_terminal)
{
    t_parsed_options parsed_options = get_default_options(stdout_is_terminal);
    t_sort_type sort_option = SORT_NOT_SET;

    for (int i = 0; arguments != NULL && arguments[i] != NULL; i++)
    {
        if (!options_is_option_argument(arguments[i]))
            continue;

        parsed_options.options_amount++;
        if (ft_are_string_equals(arguments[i], COLOR_OPTION))
        {
            parsed_options.options.color_files = 1;
            continue;
        }

        for (int j = 1; arguments[i][j] != '\0'; j++)
        {
            if (!parse_short_option(&parsed_options.options, &sort_option, arguments[i][j]))
            {
                parsed_options.invalid_option = arguments[i][j];
                return parsed_options;
            }
        }
    }
    resolve_options(&parsed_options.options, sort_option);

    return parsed_options;
}
