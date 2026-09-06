#pragma once

typedef enum e_file_type
{
    UNKNOWN_FILE_TYPE,
    REGULAR_FILE_TYPE,
    DIRECTORY_FILE_TYPE,
    CHARDEVICE_FILE_TYPE,
    BLOCKDEVICE_FILE_TYPE,
    FIFO_FILE_TYPE,
    SYMLINK_FILE_TYPE,
    SOCKET_FILE_TYPE
} t_file_type;

typedef struct s_file_stats t_file_stats;

t_file_stats *file_stats_create(const char *file_path);
t_file_type file_stats_get_file_type(const t_file_stats *file_stats);
void file_stats_destroy(t_file_stats **file_stats);
