#include <stdlib.h>
#include "application.h"
#include "path_builder.h"
#include "renderer.h"
#include "scanner.h"

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

static void process_directory(const char *directory_path, const t_parsed_arguments *parsed_arguments, t_render_context *render_context, t_ft_ls_error_code *error_code)
{
    t_result *result = scan(directory_path);
    t_file_entry_array *file_entry_array = result_get_value(result);

    if (result_has_failed(result))
    {
        const char *failed_file = result_get_error_context(result);
        if (parsed_arguments_is_file_operand(parsed_arguments, failed_file))
        {
            *error_code = FT_LS_APPLICATION_MAJOR_ERROR;
        }
        else
        {
            *error_code = FT_LS_APPLICATION_MINOR_ERROR;
        }
    }

    file_entry_array_sort(file_entry_array);
    render_directory(render_context, directory_path, file_entry_array);

    if (parsed_arguments_is_option_active(parsed_arguments, OPTIONS_RECURSIVE))
    {
        const unsigned int entries_amount = file_entry_array_get_length(file_entry_array);
        for (unsigned int i = 0; i < entries_amount; i++)
        {
            const t_file_entry *entry = file_entry_array_get_at(file_entry_array, i);
            if (file_entry_get_file_type(entry) == FILE_TYPE_DIRECTORY)
            {
                char *subdir_path = build_full_path(directory_path, file_entry_get_name(entry));
                process_directory(subdir_path, parsed_arguments, render_context, error_code);
                free(subdir_path);
            }
        }
    }

    result_destroy(&result);
    file_entry_array_destroy(&file_entry_array);
}

static t_ft_ls_error_code process_directory_file_operands(const t_parsed_arguments *parsed_arguments, t_render_context *render_context)
{
    t_ft_ls_error_code error_code = FT_LS_APPLICATION_SUCCESS;
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(parsed_arguments);

    for (unsigned int i = 0; directory_file_operands[i] != NULL; i++)
    {
        process_directory(directory_file_operands[i], parsed_arguments, render_context, &error_code);
    }

    return error_code;
}

static t_ft_ls_error_code process_file_operands(const t_parsed_arguments *parsed_arguments, t_render_context *render_context)
{
    process_non_directory_file_operands(parsed_arguments, render_context);
    render_types_separator(render_context);
    return process_directory_file_operands(parsed_arguments, render_context);
}

static t_ft_ls_error_code get_ft_ls_error_code(const t_result *parsing_arguments_result, const t_ft_ls_error_code file_operands_code)
{
    if (result_has_failed(parsing_arguments_result))
        return FT_LS_APPLICATION_MAJOR_ERROR;

    return file_operands_code;
}

t_ft_ls_error_code application_run(const t_result *parsing_arguments_result)
{
    if (parsing_arguments_result == NULL)
        return FT_LS_APPLICATION_MAJOR_ERROR;

    const t_parsed_arguments *parsed_arguments = result_get_value(parsing_arguments_result);
    t_render_context *render_context = render_context_create(parsed_arguments);

    const t_ft_ls_error_code file_operands_error_code = process_file_operands(parsed_arguments, render_context);

    render_context_destroy(&render_context);
    return get_ft_ls_error_code(parsing_arguments_result, file_operands_error_code);
}
