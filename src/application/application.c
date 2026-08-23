#include <stddef.h>
#include "application.h"
#include "renderer.h"
#include "error_codes.h"
#include "libft.h"
#include "scanner.h"

static void process_non_directory_file_operands(const t_parsed_arguments *parsed_arguments)
{
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(parsed_arguments);

    for (unsigned int i = 0; non_directory_file_operands[i] != NULL; i++)
    {
        t_file_entry_list *file_entry_list = file_entry_list_create(non_directory_file_operands[i]);
        t_render_context *non_directory_render_context = render_context_create_for_non_directory_file_operands();

        render(file_entry_list, non_directory_render_context);

        file_entry_list_destroy(&file_entry_list);
        render_context_destroy(&non_directory_render_context);
    }
}

static void process_directory_file_operands(const t_parsed_arguments *parsed_arguments)
{
    const char * const *file_operands = parsed_arguments_get_file_operands(parsed_arguments);
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(parsed_arguments);
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(parsed_arguments);
    const int num_of_file_operands = ft_str_matrix_length((char **) file_operands);
    const int has_directory_file_operands = directory_file_operands[0] != NULL;
    const int has_non_directory_file_operands = non_directory_file_operands[0] != NULL;
    const int should_print_directory_header = num_of_file_operands > 1 && has_directory_file_operands;

    for (unsigned int i = 0; directory_file_operands[i] != NULL; i++)
    {
        t_file_entry_list *file_entry_list = scan(directory_file_operands[i]);
        t_render_context *directory_render_context = render_context_create_for_directory_file_operands(directory_file_operands[i]);

        render_context_set_should_print_directory_header(directory_render_context, should_print_directory_header);
        render_context_set_should_print_directory_header_leading_newline(directory_render_context, has_non_directory_file_operands || i > 0);
        render(file_entry_list, directory_render_context);

        file_entry_list_destroy(&file_entry_list);
        render_context_destroy(&directory_render_context);
    }
}

int run_application(const t_parsed_arguments *parsed_arguments)
{
    process_non_directory_file_operands(parsed_arguments);
    process_directory_file_operands(parsed_arguments);

    return FT_LS_APPLICATION_SUCCESS;
}
