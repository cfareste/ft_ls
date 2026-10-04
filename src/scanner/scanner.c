#include <stdlib.h>
#include "scanner.h"
#include "file_entry.h"
#include "directory.h"
#include "libft.h"
#include "path_builder.h"

typedef struct s_scan_context
{
    const char *directory_path;
    t_dir_stream *dir_stream;
    char *failed_file;
    int read_failed;
    int close_failed;
} t_scan_context;

static void push_entry(t_scan_context *context, const t_dir_entry *dir_entry, t_file_entry_array *file_entry_array)
{
    if (directory_is_entry_hidden_file(dir_entry))
        return ;

    const char *entry_name = directory_get_entry_name(dir_entry);
    char *full_path = build_path(context->directory_path, entry_name);
    t_file_entry *entry = file_entry_create(entry_name);
    t_file_stats *entry_stats = file_stats_get_without_following_symlinks(full_path);

    if (entry_stats == NULL)
    {
        free(context->failed_file);
        context->failed_file = ft_safe_strdup(full_path);
    }

    file_entry_set_file_type(entry, file_stats_get_file_type(entry_stats));

    free(full_path);
    file_stats_destroy(&entry_stats);
    file_entry_array_push(file_entry_array, entry);
}

static int scan_directory_entries(t_scan_context *context, t_file_entry_array *file_entry_array)
{
    t_dir_entry *dir_entry = directory_get_next_entry(context->dir_stream);

    while (!directory_is_entry_empty(dir_entry))
    {
        push_entry(context, dir_entry, file_entry_array);

        directory_destroy_entry(&dir_entry);
        dir_entry = directory_get_next_entry(context->dir_stream);
    }

    const int read_failed = dir_entry == NULL;
    directory_destroy_entry(&dir_entry);
    return read_failed;
}

static t_result *create_scan_result(const t_scan_context *context, t_file_entry_array *file_entry_array)
{
    if (context->failed_file != NULL)
        return result_create_failed(file_entry_array, context->failed_file);
    if (context->read_failed || context->close_failed)
        return result_create_failed(file_entry_array, context->directory_path);

    return result_create_successful(file_entry_array);
}

t_result *scan(const char *path)
{
    if (!ft_is_valid_path(path))
        return result_create_failed(NULL, NULL);

    t_scan_context context = { .directory_path = path, .dir_stream = directory_open(path) };
    if (context.dir_stream == NULL)
        return result_create_failed(NULL, path);

    t_file_entry_array *file_entry_array = file_entry_array_create();
    context.read_failed = scan_directory_entries(&context, file_entry_array);
    context.close_failed = directory_close(&context.dir_stream) == -1;

    t_result *result = create_scan_result(&context, file_entry_array);
    free(context.failed_file);

    return result;
}
