#include "render_strategies.h"
#include "libft.h"

void long_format_strategy(const t_file_entry_array *file_entry_array)
{
    (void) file_entry_array;
    ft_printf("-rw-rw-r-- 2 bob developers 1024 dec 12 12:30 file\n");
}
