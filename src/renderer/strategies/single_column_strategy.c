#include "render_strategies.h"
#include "libft.h"

void single_column_strategy(const t_file_entry_array *file_entry_array)
{
    const unsigned int count = file_entry_array_get_length(file_entry_array);
    for (unsigned int i = 0; i < count; i++)
    {
        const t_file_entry *file_entry = file_entry_array_get_at(file_entry_array, i);
        ft_printf("%s\n", file_entry_get_name(file_entry));
    }
}
