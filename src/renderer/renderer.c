#include <stdlib.h>
#include "libft.h"
#include "renderer.h"

void render(const t_file_data *file_list)
{
    if (file_list == NULL)
        return;

    unsigned int i = 0;
    const t_file_data *entry = file_list;
    const unsigned int num_entries = file_data_get_length(file_list);
    while (i < num_entries)
    {
        ft_printf("%s", file_data_get_name(entry));
        if (i < num_entries - 1)
            ft_printf("  ");
        entry = file_data_get_next(entry);
        i++;
    }
    ft_printf("\n");
}
