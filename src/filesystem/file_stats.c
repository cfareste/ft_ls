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

    const int result = stat(file_path, stats);

    t_file_stats *file_stats = ft_safe_calloc(1, sizeof(t_file_stats));
    file_stats->type = get_file_type(stats->st_mode);
    free(file_stats); //TODO: delete

    return result;
}
