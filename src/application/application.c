#include <stddef.h>
#include "application.h"
#include "renderer.h"
#include "error_codes.h"
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

        render_entries(file_entry_array, render_context);

        file_entry_array_destroy(&file_entry_array);
    }
}

static int process_directory_file_operands(const t_parsed_arguments *parsed_arguments, t_render_context *render_context)
{
    int error_code = FT_LS_APPLICATION_SUCCESS;
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(parsed_arguments);

    for (unsigned int i = 0; directory_file_operands[i] != NULL; i++)
    {
        t_result *result = scan(directory_file_operands[i]);
        t_file_entry_array *file_entry_array = result_get_value(result);

        if (result_has_failed(result))
            error_code = FT_LS_APPLICATION_MAJOR_ERROR;

        file_entry_array_sort(file_entry_array);

        render_directory(render_context, directory_file_operands[i], file_entry_array);

        result_destroy(&result);
        file_entry_array_destroy(&file_entry_array);
    }

    return error_code;
}

int application_run(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return FT_LS_APPLICATION_MAJOR_ERROR;

    t_render_context *render_context = render_context_create(parsed_arguments);

    process_non_directory_file_operands(parsed_arguments, render_context);
    render_types_separator(render_context);
    const int error_code = process_directory_file_operands(parsed_arguments, render_context);

    render_context_destroy(&render_context);

    return error_code;
}
