#pragma once

#include "option_types.h"

typedef struct s_options
{
    t_format format;
    t_sort_type sort_type;
    t_time_type time_type;
    int print_owner;
    int sort_reverse;
    int filter_hidden_files;
    int immediate_dirs;
    int recursion;
    int color_files;
    int dereference_symlinks;
} t_options;

typedef struct s_parsed_options
{
    t_options options;
    int options_amount;
    char invalid_option;
} t_parsed_options;

t_parsed_options options_parse(const char **arguments, int stdout_is_terminal);
int options_is_option_argument(const char *argument);
