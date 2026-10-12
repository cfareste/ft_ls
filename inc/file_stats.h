#pragma once

#include "date.h"
#include "device.h"

typedef enum e_file_type
{
    FILE_TYPE_NONE,
    FILE_TYPE_UNKNOWN,
    FILE_TYPE_REGULAR,
    FILE_TYPE_DIRECTORY,
    FILE_TYPE_CHARDEVICE,
    FILE_TYPE_BLOCKDEVICE,
    FILE_TYPE_FIFO,
    FILE_TYPE_SYMLINK,
    FILE_TYPE_SOCKET
} t_file_type;

typedef enum e_permissions
{
    PERM_NONE = 0,
    PERM_OWNER_READ = 1 << 0,
    PERM_OWNER_WRITE = 1 << 1,
    PERM_OWNER_EXECUTE = 1 << 2,
    PERM_GROUP_READ = 1 << 3,
    PERM_GROUP_WRITE = 1 << 4,
    PERM_GROUP_EXECUTE = 1 << 5,
    PERM_OTHER_READ = 1 << 6,
    PERM_OTHER_WRITE = 1 << 7,
    PERM_OTHER_EXECUTE = 1 << 8,
    PERM_SETUID = 1 << 9,
    PERM_SETGID = 1 << 10,
    PERM_STICKY = 1 << 11
} t_permissions;

typedef struct s_file_stats t_file_stats;

t_file_stats *file_stats_get(const char *file_path);
t_file_stats *file_stats_get_without_following_symlinks(const char *file_path);
long file_stats_get_blocks(const t_file_stats *file_stats);
t_permissions file_stats_get_permissions(const t_file_stats *file_stats);
t_file_type file_stats_get_file_type(const t_file_stats *file_stats);
unsigned long file_stats_get_link_count(const t_file_stats *file_stats);
const char *file_stats_get_author(t_file_stats *stats);
const char *file_stats_get_group(t_file_stats *stats);
long file_stats_get_size(const t_file_stats *file_stats);
t_device file_stats_get_device(const t_file_stats *file_stats);
t_date file_stats_get_modification_date(const t_file_stats *file_stats);
const char *file_stats_get_target_pointed_by_link(t_file_stats *stats);
void file_stats_destroy(t_file_stats **file_stats);
