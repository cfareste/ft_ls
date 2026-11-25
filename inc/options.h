#pragma once

typedef enum e_options
{
    OPTIONS_NONE = 0,
    OPTIONS_RECURSIVE = 1 << 0,
    OPTIONS_LONG_FORMAT = 1 << 1,
    OPTIONS_READ_ALL = 1 << 2,
    OPTIONS_REVERSE = 1 << 3,
    OPTIONS_TIME_SORTING = 1 << 4,
    OPTIONS_MULTI_COLUMN_FORMAT = 1 << 5,
    OPTIONS_COLOR = 1 << 6,
    OPTIONS_NO_SORT = 1 << 7,
    OPTIONS_IMMEDIATE_DIRS = 1 << 8,
    OPTIONS_LONG_FORMAT_NO_OWNER = 1 << 9,
    OPTIONS_ENABLE_ATIME = 1 << 10
} t_options;

t_options options_get(const char **arguments);
int options_get_amount(const char **arguments);
char options_get_invalid_option(const char **arguments);
