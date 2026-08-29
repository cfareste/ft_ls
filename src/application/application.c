#include <stddef.h>
#include "application.h"
#include "renderer.h"
#include "error_codes.h"
#include "scanner.h"

static int check_if_should_print_directory_header(const t_parsed_arguments *parsed_arguments)
{
    const int has_multiple_file_operands = parsed_arguments_has_multiple_file_operands(parsed_arguments);
    const int has_directory_file_operands = parsed_arguments_has_directory_file_operands(parsed_arguments);

    return has_multiple_file_operands && has_directory_file_operands;
}

static void process_non_directory_file_operands(const t_parsed_arguments *parsed_arguments, t_render_context *render_context)
{
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(parsed_arguments);

    for (unsigned int i = 0; non_directory_file_operands[i] != NULL; i++)
    {
        t_file_entry_array *file_entry_array = file_entry_array_create();
        t_file_entry *file_entry = file_entry_create(non_directory_file_operands[i]);
        file_entry_array_push(file_entry_array, file_entry);

        render(file_entry_array, render_context);

        file_entry_array_destroy(&file_entry_array);
    }
}

static void process_directory_file_operands(const t_parsed_arguments *parsed_arguments, t_render_context *render_context)
{
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(parsed_arguments);
    const int should_print_directory_header = check_if_should_print_directory_header(parsed_arguments);

    for (unsigned int i = 0; directory_file_operands[i] != NULL; i++)
    {
        const char *directory_header = should_print_directory_header ? directory_file_operands[i] : NULL;
        t_file_entry_array *file_entry_array = scan(directory_file_operands[i]);

        render_context_set_directory_header(render_context, directory_header);
        render(file_entry_array, render_context);

        file_entry_array_destroy(&file_entry_array);
    }
}

int application_run(const t_parsed_arguments *parsed_arguments)
{
    t_render_context *render_context = render_context_create();

    process_non_directory_file_operands(parsed_arguments, render_context);
    process_directory_file_operands(parsed_arguments, render_context);

    render_context_destroy(&render_context);

    return FT_LS_APPLICATION_SUCCESS;
}
