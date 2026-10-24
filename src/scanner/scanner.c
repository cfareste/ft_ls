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
    int retrieve_advanced_metadata;
} t_current_scan_context;

struct s_scanner_context
{
    int retrieve_advanced_metadata;
};

t_scanner_context *scanner_context_create(const t_parsed_arguments *parsed_arguments)
{
    t_scanner_context *context = ft_safe_calloc(1, sizeof(t_scanner_context));
    context->retrieve_advanced_metadata = parsed_arguments_is_long_format_active(parsed_arguments);

    return context;
}

void scanner_context_destroy(t_scanner_context **context)
{
    if (context == NULL || *context == NULL)
        return;

    free(*context);
    *context = NULL;
}

static void push_entry(t_current_scan_context *context, const t_dir_entry *dir_entry, t_file_entry_array *file_entry_array)
{
    if (directory_is_entry_hidden_file(dir_entry))
        return ;

    const char *entry_name = directory_get_entry_name(dir_entry);
    char *full_path = build_path(context->directory_path, entry_name);
    t_file_stats *entry_stats = file_stats_get_without_following_symlinks(full_path);

    if (entry_stats == NULL)
    {
        free(context->failed_file);
        context->failed_file = ft_safe_strdup(full_path);
    }

    t_file_entry *entry = file_entry_create_from_stats(entry_name, entry_stats);
    if (context->retrieve_advanced_metadata)
        file_entry_set_advanced_metadata_from_stats(entry, entry_stats);

    free(full_path);
    file_stats_destroy(&entry_stats);
    file_entry_array_push(file_entry_array, entry);
}

static int scan_directory_entries(t_current_scan_context *context, t_file_entry_array *file_entry_array)
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

static t_result *create_scan_result(const t_current_scan_context *context, t_file_entry_array *file_entry_array)
{
    if (context->read_failed || context->close_failed)
        return result_create_failed(file_entry_array, context->directory_path);
    if (context->failed_file != NULL)
        return result_create_failed(file_entry_array, context->failed_file);

    return result_create_successful(file_entry_array);
}

t_result *scan(const t_scanner_context *scanner_context, const char *path)
{
    if (scanner_context == NULL || !ft_is_valid_path(path))
        return result_create_failed(NULL, NULL);

    t_current_scan_context context = {
        .directory_path = path,
        .dir_stream = directory_open(path),
        .retrieve_advanced_metadata = scanner_context->retrieve_advanced_metadata
    };
    if (context.dir_stream == NULL)
        return result_create_failed(NULL, path);

    t_file_entry_array *file_entry_array = file_entry_array_create();
    context.read_failed = scan_directory_entries(&context, file_entry_array);
    context.close_failed = directory_close(&context.dir_stream) == -1;

    t_result *result = create_scan_result(&context, file_entry_array);
    free(context.failed_file);

    return result;
}
