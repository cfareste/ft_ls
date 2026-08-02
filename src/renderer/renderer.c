#include <stdlib.h>
#include "libft.h"
#include "renderer.h"

void render(const t_file_data *file_list)
{
    if (file_list == NULL)
        return;

    const t_file_data *entry = file_list;
    while (entry != NULL)
    {
        ft_printf("%s", file_data_get_name(entry));
        if (file_data_get_next(entry) != NULL)
            ft_printf("  ");
        entry = file_data_get_next(entry);
    }
    ft_printf("\n");
}
