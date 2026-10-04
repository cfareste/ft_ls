#pragma once

typedef enum e_options
{
    OPTIONS_NONE = 0,
    OPTIONS_RECURSIVE = 1 << 0,
    OPTIONS_LONG_FORMAT = 1 << 1
} t_options;

t_options options_get(const char **arguments);
int options_get_amount(const char **arguments);
