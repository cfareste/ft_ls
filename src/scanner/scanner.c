#include <stdlib.h>
#include "scanner.h"
#include "file_entry_list.h"
#include "directory.h"

static int is_hidden_file(const char *file_entry_name)
{
    return file_entry_name[0] == '.';
}

t_file_entry_array *scan(const char *path)
{
    DIR *dir_stream = directory_open(path);

    t_file_entry_array *file_entry_array = NULL;
    const struct dirent *dir_entry = directory_get_next_entry(dir_stream);
    while (dir_entry != NULL)
    {
        if (!is_hidden_file(dir_entry->d_name))
        {
            t_file_entry_array *next = file_entry_array_create(dir_entry->d_name);
            file_entry_array_add_entry(&file_entry_array, next);
        }

        dir_entry = directory_get_next_entry(dir_stream);
    }

    directory_close(&dir_stream);
    return file_entry_array;
}
