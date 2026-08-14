#include <stdlib.h>
#include <sys/stat.h>
#include "scanner.h"
#include "file_entry_list.h"
#include "directory.h"
#include "file_data.h"

static t_file_entry_list *scan_directory(const char *path)
{
    DIR *dir_stream = directory_open(path);

    t_file_entry_list *file_entry_list = NULL;
    const struct dirent *dir_entry = directory_get_next_entry(dir_stream);
    while (dir_entry != NULL)
    {
        t_file_entry_list *next = file_entry_list_create(dir_entry->d_name);

        file_entry_list_add_entry(&file_entry_list, next);

        dir_entry = directory_get_next_entry(dir_stream);
    }

    directory_close(&dir_stream);
    return file_entry_list;
}

t_file_entry_list *scan(const char *path)
{
    struct stat file_stats;

    if (get_file_stats(path, &file_stats) == FILE_STATS_COULD_NOT_RETRIEVE_STATS)
        return NULL;

    if (!S_ISDIR(file_stats.st_mode))
    {
        return file_entry_list_create(path);
    }

    return scan_directory(path);
}
