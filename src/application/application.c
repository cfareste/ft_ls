#include <stddef.h>
#include "application.h"
#include "file_entry_list_factory.h"
#include "renderer.h"
#include "error_codes.h"

int run_application(const t_parsed_arguments *parsed_arguments)
{
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(parsed_arguments);
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(parsed_arguments);

    t_render_config *non_directory_render_config = render_config_create_for_non_directory_file_operands();
    for (unsigned int i = 0; non_directory_file_operands[i] != NULL; i++)
    {
        t_file_entry_list *file_entry_list = file_entry_list_create_from_path(non_directory_file_operands[i]);

        render(file_entry_list, non_directory_render_config);

        file_entry_list_destroy(&file_entry_list);
    }
    render_config_destroy(&non_directory_render_config);

    const int has_multiple_entries = non_directory_file_operands[0] != NULL;
    for (unsigned int i = 0; directory_file_operands[i] != NULL; i++)
    {
        t_render_config *directory_render_config = render_config_create_for_directory_file_operands(directory_file_operands[i]);
        render_config_set_should_print_directory_header(directory_render_config, has_multiple_entries);
        t_file_entry_list *file_entry_list = file_entry_list_create_from_path(directory_file_operands[i]);

        render(file_entry_list, directory_render_config);

        file_entry_list_destroy(&file_entry_list);
        render_config_destroy(&directory_render_config);
    }

    return FT_LS_APPLICATION_SUCCESS;
}
