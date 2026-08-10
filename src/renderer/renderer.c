#include <stdlib.h>
#include "libft.h"
#include "renderer.h"

void render(const t_file_entry_list *file_entry_list)
{
    if (file_entry_list == NULL)
        return;

    const t_file_entry_list *entry = file_entry_list;
    while (entry != NULL)
    {
        ft_printf("%s\n", file_entry_list_get_name(entry));
        entry = file_entry_list_get_next(entry);
    }
}
