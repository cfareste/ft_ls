#include <stdlib.h>
#include "libft.h"
#include "file_data.h"

int get_file_stats(const char *file_path, struct stat *stats)
{
    if (ft_is_str_empty(file_path) || stats == NULL)
        return FILE_STATS_COULD_NOT_RETRIEVE_STATS;

    return stat(file_path, stats);
}
