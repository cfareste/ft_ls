#include <stdlib.h>
#include "application.h"
#include "path_builder.h"
#include "renderer.h"
#include "scanner.h"

typedef struct s_application_context
{
    const t_parsed_arguments *parsed_arguments;
    t_render_context *render_context;
    t_ft_ls_error_code error_code;
} t_application_context;

static void process_directory(t_application_context *context, const char *directory_path);

static t_file_entry_array *create_non_directory_entry_array(const char *non_directory_file_operand)
{
    t_file_entry_array *file_entry_array = file_entry_array_create();
    t_file_entry *file_entry = file_entry_create(non_directory_file_operand);
    file_entry_array_push(file_entry_array, file_entry);

    return file_entry_array;
}

static void process_non_directory_file_operands(const t_parsed_arguments *parsed_arguments, const t_render_context *render_context)
{
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(parsed_arguments);

    for (unsigned int i = 0; non_directory_file_operands[i] != NULL; i++)
    {
        t_file_entry_array *file_entry_array = create_non_directory_entry_array(non_directory_file_operands[i]);

        render_entries(render_context, file_entry_array);

        file_entry_array_destroy(&file_entry_array);
    }
}

static void update_error_code(t_application_context *context, const t_ft_ls_error_code error_code)
{
    if (context->error_code >= error_code)
        return ;

    context->error_code = error_code;
}

static t_ft_ls_error_code get_scan_error(const t_application_context *context, const t_result *scan_result)
{
    if (result_has_succeed(scan_result))
        return FT_LS_APPLICATION_SUCCESS;

    const char *failed_file = result_get_error_context(scan_result);
    const int is_file_operand = parsed_arguments_is_file_operand(context->parsed_arguments, failed_file);

    return is_file_operand ? FT_LS_APPLICATION_MAJOR_ERROR : FT_LS_APPLICATION_MINOR_ERROR;
}

static t_file_entry_array *get_directory_content(t_application_context *context, const char *directory_path)
{
    t_result *result = scan(directory_path);
    const t_ft_ls_error_code scan_error = get_scan_error(context, result);

    update_error_code(context, scan_error);
    t_file_entry_array *file_entry_array = result_get_value(result);
    file_entry_array_sort(file_entry_array);

    result_destroy(&result);
    return file_entry_array;
}

static void process_subdirectories(t_application_context *context, const char *directory_path, const t_file_entry_array *file_entry_array)
{
    const unsigned int entries_amount = file_entry_array_get_length(file_entry_array);
    for (unsigned int i = 0; i < entries_amount; i++)
    {
        const t_file_entry *entry = file_entry_array_get_at(file_entry_array, i);
        if (file_entry_get_file_type(entry) != FILE_TYPE_DIRECTORY)
            continue;

        char *subdir_path = build_full_path(directory_path, file_entry_get_name(entry));
        process_directory(context, subdir_path);
        free(subdir_path);
    }
}

static void process_directory(t_application_context *context, const char *directory_path)
{
    t_file_entry_array *file_entry_array = get_directory_content(context, directory_path);

    render_directory(context->render_context, directory_path, file_entry_array);
    if (parsed_arguments_is_option_active(context->parsed_arguments, OPTIONS_RECURSIVE))
        process_subdirectories(context, directory_path, file_entry_array);

    file_entry_array_destroy(&file_entry_array);
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

t_ft_ls_error_code application_run(const t_result *parsing_arguments_result)
{
    if (parsing_arguments_result == NULL)
        return FT_LS_APPLICATION_MAJOR_ERROR;

    const t_parsed_arguments *parsed_arguments = result_get_value(parsing_arguments_result);
    t_render_context *render_context = render_context_create(parsed_arguments);
    t_application_context context = {
        .parsed_arguments = parsed_arguments,
        .render_context = render_context,
        .error_code = result_has_failed(parsing_arguments_result) ? FT_LS_APPLICATION_MAJOR_ERROR : FT_LS_APPLICATION_SUCCESS
    };

    process_file_operands(&context);

    render_context_destroy(&render_context);
    return context.error_code;
}
