#include <stdlib.h>
#include "scanner.h"
#include "file_entry.h"
#include "directory.h"
#include "libft.h"

static void push_entry(t_file_entry_array *file_entry_array, const t_dir_entry *dir_entry)
{
    if (directory_is_entry_hidden_file(dir_entry))
        return ;

    const char *entry_name = directory_get_entry_name(dir_entry);
    t_file_entry *entry = file_entry_create(entry_name);

    file_entry_array_push(file_entry_array, entry);
}

t_file_entry_array *scan(const char *path)
{
    if (!ft_is_valid_path(path))
        return NULL;

    t_dir_stream *dir_stream = directory_open(path);

    if (dir_stream == NULL)
        return NULL;

    t_file_entry_array *file_entry_array = file_entry_array_create();
    t_dir_entry *dir_entry = directory_get_next_entry(dir_stream);
    while (!directory_is_entry_empty(dir_entry))
    {
        push_entry(file_entry_array, dir_entry);

        directory_destroy_entry(&dir_entry);
        dir_entry = directory_get_next_entry(dir_stream);
    }

    directory_destroy_entry(&dir_entry);
    directory_close(&dir_stream);

    return file_entry_array;
}
