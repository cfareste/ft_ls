#include <stdlib.h>
#include "scanner.h"
#include "file_data.h"
#include "os_adapter.h"

t_file_data *scan(const char *path)
{
    t_dir_stream *dir_stream = dir_stream_open(path);
    if (dir_stream == NULL)
        return NULL;

    t_file_data *file_data_list = NULL;
    t_dir_entry *dir_entry = dir_entry_get_next(dir_stream);
    while (!dir_entry_is_empty(dir_entry))
    {
        t_file_data *next = file_data_create();

        file_data_set_name(next, dir_entry_get_name(dir_entry));
        file_data_add_entry(&file_data_list, next);

        dir_entry_destroy(&dir_entry);
        dir_entry = dir_entry_get_next(dir_stream);
    }

    dir_entry_destroy(&dir_entry);
    dir_stream_close(&dir_stream);

    return file_data_list;
}
