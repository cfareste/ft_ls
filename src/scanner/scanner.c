#include <stdlib.h>
#include "scanner.h"
#include "file_entry.h"
#include "directory.h"
#include "libft.h"
#include "path_builder.h"

struct s_scanner_context
{
    int retrieve_long_format_metadata;
    int include_hidden_entries;
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

    context->retrieve_long_format_metadata = parsed_arguments_is_long_format_active(parsed_arguments);
    context->include_hidden_entries = parsed_arguments_is_option_active(parsed_arguments, OPTIONS_READ_ALL);

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
    return !state->scanner->include_hidden_entries && directory_is_entry_hidden_file(entry);
}

static void retrieve_entry_metadata(t_scan_state *state, t_file_entry *entry, t_file_stats *stats, const char *entry_path)
{
    if (!state->scanner->retrieve_long_format_metadata)
        return;

    file_entry_set_advanced_metadata_from_stats(entry, stats);
    if (file_stats_target_retrieval_failed(stats))
        record_failed_entry(state, entry_path);
}

static void append_entry(t_scan_state *state, const t_dir_entry *directory_entry, t_file_entry_array *entries)
{
    if (should_skip_entry(state, directory_entry))
        return;

    const char *entry_name = directory_get_entry_name(directory_entry);
    char *entry_path = build_path(state->directory_path, entry_name);
    t_file_stats *stats = file_stats_get_without_following_symlinks(entry_path);

    if (stats == NULL)
        record_failed_entry(state, entry_path);

    t_file_entry *entry = file_entry_create_from_stats(entry_name, stats);
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
