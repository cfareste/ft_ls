#include <stddef.h>
#include "application.h"
#include "file_entry_list_factory.h"
#include "renderer.h"
#include "error_codes.h"
#include "libft.h"

int run_application(const t_parsed_arguments *parsed_arguments)
{
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(parsed_arguments);
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(parsed_arguments);

    for (unsigned int i = 0; non_directory_file_operands[i] != NULL; i++)
    {
        t_file_entry_list *file_entry_list = create_file_entry_list_from_path(non_directory_file_operands[i]);

        render(file_entry_list);

        file_entry_list_destroy(&file_entry_list);
    }

    for (unsigned int i = 0; directory_file_operands[i] != NULL; i++)
    {
        if (non_directory_file_operands[0] != NULL)
            ft_printf("\n%s:\n", directory_file_operands[i]);

        t_file_entry_list *file_entry_list = create_file_entry_list_from_path(directory_file_operands[i]);

        render(file_entry_list);

        file_entry_list_destroy(&file_entry_list);
    }

    return FT_LS_APPLICATION_SUCCESS;
}
