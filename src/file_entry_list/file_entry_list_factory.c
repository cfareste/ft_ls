#include <stdlib.h>
#include "file_entry_list_factory.h"
#include "file_stats.h"
#include "scanner.h"

t_file_entry_list *create_file_entry_list_from_path(const char *file_path)
{
    struct stat file_stats;

    if (get_file_stats(file_path, &file_stats) == FILE_STATS_COULD_NOT_RETRIEVE_STATS)
        return NULL;

    if (!S_ISDIR(file_stats.st_mode))
    {
        return file_entry_list_create(file_path);
    }

    return scan(file_path);
}
