#include <stdlib.h>
#include <sys/stat.h>
#include "scanner.h"
#include "file_data.h"
#include "directory.h"

static t_file_data *scan_directory(const char *path)
{
    DIR *dir_stream = directory_open(path);

    t_file_data *file_data_list = NULL;
    const struct dirent *dir_entry = directory_get_next_entry(dir_stream);
    while (dir_entry != NULL)
    {
        t_file_data *next = file_data_create(dir_entry->d_name);

        file_data_add_entry(&file_data_list, next);

        dir_entry = directory_get_next_entry(dir_stream);
    }

    directory_close(&dir_stream);
    return file_data_list;
}

t_file_data *scan(const char *path)
{
    if (path == NULL || *path == '\0')
        return NULL;

    struct stat st;

    stat(path, &st);

    if (!S_ISDIR(st.st_mode))
    {
        return file_data_create(path);
    }

    return scan_directory(path);
}
