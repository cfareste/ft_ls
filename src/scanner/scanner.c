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

static t_result *create_scan_result(t_file_entry_array *file_entry_array, const int failed_to_read_directory, const int failed_to_close_directory)
{
    if (failed_to_read_directory || failed_to_close_directory)
        return result_create_failed(file_entry_array);

    return result_create_successful(file_entry_array);
}

t_result *scan(const char *path)
{
    if (!ft_is_valid_path(path))
        return result_create_failed(NULL);

    t_dir_stream *dir_stream = directory_open(path);
    if (dir_stream == NULL)
        return result_create_failed(NULL);

    t_file_entry_array *file_entry_array = file_entry_array_create();
    t_dir_entry *dir_entry = directory_get_next_entry(dir_stream);
    while (!directory_is_entry_empty(dir_entry))
    {
        push_entry(file_entry_array, dir_entry);

        directory_destroy_entry(&dir_entry);
        dir_entry = directory_get_next_entry(dir_stream);
    }
    const int failed_to_read_directory = dir_entry == NULL;
    const int failed_to_close_directory = directory_close(&dir_stream) == -1;

    directory_destroy_entry(&dir_entry);
    return create_scan_result(file_entry_array, failed_to_read_directory, failed_to_close_directory);
}
