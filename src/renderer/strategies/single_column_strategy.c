#include "render_strategies.h"
#include "render_colors.h"
#include "libft.h"

void directory_entries_prefix_single_column_strategy(const t_parsed_arguments *parsed_arguments, const t_file_entry_array *file_entry_array)
{
    (void)parsed_arguments;
    (void)file_entry_array;
}

void single_column_strategy(const t_parsed_arguments *parsed_arguments, const t_file_entry_array *file_entry_array)
{
    if (file_entry_array == NULL)
        return;

    const unsigned int count = file_entry_array_get_length(file_entry_array);
    for (unsigned int i = 0; i < count; i++)
    {
        const t_file_entry *file_entry = file_entry_array_get_at(file_entry_array, i);
        render_colors_print_name(parsed_arguments, file_entry);
        ft_printf("\n");
    }
}
