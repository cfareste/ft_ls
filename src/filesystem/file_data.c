#include <stdlib.h>
#include "file_data.h"

int get_file_data(const char *file_path, struct stat *stats)
{
    if (file_path == NULL || *file_path == '\0')
        return FILE_DATA_COULD_NOT_RETRIEVE_DATA;

    return stat(file_path, stats);
}