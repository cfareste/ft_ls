#include <stdlib.h>
#include "scanner.h"
#include "file_data.h"
#include "directory.h"

t_file_data *scan(const char *path)
{
    DIR *dir_stream = directory_open(path);
    if (dir_stream == NULL)
        return NULL;

    t_file_data *file_data_list = NULL;
    struct dirent *dir_entry = directory_get_next_entry(dir_stream);
    while (dir_entry != NULL)
    {
        t_file_data *next = file_data_create(dir_entry->d_name);

        file_data_add_entry(&file_data_list, next);

        dir_entry = directory_get_next_entry(dir_stream);
    }

    directory_close(dir_stream);

    return file_data_list;
}
