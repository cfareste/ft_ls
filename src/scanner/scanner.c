#include <stdlib.h>
#include "scanner.h"
#include "file_entry.h"
#include "directory.h"
#include "libft.h"

t_file_entry_array *scan(const char *path)
{
    if (!ft_is_valid_path(path))
        return NULL;

    t_dir_stream *dir_stream = directory_open(path);

    t_file_entry_array *file_entry_array = file_entry_array_create();
    t_dir_entry *dir_entry = directory_get_next_entry(dir_stream);
    while (!directory_is_entry_empty(dir_entry))
    {
        const char *entry_name = directory_get_entry_name(dir_entry);
        if (!directory_is_entry_hidden_file(dir_entry))
        {
            t_file_entry *entry = file_entry_create(entry_name);
            file_entry_array_push(file_entry_array, entry);
        }

        directory_destroy_entry(&dir_entry);
        dir_entry = directory_get_next_entry(dir_stream);
    }

    directory_destroy_entry(&dir_entry);
    directory_close(&dir_stream);

    return file_entry_array;
}
