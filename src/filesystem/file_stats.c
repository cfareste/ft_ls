#include <stdlib.h>
#include "libft.h"
#include "file_stats.h"

int file_stats_get(const char *file_path, struct stat *stats)
{
    if (!ft_is_valid_path(file_path) || stats == NULL)
        return FILE_STATS_COULD_NOT_RETRIEVE_STATS;

    return stat(file_path, stats);
}
