#pragma once

#include <sys/stat.h>

#define FILE_STATS_SUCCESS (0)
#define FILE_STATS_COULD_NOT_RETRIEVE_STATS (-1)

typedef enum e_file_type
{
    UNKNOWN_TYPE,
    REGULAR_TYPE,
    DIRECTORY_TYPE,
    CHARDEVICE_TYPE,
    BLOCKDEVICE_TYPE,
    FIFO_TYPE,
    SYMLINK_TYPE,
    SOCKET_TYPE
} t_file_type;

typedef struct s_file_stats t_file_stats;

int file_stats_get(const char *file_path, struct stat *stats);
