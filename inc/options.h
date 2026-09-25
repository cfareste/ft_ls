#pragma once

typedef enum e_options
{
    OPTIONS_NONE = 0,
    OPTIONS_RECURSIVE = 1 << 0
} t_options;

t_options options_get(int num_of_arguments, const char **arguments);
int options_get_amount(const char **arguments);
