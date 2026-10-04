#include <stdlib.h>
#include "scanner.h"
#include "file_entry.h"
#include "directory.h"
#include "libft.h"
#include "path_builder.h"

static char *push_entry(const t_dir_entry *dir_entry, t_file_entry_array *file_entry_array, const char *parent_path)
{
    if (directory_is_entry_hidden_file(dir_entry))
        return NULL;

    const char *entry_name = directory_get_entry_name(dir_entry);
    char *full_path = build_path(parent_path, entry_name);
    t_file_entry *entry = file_entry_create(entry_name);
    t_file_stats *entry_stats = file_stats_get_without_following_symlinks(full_path);
    char *failed_file = NULL;

    if (entry_stats == NULL)
        failed_file = ft_safe_strdup(full_path);
    else
        file_entry_set_file_type(entry, file_stats_get_file_type(entry_stats));

    free(full_path);
    file_stats_destroy(&entry_stats);
    file_entry_array_push(file_entry_array, entry);
    return failed_file;
}

static int scan_directory_entries(const t_dir_stream *dir_stream, t_file_entry_array *file_entry_array,
                                  const char *parent_path, char **failed_file)
{
    t_dir_entry *dir_entry = directory_get_next_entry(dir_stream);

    while (!directory_is_entry_empty(dir_entry))
    {
        char *entry_error = push_entry(dir_entry, file_entry_array, parent_path);

        if (entry_error != NULL)
        {
            free(*failed_file);
            *failed_file = entry_error;
        }

        directory_destroy_entry(&dir_entry);
        dir_entry = directory_get_next_entry(dir_stream);
    }

    const int read_failed = dir_entry == NULL;
    directory_destroy_entry(&dir_entry);
    return read_failed;
}

static t_result *create_scan_result(t_file_entry_array *file_entry_array, const char *path,
                                    const char *failed_file, const int directory_operation_failed)
{
    if (failed_file != NULL)
        return result_create_failed(file_entry_array, failed_file);
    if (directory_operation_failed)
        return result_create_failed(file_entry_array, path);
    return result_create_successful(file_entry_array);
}

t_result *scan(const char *path)
{
    if (!ft_is_valid_path(path))
        return result_create_failed(NULL, NULL);

    t_dir_stream *dir_stream = directory_open(path);
    if (dir_stream == NULL)
        return result_create_failed(NULL, path);

    char *failed_file = NULL;
    t_file_entry_array *file_entry_array = file_entry_array_create();

    const int read_failed = scan_directory_entries(dir_stream, file_entry_array, path, &failed_file);
    const int close_failed = directory_close(&dir_stream) == -1;

    const int directory_operation_failed = read_failed || close_failed;
    t_result *result = create_scan_result(file_entry_array, path, failed_file, directory_operation_failed);
    free(failed_file);

    return result;
}
