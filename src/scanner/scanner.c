#include <stdlib.h>
#include "scanner.h"
#include "file_entry.h"
#include "directory.h"
#include "libft.h"
#include "path_builder.h"

#define ENTRY_PUSH_SUCCESS 1
#define ENTRY_PUSH_FAILURE 0

static int push_entry(t_file_entry_array *file_entry_array, const t_dir_entry *dir_entry, const char *directory_path)
{
    if (directory_is_entry_hidden_file(dir_entry))
        return ENTRY_PUSH_SUCCESS;

    int error_code = ENTRY_PUSH_SUCCESS;
    const char *entry_name = directory_get_entry_name(dir_entry);
    char *full_path = build_path(directory_path, entry_name);

    t_file_entry *entry = file_entry_create(entry_name);
    t_file_stats *entry_stats = file_stats_get_without_following_symlinks(full_path);
    if (entry_stats == NULL)
    {
        error_code = ENTRY_PUSH_FAILURE;
    }
    else
    {
        file_entry_set_file_type(entry, file_stats_get_file_type(entry_stats));
    }

    free(full_path);
    file_stats_destroy(&entry_stats);
    file_entry_array_push(file_entry_array, entry);
    return error_code;
}

t_result *scan(const char *path)
{
    if (!ft_is_valid_path(path))
        return result_create_failed(NULL);

    int error_code = ENTRY_PUSH_SUCCESS;

    t_dir_stream *dir_stream = directory_open(path);
    if (dir_stream == NULL)
        return result_create_failed(NULL);

    t_file_entry_array *file_entry_array = file_entry_array_create();
    t_dir_entry *dir_entry = directory_get_next_entry(dir_stream);
    while (!directory_is_entry_empty(dir_entry))
    {
        error_code &= push_entry(file_entry_array, dir_entry, path);

        directory_destroy_entry(&dir_entry);
        dir_entry = directory_get_next_entry(dir_stream);
    }
    const int close_directory_error = directory_close(&dir_stream);
    const int directory_operation_failed = dir_entry == NULL || close_directory_error == -1;
    directory_destroy_entry(&dir_entry);

    if (error_code != ENTRY_PUSH_SUCCESS)
        return result_create_failed(file_entry_array);
    if (directory_operation_failed)
        return result_create_failed(file_entry_array);
    return result_create_successful(file_entry_array);
}
