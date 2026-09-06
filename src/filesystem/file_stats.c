#include <stdlib.h>
#include "libft.h"
#include "file_stats.h"

struct s_file_stats
{
    t_file_type type;
};

int file_stats_get(const char *file_path, struct stat *stats)
{
    if (!ft_is_valid_path(file_path) || stats == NULL)
        return FILE_STATS_COULD_NOT_RETRIEVE_STATS;

    return stat(file_path, stats);
}

t_file_stats *file_stats_create(const char *file_path)
{
    (void) file_path;
    return ft_safe_calloc(1, sizeof(t_file_stats));
}

void file_stats_destroy(t_file_stats **file_stats)
{
    free(*file_stats);
    *file_stats = NULL;
}
