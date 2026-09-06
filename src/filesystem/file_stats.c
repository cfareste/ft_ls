#include <stdlib.h>
#include "libft.h"
#include "file_stats.h"

struct s_file_stats
{
    t_file_type type;
};

static t_file_type get_file_type(const mode_t mode)
{
    t_file_type file_type = UNKNOWN_TYPE;

    if (S_ISREG(mode))
        file_type = REGULAR_TYPE;
    if (S_ISDIR(mode))
        file_type = DIRECTORY_TYPE;
    if (S_ISCHR(mode))
        file_type = CHARDEVICE_TYPE;
    if (S_ISBLK(mode))
        file_type = BLOCKDEVICE_TYPE;
    if (S_ISFIFO(mode))
        file_type = FIFO_TYPE;
    if (S_ISLNK(mode))
        file_type = SYMLINK_TYPE;
    if (S_ISSOCK(mode))
        file_type = SOCKET_TYPE;

    return file_type;
}

int file_stats_get(const char *file_path, struct stat *stats)
{
    if (!ft_is_valid_path(file_path) || stats == NULL)
        return FILE_STATS_COULD_NOT_RETRIEVE_STATS;

    return stat(file_path, stats);
}

t_file_stats *file_stats_create(const char *file_path)
{
    if (!ft_is_valid_path(file_path))
        return NULL;

    struct stat stats;
    file_stats_get(file_path, &stats);

    t_file_stats *file_stats = ft_safe_calloc(1, sizeof(t_file_stats));
    file_stats->type = get_file_type(stats.st_mode);

    return file_stats;
}

t_file_type file_stats_get_file_type(const t_file_stats *file_stats)
{
    return file_stats->type;
}

void file_stats_destroy(t_file_stats **file_stats)
{
    if (file_stats == NULL || *file_stats == NULL)
        return ;

    free(*file_stats);
    *file_stats = NULL;
}
