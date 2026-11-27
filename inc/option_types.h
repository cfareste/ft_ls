#pragma once

typedef enum e_format
{
    FORMAT_SINGLE_COLUMN,
    FORMAT_COLUMNS,
    FORMAT_LONG
} t_format;

typedef enum e_sort_type
{
    SORT_NOT_SET,
    SORT_NAME,
    SORT_TIME,
    SORT_NONE
} t_sort_type;

typedef enum e_time_type
{
    TIME_MTIME,
    TIME_ATIME
} t_time_type;
