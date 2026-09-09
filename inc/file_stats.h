#pragma once

typedef enum e_file_type
{
    FILE_TYPE_UNKNOWN,
    FILE_TYPE_REGULAR,
    FILE_TYPE_DIRECTORY,
    FILE_TYPE_CHARDEVICE,
    FILE_TYPE_BLOCKDEVICE,
    FILE_TYPE_FIFO,
    FILE_TYPE_SYMLINK,
    FILE_TYPE_SOCKET
} t_file_type;

typedef struct s_file_stats t_file_stats;

t_file_stats *file_stats_get(const char *file_path);
t_file_type file_stats_get_file_type(const t_file_stats *file_stats);
void file_stats_destroy(t_file_stats **file_stats);
