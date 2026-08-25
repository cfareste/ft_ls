#include <stdlib.h>
#include "scanner.h"
#include "file_entry_list.h"
#include "directory.h"

static int is_hidden_file(const char *file_entry_name)
{
    return file_entry_name[0] == '.';
}

t_file_entry_list *scan(const char *path)
{
    DIR *dir_stream = directory_open(path);

    t_file_entry_list *file_entry_list = NULL;
    const struct dirent *dir_entry = directory_get_next_entry(dir_stream);
    while (dir_entry != NULL)
    {
        if (!is_hidden_file(dir_entry->d_name))
        {
            t_file_entry_list *next = file_entry_list_create(dir_entry->d_name);

            file_entry_list_add_entry(&file_entry_list, next);
        }

        dir_entry = directory_get_next_entry(dir_stream);
    }

    directory_close(&dir_stream);
    return file_entry_list;
}
