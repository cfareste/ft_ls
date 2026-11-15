#include <stdlib.h>
#include "application.h"
#include "path_builder.h"
#include "renderer.h"
#include "scanner.h"
#include "sorter.h"
#include "libft.h"

typedef struct s_application_context
{
    const t_parsed_arguments *parsed_arguments;
    t_render_context *render_context;
    t_scanner_context *scanner_context;
    t_sorter_context *sorter_context;
    t_ft_ls_error_code error_code;
} t_application_context;

static void process_directory(t_application_context *context, const char *directory_path);

static t_file_entry *create_file_operand_entry(const t_parsed_arguments *parsed_arguments, const char *file_operand, t_file_stats *file_stats)
{
    t_file_entry *entry = file_entry_create_from_stats(file_operand, file_stats);

    if (parsed_arguments_is_long_format_active(parsed_arguments))
        file_entry_set_advanced_metadata_from_stats(entry, file_stats);
    return entry;
}

static t_file_entry_array *create_non_directory_entry_array(const t_parsed_arguments *parsed_arguments)
{
    const char * const *file_operands = parsed_arguments_get_non_directory_file_operands(parsed_arguments);
    t_file_stats * const *file_stats = parsed_arguments_get_non_directory_file_operands_stats(parsed_arguments);
    t_file_entry_array *entries = file_entry_array_create();

    for (unsigned int i = 0; file_operands[i] != NULL; i++)
    {
        t_file_entry *entry = create_file_operand_entry(parsed_arguments, file_operands[i], file_stats[i]);
        file_entry_array_push(entries, entry);
    }

    return entries;
}

static void process_non_directory_file_operands(const t_parsed_arguments *parsed_arguments, const t_render_context *render_context)
{
    t_file_entry_array *entries = create_non_directory_entry_array(parsed_arguments);

    render_entries(render_context, entries);

    file_entry_array_destroy(&entries);
}

static void update_error_code(t_application_context *context, const t_ft_ls_error_code error_code)
{
    if (error_code > context->error_code)
        context->error_code = error_code;
}

static t_ft_ls_error_code classify_scan_result(const t_application_context *context, const t_result *scan_result)
{
    if (result_has_succeed(scan_result))
        return FT_LS_APPLICATION_SUCCESS;

    const char *failed_path = result_get_error_context(scan_result);
    const int is_operand = parsed_arguments_is_file_operand(context->parsed_arguments, failed_path);

    return is_operand ? FT_LS_APPLICATION_MAJOR_ERROR : FT_LS_APPLICATION_MINOR_ERROR;
}

static t_file_entry_array *scan_directory(t_application_context *context, const char *directory_path)
{
    t_result *scan_result = scan(context->scanner_context, directory_path);

    update_error_code(context, classify_scan_result(context, scan_result));
    t_file_entry_array *entries = result_get_value(scan_result);

    result_destroy(&scan_result);
    return entries;
}

static int can_entry_be_recursed(const t_file_entry *entry)
{
    if (file_entry_get_file_type(entry) != FILE_TYPE_DIRECTORY)
        return 0;

    const char *name = file_entry_get_name(entry);
    return !ft_are_string_equals(name, ".") && !ft_are_string_equals(name, "..");
}

static void process_subdirectories(t_application_context *context, const char *directory_path, const t_file_entry_array *file_entry_array)
{
    const unsigned int entry_count = file_entry_array_get_length(file_entry_array);
    for (unsigned int i = 0; i < entry_count; i++)
    {
        const t_file_entry *entry = file_entry_array_get_at(file_entry_array, i);
        if (!can_entry_be_recursed(entry))
            continue;

        char *child_directory_path = build_full_path(directory_path, file_entry_get_name(entry));
        process_directory(context, child_directory_path);
        free(child_directory_path);
    }
}

static void process_directory(t_application_context *context, const char *directory_path)
{
    t_file_entry_array *entries = scan_directory(context, directory_path);

    file_entry_array_sort(context->sorter_context, entries);
    render_directory(context->render_context, directory_path, entries);

    if (parsed_arguments_is_option_active(context->parsed_arguments, OPTIONS_RECURSIVE))
        process_subdirectories(context, directory_path, entries);

    file_entry_array_destroy(&entries);
}

static void process_directory_file_operands(t_application_context *context)
{
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(context->parsed_arguments);

    for (unsigned int i = 0; directory_file_operands[i] != NULL; i++)
    {
        process_directory(context, directory_file_operands[i]);
    }
}

static void process_file_operands(t_application_context *context)
{
    process_non_directory_file_operands(context->parsed_arguments, context->render_context);
    render_types_separator(context->render_context);
    process_directory_file_operands(context);
}

static void initialize_context(t_application_context *context, const t_parsed_arguments *parsed_arguments, const t_result *parsing_arguments_result)
{
    context->parsed_arguments = parsed_arguments;
    context->render_context = render_context_create(parsed_arguments);
    context->scanner_context = scanner_context_create(parsed_arguments);
    context->sorter_context = sorter_context_create(parsed_arguments);
    context->error_code = result_has_failed(parsing_arguments_result) ? FT_LS_APPLICATION_MAJOR_ERROR : FT_LS_APPLICATION_SUCCESS;
}

static void destroy_context(t_application_context *context)
{
    render_context_destroy(&context->render_context);
    scanner_context_destroy(&context->scanner_context);
    sorter_context_destroy(&context->sorter_context);
}

t_ft_ls_error_code application_run(const t_result *parsing_arguments_result)
{
    const t_parsed_arguments *parsed_arguments = result_get_value(parsing_arguments_result);
    if (parsed_arguments == NULL)
        return FT_LS_APPLICATION_MAJOR_ERROR;

    t_application_context context;
    initialize_context(&context, parsed_arguments, parsing_arguments_result);

    process_file_operands(&context);

    destroy_context(&context);
    return context.error_code;
}
