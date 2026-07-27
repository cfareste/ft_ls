#include <stdlib.h>
#include "scanner.h"
#include "file_data.h"
#include "os_adapter.h"

static int is_invalid_path(const char *path)
{
    return path == NULL || path[0] == '\0';
}

static t_dir_stream *get_dir_stream(const char *path)
{
    if (is_invalid_path(path))
        return NULL;

    return opendir_adapter(path);
}

t_file_data *scan(const char *path)
{
    t_dir_stream *dir_stream = get_dir_stream(path);
    if (dir_stream == NULL)
        return NULL;

    t_file_data *file_data_list = NULL;
    t_dir_entry *dir_entry = readdir_adapter(dir_stream);
    while (!dir_entry_is_null(dir_entry))
    {
        t_file_data *next = file_data_create();

        file_data_set_name(next, dir_entry_get_name(dir_entry));
        file_data_add_entry(&file_data_list, next);

        dir_entry_destroy(&dir_entry);
        dir_entry = readdir_adapter(dir_stream);
    }

    dir_entry_destroy(&dir_entry);
    closedir_adapter(&dir_stream);

    return file_data_list;
}
