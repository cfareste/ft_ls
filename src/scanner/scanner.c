#include <stdlib.h>
#include "scanner.h"
#include "file_entry.h"
#include "directory.h"
#include "libft.h"
#include "path_builder.h"

struct s_scanner_context
{
    int retrieve_long_format_metadata;
    int sort_by_time;
    int recurse;
    int color_files;
    int retrieve_author;
    int filter_hidden_files;
};

typedef struct s_scan_state
{
    const t_scanner_context *scanner;
    const char *directory_path;
    t_dir_stream *directory_stream;
    char *failed_entry_path;
    int directory_operation_failed;
} t_scan_state;

t_scanner_context *scanner_context_create(const t_parsed_arguments *parsed_arguments)
{
    t_scanner_context *context = ft_safe_calloc(1, sizeof(t_scanner_context));

    context->retrieve_long_format_metadata = parsed_arguments_get_format(parsed_arguments) == FORMAT_LONG;
    context->sort_by_time = parsed_arguments_get_sort_type(parsed_arguments) == SORT_TIME;
    context->recurse = parsed_arguments_should_recurse(parsed_arguments);
    context->color_files = parsed_arguments_should_color_files(parsed_arguments);
    context->retrieve_author = parsed_arguments_should_print_owner(parsed_arguments);
    context->filter_hidden_files = parsed_arguments_should_filter_hidden_files(parsed_arguments);

    return context;
}

void scanner_context_destroy(t_scanner_context **context)
{
    if (context == NULL || *context == NULL)
        return;

    free(*context);
    *context = NULL;
}

static void record_failed_entry(t_scan_state *state, const char *entry_path)
{
    free(state->failed_entry_path);
    state->failed_entry_path = ft_safe_strdup(entry_path);
}

static int should_skip_entry(const t_scan_state *state, const t_dir_entry *entry)
{
    return state->scanner->filter_hidden_files && directory_is_entry_hidden_file(entry);
}

static void retrieve_entry_metadata(t_scan_state *state, t_file_entry *entry, t_file_stats *stats, const char *entry_path)
{
    if (!state->scanner->retrieve_long_format_metadata)
        return;

    file_entry_set_advanced_metadata_from_stats(entry, stats, state->scanner->retrieve_author);
    if (file_stats_target_retrieval_failed(stats) || file_stats_acl_retrieval_failed(stats))
        record_failed_entry(state, entry_path);
}

static int should_retrieve_entry_stats(const t_scanner_context *context, const t_file_type type)
{
    return context->retrieve_long_format_metadata || context->sort_by_time
        || (type == FILE_TYPE_UNKNOWN && (context->recurse || context->color_files))
        || (context->color_files && (type == FILE_TYPE_REGULAR || type == FILE_TYPE_DIRECTORY));
}

static void append_entry(t_scan_state *state, const t_dir_entry *directory_entry, t_file_entry_array *entries)
{
    if (should_skip_entry(state, directory_entry))
        return;

    const char *entry_name = directory_get_entry_name(directory_entry);
    const t_file_type type = directory_get_entry_type(directory_entry);
    char *entry_path = build_path(state->directory_path, entry_name);
    t_file_stats *stats = NULL;
    const int retrieve_stats = should_retrieve_entry_stats(state->scanner, type);
    if (retrieve_stats)
    {
        stats = file_stats_get_without_following_symlinks(entry_path);
        if (stats == NULL)
            record_failed_entry(state, entry_path);
    }

    t_file_entry *entry = file_entry_create_from_stats(entry_name, stats);
    if (!retrieve_stats)
        file_entry_set_file_type(entry, type);
    file_entry_set_path(entry, entry_path);
    retrieve_entry_metadata(state, entry, stats, entry_path);

    free(entry_path);
    file_stats_destroy(&stats);
    file_entry_array_push(entries, entry);
}

static void scan_directory_entries(t_scan_state *state, t_file_entry_array *entries)
{
    t_dir_entry *entry = directory_get_next_entry(state->directory_stream);

    while (!directory_is_entry_empty(entry))
    {
        append_entry(state, entry, entries);
        directory_destroy_entry(&entry);
        entry = directory_get_next_entry(state->directory_stream);
    }

    state->directory_operation_failed = entry == NULL;
    directory_destroy_entry(&entry);
}

static t_result *create_scan_result(const t_scan_state *state, t_file_entry_array *entries)
{
    if (state->directory_operation_failed)
        return result_create_failed(entries, state->directory_path);
    if (state->failed_entry_path != NULL)
        return result_create_failed(entries, state->failed_entry_path);

    return result_create_successful(entries);
}

t_result *scan(const t_scanner_context *context, const char *path)
{
    if (context == NULL || !ft_is_valid_path(path))
        return result_create_failed(NULL, NULL);

    t_scan_state state = {
        .scanner = context,
        .directory_path = path,
        .directory_stream = directory_open(path)
    };
    if (state.directory_stream == NULL)
        return result_create_failed(NULL, path);

    t_file_entry_array *entries = file_entry_array_create();
    scan_directory_entries(&state, entries);
    if (directory_close(&state.directory_stream) == -1)
        state.directory_operation_failed = 1;

    t_result *result = create_scan_result(&state, entries);
    free(state.failed_entry_path);

    return result;
}
