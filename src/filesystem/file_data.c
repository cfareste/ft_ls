#include "libft.h"
#include "file_data.h"

int get_file_data(const char *file_path, struct stat *stats)
{
    if (ft_is_str_empty(file_path))
        return FILE_DATA_COULD_NOT_RETRIEVE_DATA;

    return stat(file_path, stats);
}
