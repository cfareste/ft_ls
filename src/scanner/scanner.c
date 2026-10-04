#include <stdlib.h>
#include "scanner.h"
#include "file_entry.h"
#include "directory.h"
#include "libft.h"
#include "path_builder.h"

typedef struct s_scan_context
{
    const char *path;
    t_dir_stream *dir_stream;
    t_file_entry_array *file_entry_array;
    char *failed_file;
} t_scan_context;

static char *push_entry(const t_scan_context *context, const t_dir_entry *dir_entry)
{
    if (directory_is_entry_hidden_file(dir_entry))
        return NULL;

    const char *entry_name = directory_get_entry_name(dir_entry);
    char *full_path = build_path(context->path, entry_name);
    t_file_entry *entry = file_entry_create(entry_name);
    t_file_stats *entry_stats = file_stats_get_without_following_symlinks(full_path);
    char *failed_file = NULL;

    if (entry_stats == NULL)
        failed_file = ft_safe_strdup(full_path);
    else
        file_entry_set_file_type(entry, file_stats_get_file_type(entry_stats));

    free(full_path);
    file_stats_destroy(&entry_stats);
    file_entry_array_push(context->file_entry_array, entry);
    return failed_file;
}

static int scan_directory_entries(t_scan_context *context)
{
    t_dir_entry *dir_entry = directory_get_next_entry(context->dir_stream);

    while (!directory_is_entry_empty(dir_entry))
    {
        char *entry_error = push_entry(context, dir_entry);

        if (entry_error != NULL)
        {
            free(context->failed_file);
            context->failed_file = entry_error;
        }

        directory_destroy_entry(&dir_entry);
        dir_entry = directory_get_next_entry(context->dir_stream);
    }

    const int read_failed = dir_entry == NULL;
    directory_destroy_entry(&dir_entry);
    return read_failed;
}

static t_result *create_scan_result(const t_scan_context *context, const int directory_operation_failed)
{
    if (context->failed_file != NULL)
        return result_create_failed(context->file_entry_array, context->failed_file);
    if (directory_operation_failed)
        return result_create_failed(context->file_entry_array, context->path);
    return result_create_successful(context->file_entry_array);
}

t_result *scan(const char *path)
{
    if (!ft_is_valid_path(path))
        return result_create_failed(NULL, NULL);

    t_scan_context context = {0};
    context.path = path;
    context.dir_stream = directory_open(path);
    if (context.dir_stream == NULL)
        return result_create_failed(NULL, path);

    context.file_entry_array = file_entry_array_create();
    const int read_failed = scan_directory_entries(&context);
    const int close_failed = directory_close(&context.dir_stream) == -1;

    const int directory_operation_failed = read_failed || close_failed;
    t_result *result = create_scan_result(&context, directory_operation_failed);
    free(context.failed_file);

    return result;
}
