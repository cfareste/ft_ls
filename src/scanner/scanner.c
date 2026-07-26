#include <stdlib.h>
#include "scanner.h"
#include "file_data.h"
#include "os_adapter.h"

t_file_data *scan(const char *path)
{
    if (path == NULL) return NULL;
    if (path[0] == '\0') return NULL;

    t_dir_stream *dir_stream = opendir_adapter(path);
    t_file_data *file_data_list = NULL;

    const t_dir_entry *dir_entry = readdir_adapter(dir_stream);
    while (dir_entry != NULL)
    {
        t_file_data *next = file_data_create();

        file_data_set_name(next, dir_entry_get_name(dir_entry));
        file_data_add_entry(&file_data_list, next);

        dir_entry = readdir_adapter(dir_stream);
    }

    closedir_adapter(&dir_stream);

    return file_data_list;
}
